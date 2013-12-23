#ifndef __PLUGINPROCESSOR_H_A4C75B3B__
#define __PLUGINPROCESSOR_H_A4C75B3B__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginEditor.h"
#include "PluginParameters.h"
#include "Resources.h"

typedef enum {
  kParamFilterFrequency,
  kParamResonance,
  kParamValleySize,
  kParamNumParams
} kParams;

static const float kFrequencyMin = 400.0f;
static const float kFrequencyDefault = kFrequencyMin;
static const float kFrequencyMax = 1400.0f;
static const float kResonanceMin = 0.1f;
static const float kResonanceDefault = 1.0f;
static const float kResonanceMax = sqrtf(2.0);
static const float kValleySizeMin = 0.1f;
static const float kValleySizeMax = 20000.0f;
static const float kValleySizeDefault = kValleySizeMin;

#define PARAM_TEXT_NUM_DECIMAL_PLACES 2

using namespace teragon;

class NotNotchFilterAudioProcessor : public AudioProcessor, public PluginParameterObserver {
public:
    NotNotchFilterAudioProcessor();
    ~NotNotchFilterAudioProcessor() {}

    // Playback
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void releaseResources() {}
    void reset();
    void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    // Editor
    AudioProcessorEditor *createEditor() { return new PluginEditor(this, parameters, Resources::getCache()); }
    bool hasEditor() const { return true; };

    // Parameter methods
    int getNumParameters() { return parameters.size(); }
    float getParameter(int index) { return (float)parameters[index]->getScaledValue(); }
    void setParameter(int index, float newValue) { parameters.setScaled(index, newValue); }
    const String getParameterName(int index) { return parameters[index]->getName().c_str(); }
    const String getParameterText(int index) { return parameters[index]->getDisplayText().c_str(); }

    // Plugin properties and basic information
    const String getName() const { return JucePlugin_Name; }
    const String getInputChannelName(int channelIndex) const { return String(channelIndex + 1); };
    const String getOutputChannelName(int channelIndex) const { return String(channelIndex + 1); };
    bool isInputChannelStereoPair(int index) const { return true; };
    bool isOutputChannelStereoPair(int index) const { return true; };
    bool acceptsMidi() const { return false; };
    bool producesMidi() const { return false; };
    bool silenceInProducesSilenceOut() const { return true; }
    double getTailLengthSeconds() const { return 0.0; }

    // Program support (not needed by this plugin)
    int getNumPrograms() { return 0; };
    int getCurrentProgram() { return 0; };
    void setCurrentProgram(int index) {};
    const String getProgramName(int index) { return String::empty; };
    void changeProgramName(int index, const String& newName) {};

    // State save/restore
    void getStateInformation(MemoryBlock& destData);
    void setStateInformation(const void *data, int sizeInBytes);

private:
    // ParameterSet and cached parameters
    ThreadsafePluginParameterSet parameters;

    float maxFilterFrequency;
    float loFrequency;
    float hiFrequency;

    float hiLastInput1[2], hiLastInput2[2], hiLastInput3[2];
    float loLastInput1[2], loLastInput2[2], loLastInput3[2];
    float hiLastOutput1[2], hiLastOutput2[2];
    float loLastOutput1[2], loLastOutput2[2];

    float loCoeffA1, loCoeffA2;
    float loCoeffB1, loCoeffB2;
    float hiCoeffA1, hiCoeffA2;
    float hiCoeffB1, hiCoeffB2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NotNotchFilterAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_A4C75B3B__
