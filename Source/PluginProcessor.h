#ifndef __PLUGINPROCESSOR_H_A4C75B3B__
#define __PLUGINPROCESSOR_H_A4C75B3B__

#include "../JuceLibraryCode/JuceHeader.h"

typedef enum {
  kHangingValleyParamFilterFrequency,
  kHangingValleyParamResonance,
  kHangingValleyParamValleySize,
  kHangingValleyParamNumParams
} kHangingValleyParams;

static const float kHangingValleyFrequencyMin = 20.0f;
static const float kHangingValleyFrequencyMax = 20000.0f;
static const float kHangingValleyResonanceMin = 0.1f;
static const float kHangingValleyResonanceDefault = 1.0f;
static const float kHangingValleyResonanceMax = sqrtf(2.0);

#define PARAM_TEXT_NUM_DECIMAL_PLACES 2

//==============================================================================
class HangingValleyAudioProcessor  : public AudioProcessor {
public:

  //==============================================================================
  HangingValleyAudioProcessor();

  ~HangingValleyAudioProcessor();

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock);

  void releaseResources();

  void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

  //==============================================================================
  AudioProcessorEditor *createEditor() { return NULL; }

  bool hasEditor() const { return false; };

  //==============================================================================
  const String getName() const;

  int getNumParameters();

  float getParameter(int index);

  void setParameter(int index, float newValue);

  const String getParameterName(int index);

  const String getParameterText(int index);

  const String getInputChannelName(int channelIndex) const { return String(channelIndex + 1); };

  const String getOutputChannelName(int channelIndex) const { return String(channelIndex + 1); };

  bool isInputChannelStereoPair(int index) const { return true; };

  bool isOutputChannelStereoPair(int index) const { return true; };

  bool acceptsMidi() const { return false; };

  bool producesMidi() const { return false; };

  //==============================================================================
  int getNumPrograms() { return 0; };

  int getCurrentProgram() { return 0; };

  void setCurrentProgram(int index) {};

  const String getProgramName(int index) { return String::empty; };

  void changeProgramName(int index, const String& newName) {};

  //==============================================================================
  void getStateInformation(MemoryBlock& destData);

  void setStateInformation(const void *data, int sizeInBytes);

private:
  void resetLastIOData();
  void recalculateCoefficients(const double sampleRate, const float frequency, const float resonance);
  void processHiFilter(float *channelData, const int channel, const int numSamples);
  void processLoFilter(float *channelData, const int channel, const int numSamples);

  float frequency;
  float resonance;
  float valleySize;

  float lastInput1[2], lastInput2[2], lastInput3[2];
  float lastOutput1[2], lastOutput2[2];

  float loCoeffA1, loCoeffA2;
  float loCoeffB1, loCoeffB2;
  float hiCoeffA1, hiCoeffA2;
  float hiCoeffB1, hiCoeffB2;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HangingValleyAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_A4C75B3B__
