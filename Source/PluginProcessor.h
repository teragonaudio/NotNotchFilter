#ifndef __PLUGINPROCESSOR_H_A4C75B3B__
#define __PLUGINPROCESSOR_H_A4C75B3B__

#include "../JuceLibraryCode/JuceHeader.h"
#include "TeragonPluginBase.h"
#include "PluginParameters.h"
#include "PluginEditor.h"
#include "Resources.h"

using namespace teragon;

static const float kFrequencyMin = 400.0f;
static const float kFrequencyDefault = kFrequencyMin;
static const float kFrequencyMax = 1400.0f;
static const float kResonanceMin = 0.1f;
static const float kResonanceDefault = 1.0f;
static const float kResonanceMax = sqrtf(2.0);
static const float kValleySizeMin = 0.1f;
static const float kValleySizeMax = 20000.0f;
static const float kValleySizeDefault = kValleySizeMin;
static const float kMinimumNotchFrequency = 20.0f;

class NotNotchFilterAudioProcessor : public TeragonPluginBase, public ParameterObserver {
public:
    NotNotchFilterAudioProcessor();
    ~NotNotchFilterAudioProcessor() {}

    const String getName() const { return JucePlugin_Name; }
    AudioProcessorEditor *createEditor();

    // Playback
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void releaseResources();
    void reset();
    void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    // ParameterObserver methods
    bool isRealtimePriority() const { return true; }
    void onParameterUpdated(const Parameter *parameter);

private:
    void recalculateCoefficients(const double sampleRate);

private:
    // Cached parameters
    FrequencyParameter *frequency;
    FloatParameter *resonance;
    FloatParameter *valleySize;

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
