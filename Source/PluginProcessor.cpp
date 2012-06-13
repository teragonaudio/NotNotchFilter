#include "PluginProcessor.h"

//==============================================================================
NotNotchFilterAudioProcessor::NotNotchFilterAudioProcessor() {
  baseFrequency = kNotNotchFilterFrequencyDefault;
  resonance = kNotNotchFilterResonanceDefault;
  valleySize = kNotNotchFilterValleySizeDefault;
  recalculateCoefficients(getSampleRate(), baseFrequency, resonance);
}

NotNotchFilterAudioProcessor::~NotNotchFilterAudioProcessor() {
}

//==============================================================================
const String NotNotchFilterAudioProcessor::getName() const {
  return JucePlugin_Name;
}

int NotNotchFilterAudioProcessor::getNumParameters() {
  return kNotNotchFilterParamNumParams;
}

static float scaleFrequencyToParameterRange(float value, float max, float min) {
  return (logf(value) - logf(min)) / (logf(max) - logf(min));
}

float NotNotchFilterAudioProcessor::getParameter(int index) {
  switch(index) {
    case kNotNotchFilterParamFilterFrequency:
      return scaleFrequencyToParameterRange(baseFrequency, kNotNotchFilterFrequencyMax, kNotNotchFilterFrequencyMin);
    case kNotNotchFilterParamResonance:
      return (resonance - kNotNotchFilterResonanceMin) / (kNotNotchFilterResonanceMax - kNotNotchFilterResonanceMin);
    case kNotNotchFilterParamValleySize:
      return scaleFrequencyToParameterRange(valleySize, kNotNotchFilterValleySizeMax, kNotNotchFilterValleySizeMin);
    default:
      return 0.0f;
  }
}

static float scaleParameterRangeToFrequency(float value, float max, float min) {
  float frequency = expf(value * (logf(max) - logf(min)) + logf(min));
  if(frequency > max) {
    return max;
  }
  else if(frequency < min) {
    return min;
  }
  else {
    return frequency;
  }
}

void NotNotchFilterAudioProcessor::setParameter(int index, float newValue) {
  switch(index) {
    case kNotNotchFilterParamFilterFrequency:
      baseFrequency = scaleParameterRangeToFrequency(newValue, kNotNotchFilterFrequencyMax, kNotNotchFilterFrequencyMin);
      break;
    case kNotNotchFilterParamResonance:
      resonance = newValue * (kNotNotchFilterResonanceMax - kNotNotchFilterResonanceMin) + kNotNotchFilterResonanceMin;
      break;
    case kNotNotchFilterParamValleySize:
      valleySize = scaleParameterRangeToFrequency(newValue, kNotNotchFilterValleySizeMax, kNotNotchFilterValleySizeMin);
      break;
    default:
      break;
  }

  recalculateCoefficients(getSampleRate(), baseFrequency, resonance);
}

const String NotNotchFilterAudioProcessor::getParameterName(int index) {
  switch(index) {
    case kNotNotchFilterParamFilterFrequency:
      return String("Frequency");
    case kNotNotchFilterParamResonance:
      return String("Resonance");
    case kNotNotchFilterParamValleySize:
      return String("Valley Size");
    default:
      return String::empty;
  }
}

static const String getParameterTextForFrequency(const float frequency) {
  String outText;
  if(frequency > 1000) {
    outText = String(frequency / 1000.0f, PARAM_TEXT_NUM_DECIMAL_PLACES);
    outText.append(String(" kHz"), 4);
  }
  else {
    outText = String(frequency, PARAM_TEXT_NUM_DECIMAL_PLACES);
    outText.append(String(" Hz"), 3);
  }
  return outText;
}

const String NotNotchFilterAudioProcessor::getParameterText(int index) {
  switch(index) {
    case kNotNotchFilterParamFilterFrequency:
      return getParameterTextForFrequency(baseFrequency);
    case kNotNotchFilterParamResonance:
      return String(resonance, PARAM_TEXT_NUM_DECIMAL_PLACES);
    case kNotNotchFilterParamValleySize:
      return getParameterTextForFrequency(valleySize);
    default:
      return String::empty;
  }
}

//==============================================================================
void NotNotchFilterAudioProcessor::resetLastIOData() {
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

void NotNotchFilterAudioProcessor::recalculateCoefficients(const double sampleRate, const float baseFrequency, const float filterResonance) {
  loFrequency = baseFrequency + valleySize;
  if(loFrequency > kNotNotchFilterFrequencyMax) {
    loFrequency = kNotNotchFilterFrequencyMax;
  }

  const float hiCoeffConstant = (float)tan(M_PI * loFrequency / sampleRate);
  hiCoeffA1 = 1.0f / ((1.0f + filterResonance * hiCoeffConstant) + (hiCoeffConstant * hiCoeffConstant));
  hiCoeffA2 = -2.0f * hiCoeffA1;
  hiCoeffB1 = 2.0f * hiCoeffA1 * ((hiCoeffConstant * hiCoeffConstant) - 1.0f);
  hiCoeffB2 = hiCoeffA1 * (1.0f - (filterResonance * hiCoeffConstant) + (hiCoeffConstant * hiCoeffConstant));

  hiFrequency = baseFrequency;

  const float loCoeffConstant = (float)(1.0f / tan(hiFrequency / sampleRate));
  loCoeffA1 = 1.0f / (1.0f + (filterResonance * loCoeffConstant) + (loCoeffConstant * loCoeffConstant));
  loCoeffA2 = 2.0f * loCoeffA1;
  loCoeffB1 = 2.0f * loCoeffA1 * (1.0f - (loCoeffConstant * loCoeffConstant));
  loCoeffB2 = loCoeffA1 * (1.0f - (filterResonance * loCoeffConstant) + (loCoeffConstant * loCoeffConstant));
}

void NotNotchFilterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  resetLastIOData();
  recalculateCoefficients(sampleRate, baseFrequency, resonance);
}

void NotNotchFilterAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void NotNotchFilterAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
  // Pass audio through if valley size is set to min. We want to have a clean signal when the filter is off.
  if(valleySize > kNotNotchFilterValleySizeMin) {
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
void NotNotchFilterAudioProcessor::getStateInformation(MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void NotNotchFilterAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new NotNotchFilterAudioProcessor();
}
