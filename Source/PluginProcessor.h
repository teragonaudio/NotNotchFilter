#ifndef __PLUGINPROCESSOR_H_A4C75B3B__
#define __PLUGINPROCESSOR_H_A4C75B3B__

#include "../JuceLibraryCode/JuceHeader.h"

typedef enum {
  kNotNotchFilterParamFilterFrequency,
  kNotNotchFilterParamResonance,
  kNotNotchFilterParamValleySize,
  kNotNotchFilterParamNumParams
} kNotNotchFilterParams;

static const float kNotNotchFilterFrequencyMin = 400.0f;
static const float kNotNotchFilterFrequencyDefault = kNotNotchFilterFrequencyMin;
static const float kNotNotchFilterFrequencyMax = 1400.0f;
static const float kNotNotchFilterResonanceMin = 0.1f;
static const float kNotNotchFilterResonanceDefault = 1.0f;
static const float kNotNotchFilterResonanceMax = sqrtf(2.0);
static const float kNotNotchFilterValleySizeMin = 0.1f;
static const float kNotNotchFilterValleySizeMax = 20000.0f;
static const float kNotNotchFilterValleySizeDefault = 0.1f;

#define PARAM_TEXT_NUM_DECIMAL_PLACES 2

//==============================================================================
class NotNotchFilterAudioProcessor  : public AudioProcessor {
public:

  //==============================================================================
  NotNotchFilterAudioProcessor();

  ~NotNotchFilterAudioProcessor();

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

  float getMaxFilterFrequency() const;

  float getParameter(int index);

  void setParameter(int index, float newValue);

  const String getParameterName(int index);

  const String getParameterNameForStorage(int index);

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
  void recalculateCoefficients(const double sampleRate, const float filterFrequency, const float filterResonance);
  void processHiFilter(float *channelData, const int channel, const int numSamples);
  void processLoFilter(float *channelData, const int channel, const int numSamples);

  float baseFrequency;
  float loFrequency;
  float hiFrequency;
  float resonance;
  float valleySize;

  float hiLastInput1[2], hiLastInput2[2], hiLastInput3[2];
  float loLastInput1[2], loLastInput2[2], loLastInput3[2];
  float hiLastOutput1[2], hiLastOutput2[2];
  float loLastOutput1[2], loLastOutput2[2];

  float loCoeffA1, loCoeffA2;
  float loCoeffB1, loCoeffB2;
  float hiCoeffA1, hiCoeffA2;
  float hiCoeffB1, hiCoeffB2;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NotNotchFilterAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_A4C75B3B__
