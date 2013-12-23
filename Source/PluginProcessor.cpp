#include "PluginProcessor.h"

#if WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#endif

NotNotchFilterAudioProcessor::NotNotchFilterAudioProcessor() :
AudioProcessor(),
PluginParameterObserver() {
    parameters.add(new FrequencyParameter("Frequency", kFrequencyMin, kFrequencyMax, kFrequencyDefault));
    parameters.add(new FloatParameter("Resonance", kResonanceMin, kResonanceMax, kResonanceDefault));
    parameters.add(new FrequencyParameter("Valley Size", kValleySizeMin, kValleySizeMax, kValleySizeDefault));
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

void NotNotchFilterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    reset();

    float baseFrequency = parameters["Frequency"]->getValue();
    float filterResonance = parameters["Resonance"]->getValue();
    maxFilterFrequency = (float)(getSampleRate() / 2.0f) - 10.0f;

    loFrequency = baseFrequency + parameters["Valley Size"]->getValue();
    if(loFrequency > maxFilterFrequency) {
        loFrequency = maxFilterFrequency;
    }

    const float hiCoeffConstant = (float)tan(M_PI * loFrequency / sampleRate);
    hiCoeffA1 = 1.0f / ((1.0f + filterResonance * hiCoeffConstant) + (hiCoeffConstant * hiCoeffConstant));
    hiCoeffA2 = -2.0f * hiCoeffA1;
    hiCoeffB1 = 2.0f * hiCoeffA1 * ((hiCoeffConstant * hiCoeffConstant) - 1.0f);
    hiCoeffB2 = hiCoeffA1 * (1.0f - (filterResonance * hiCoeffConstant) + (hiCoeffConstant * hiCoeffConstant));

    hiFrequency = baseFrequency;// - valleySize;
    // Don't compare to the minimum notch frequency; instead we want to catch anything below 20Hz
    // as it could cause feedback in the filter or other artifacts.
    if(hiFrequency < 20) {
        hiFrequency = 20;
    }

    const float loCoeffConstant = (float)(1.0f / tan(hiFrequency / sampleRate));
    loCoeffA1 = 1.0f / (1.0f + (filterResonance * loCoeffConstant) + (loCoeffConstant * loCoeffConstant));
    loCoeffA2 = 2.0f * loCoeffA1;
    loCoeffB1 = 2.0f * loCoeffA1 * (1.0f - (loCoeffConstant * loCoeffConstant));
    loCoeffB2 = loCoeffA1 * (1.0f - (filterResonance * loCoeffConstant) + (loCoeffConstant * loCoeffConstant));
}

void NotNotchFilterAudioProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    // Pass audio through if valley size is set to min. We want to have a clean signal when the filter is off.
    if(parameters["Valley Size"]->getValue() > kValleySizeMin) {
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

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for(int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
void NotNotchFilterAudioProcessor::getStateInformation(MemoryBlock &destData) {
    XmlElement xml(getName());
    for(int i = 0; i < parameters.size(); i++) {
        PluginParameter *parameter = parameters[i];
        xml.setAttribute(parameter->getSafeName().c_str(), parameter->getValue());
    }
    copyXmlToBinary(xml, destData);
}

void NotNotchFilterAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
    ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if(xmlState != 0 && xmlState->hasTagName(getName())) {
        for(int i = 0; i < parameters.size(); i++) {
            PluginParameter *parameter = parameters[i];
            if(xmlState->hasAttribute(parameter->getSafeName().c_str())) {
                parameters.set(parameter, xmlState->getDoubleAttribute(parameter->getSafeName().c_str()));
            }
        }
        reset();
    }
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new NotNotchFilterAudioProcessor();
}
