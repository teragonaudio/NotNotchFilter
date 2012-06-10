#include "PluginProcessor.h"

//==============================================================================
HangingValleyAudioProcessor::HangingValleyAudioProcessor() {
}

HangingValleyAudioProcessor::~HangingValleyAudioProcessor() {
}

//==============================================================================
const String HangingValleyAudioProcessor::getName() const {
  return JucePlugin_Name;
}

int HangingValleyAudioProcessor::getNumParameters() {
  return kHangingValleyParamNumParams;
}

static float scaleFrequencyToParameterRange(float value, float max, float min) {
  return (logf(value) - logf(min)) / (logf(max) - logf(min));
}

float HangingValleyAudioProcessor::getParameter(int index) {
  switch(index) {
    case kHangingValleyParamFilterFrequency:
      return scaleFrequencyToParameterRange(frequency, kHangingValleyFrequencyMax, kHangingValleyFrequencyMin);
    case kHangingValleyParamResonance:
      return (resonance - kHangingValleyResonanceMin) / (kHangingValleyResonanceMax - kHangingValleyResonanceMin);
    case kHangingValleyParamValleySize:
      return scaleFrequencyToParameterRange(valleySize, kHangingValleyFrequencyMax, kHangingValleyFrequencyMin);
    default:
      return 0.0f;
  };
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

void HangingValleyAudioProcessor::setParameter(int index, float newValue) {
  switch(index) {
    case kHangingValleyParamFilterFrequency:
      frequency = scaleParameterRangeToFrequency(newValue, kHangingValleyFrequencyMax, kHangingValleyFrequencyMin);
      break;
    case kHangingValleyParamResonance:
      resonance = newValue * (kHangingValleyResonanceMax - kHangingValleyResonanceMin) + kHangingValleyResonanceMin;
      break;
    case kHangingValleyParamValleySize:
      valleySize = scaleParameterRangeToFrequency(newValue, kHangingValleyFrequencyMax, kHangingValleyFrequencyMin);
      break;
    default:
      break;
  }
}

const String HangingValleyAudioProcessor::getParameterName(int index) {
  switch(index) {
    case kHangingValleyParamFilterFrequency:
      return String("Frequency");
    case kHangingValleyParamResonance:
      return String("Resonance");
    case kHangingValleyParamValleySize:
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

const String HangingValleyAudioProcessor::getParameterText(int index) {
  return String::empty;
}

//==============================================================================
void HangingValleyAudioProcessor::resetLastIOData() {
  for(int i = 0; i < 2; i++) {
    lastInput1[i] = 0.0f;
    lastInput2[i] = 0.0f;
    lastInput3[i] = 0.0f;
    lastOutput1[i] = 0.0f;
    lastOutput2[i] = 0.0f;
  }
}

void HangingValleyAudioProcessor::recalculateCoefficients(const double sampleRate, const float frequency, const float resonance) {
  const float hiCoeffConstant = (float)tan(M_PI * frequency / sampleRate);
  hiCoeffA1 = 1.0f / ((1.0f + resonance * hiCoeffConstant) + (hiCoeffConstant * hiCoeffConstant));
  hiCoeffA2 = -2.0f * hiCoeffA1;
  hiCoeffB1 = 2.0f * hiCoeffA1 * ((hiCoeffConstant * hiCoeffConstant) - 1.0f);
  hiCoeffB2 = hiCoeffA1 * (1.0f - (resonance * hiCoeffConstant) + (hiCoeffConstant * hiCoeffConstant));

  const float loCoeffConstant = (float)(1.0f / tan(frequency / sampleRate));
  loCoeffA1 = 1.0f / (1.0f + (resonance * loCoeffConstant) + (loCoeffConstant * loCoeffConstant));
  loCoeffA2 = 2.0f * loCoeffA1;
  loCoeffB1 = 2.0f * loCoeffA1 * (1.0f - (loCoeffConstant * loCoeffConstant));
  loCoeffB2 = loCoeffA1 * (1.0f - (resonance * loCoeffConstant) + (loCoeffConstant * loCoeffConstant));
}

void HangingValleyAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
}

void HangingValleyAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void HangingValleyAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  for(int channel = 0; channel < getNumInputChannels(); ++channel) {
    float *channelData = buffer.getSampleData(channel);

    // ..do something to the data...
  }

  // In case we have more outputs than inputs, we'll clear any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  for(int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
    buffer.clear(i, 0, buffer.getNumSamples());
  }
}

//==============================================================================
void HangingValleyAudioProcessor::getStateInformation(MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void HangingValleyAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new HangingValleyAudioProcessor();
}
