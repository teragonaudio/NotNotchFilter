/*
 * Copyright (c) 2013 - Teragon Audio LLC
 *
 * Permission is granted to use this software under the terms of either:
 * a) the GPL v2 (or any later version)
 * b) the Affero GPL v3
 *
 * Details of these licenses can be found at: www.gnu.org/licenses
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * ------------------------------------------------------------------------------
 *
 * This software uses the JUCE library.
 *
 * To release a closed-source product which uses JUCE, commercial licenses are
 * available: visit www.juce.com for more information.
 */

#include "PluginProcessor.h"

#if WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#endif

NotNotchFilterAudioProcessor::NotNotchFilterAudioProcessor() :
TeragonPluginBase(), ParameterObserver() {
    frequency = new FrequencyParameter("Frequency",
                                       kFrequencyMin,
                                       kFrequencyMax,
                                       kFrequencyDefault);
    parameters.add(frequency);

    resonance = new FloatParameter("Resonance",
                                   kResonanceMin,
                                   kResonanceMax,
                                   kResonanceDefault);
    parameters.add(resonance);

    valleySize = new ValleyParameter("Valley Size");
    parameters.add(valleySize);

    for(int i = 0; i < parameters.size(); ++i) {
        parameters[i]->addObserver(this);
    }

    ParameterString version = ProjectInfo::projectName;
    version.append(" version ").append(ProjectInfo::versionString);
    parameters.add(new StringParameter("Version", version));
}

void NotNotchFilterAudioProcessor::onParameterUpdated(const Parameter *) {
    recalculateCoefficients(getSampleRate());
}

void NotNotchFilterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    TeragonPluginBase::prepareToPlay(sampleRate, samplesPerBlock);
    recalculateCoefficients(sampleRate);
    reset();
}

void NotNotchFilterAudioProcessor::recalculateCoefficients(const double sampleRate) {
    const float baseFrequency = (float)frequency->getValue();
    const float filterResonance = (float)resonance->getValue();
    maxFilterFrequency = (float)(sampleRate / 2.0f) - 10.0f;

    loFrequency = (float)(baseFrequency + valleySize->getValue());
    if(loFrequency > maxFilterFrequency) {
        loFrequency = maxFilterFrequency;
    }

    const float hiCoeffConstant = (float)tan(M_PI * loFrequency / sampleRate);
    hiCoeffA1 = 1.0f / ((1.0f + filterResonance * hiCoeffConstant) + (hiCoeffConstant * hiCoeffConstant));
    hiCoeffA2 = -2.0f * hiCoeffA1;
    hiCoeffB1 = 2.0f * hiCoeffA1 * ((hiCoeffConstant * hiCoeffConstant) - 1.0f);
    hiCoeffB2 = hiCoeffA1 * (1.0f - (filterResonance * hiCoeffConstant) + (hiCoeffConstant * hiCoeffConstant));

    hiFrequency = baseFrequency; // - valleySize;
    // Don't compare to the minimum notch frequency; instead we want to catch anything below 20Hz
    // as it could cause feedback in the filter or other artifacts.
    if(hiFrequency < kMinimumNotchFrequency) {
        hiFrequency = kMinimumNotchFrequency;
    }

    const float loCoeffConstant = (float)(1.0f / tan(hiFrequency / sampleRate));
    loCoeffA1 = 1.0f / (1.0f + (filterResonance * loCoeffConstant) + (loCoeffConstant * loCoeffConstant));
    loCoeffA2 = 2.0f * loCoeffA1;
    loCoeffB1 = 2.0f * loCoeffA1 * (1.0f - (loCoeffConstant * loCoeffConstant));
    loCoeffB2 = loCoeffA1 * (1.0f - (filterResonance * loCoeffConstant) + (loCoeffConstant * loCoeffConstant));
}

void NotNotchFilterAudioProcessor::reset() {
    for(int i = 0; i < 2; i++) {
        hiLastInput1[i] = 0.0f;
        hiLastInput2[i] = 0.0f;
        hiLastInput3[i] = 0.0f;
        hiLastOutput1[i] = 0.0f;
        hiLastOutput2[i] = 0.0f;

        loLastInput1[i] = 0.0f;
        loLastInput2[i] = 0.0f;
        loLastInput3[i] = 0.0f;
        loLastOutput1[i] = 0.0f;
        loLastOutput2[i] = 0.0f;
    }
}

void NotNotchFilterAudioProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    TeragonPluginBase::processBlock(buffer, midiMessages);

    // Pass audio through if valley size is set to min. We want to have a clean
    // signal when the filter is off.
    if(valleySize->getValue() > 0) {
        for(int channel = 0; channel < getNumInputChannels(); ++channel) {
            float *channelData = buffer.getSampleData(channel);
            float hiOutput, loOutput;
            for(int i = 0; i < buffer.getNumSamples(); i++) {
                hiLastInput3[channel] = hiLastInput2[channel];
                hiLastInput2[channel] = hiLastInput1[channel];
                hiLastInput1[channel] = channelData[i];

                hiOutput = (hiCoeffA1 * hiLastInput1[channel]) +
                    (hiCoeffA2 * hiLastInput2[channel]) +
                    (hiCoeffA1 * hiLastInput3[channel]) -
                    (hiCoeffB1 * hiLastOutput1[channel]) -
                    (hiCoeffB2 * hiLastOutput2[channel]);

                hiLastOutput2[channel] = hiLastOutput1[channel];
                hiLastOutput1[channel] = hiOutput;

                loLastInput3[channel] = loLastInput2[channel];
                loLastInput2[channel] = loLastInput1[channel];
                loLastInput1[channel] = channelData[i];

                loOutput = (loCoeffA1 * loLastInput1[channel]) +
                    (loCoeffA2 * loLastInput2[channel]) +
                    (loCoeffA1 * loLastInput3[channel]) -
                    (loCoeffB1 * loLastOutput1[channel]) -
                    (loCoeffB2 * loLastOutput2[channel]);

                loLastOutput2[channel] = loLastOutput1[channel];
                loLastOutput1[channel] = loOutput;

                channelData[i] = hiOutput + loOutput;
            }
        }
    }
}

void NotNotchFilterAudioProcessor::releaseResources() {
    TeragonPluginBase::releaseResources();
}

AudioProcessorEditor *NotNotchFilterAudioProcessor::createEditor() {
    return new PluginEditor(this, parameters, Resources::getCache());
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new NotNotchFilterAudioProcessor();
}
