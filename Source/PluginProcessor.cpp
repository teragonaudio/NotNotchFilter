#include "PluginProcessor.h"
#include "PluginEditor.h"


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
  return 0;
}

float HangingValleyAudioProcessor::getParameter(int index) {
  return 0.0f;
}

void HangingValleyAudioProcessor::setParameter(int index, float newValue) {
}

const String HangingValleyAudioProcessor::getParameterName(int index) {
  return String::empty;
}

const String HangingValleyAudioProcessor::getParameterText(int index) {
  return String::empty;
}

const String HangingValleyAudioProcessor::getInputChannelName(int channelIndex) const {
  return String(channelIndex + 1);
}

const String HangingValleyAudioProcessor::getOutputChannelName(int channelIndex) const {
  return String(channelIndex + 1);
}

bool HangingValleyAudioProcessor::isInputChannelStereoPair(int index) const {
  return true;
}

bool HangingValleyAudioProcessor::isOutputChannelStereoPair(int index) const {
  return true;
}

bool HangingValleyAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
  return false;
#endif
}

bool HangingValleyAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
  return false;
#endif
}

int HangingValleyAudioProcessor::getNumPrograms() {
  return 0;
}

int HangingValleyAudioProcessor::getCurrentProgram() {
  return 0;
}

void HangingValleyAudioProcessor::setCurrentProgram(int index) {
}

const String HangingValleyAudioProcessor::getProgramName(int index) {
  return String::empty;
}

void HangingValleyAudioProcessor::changeProgramName(int index, const String& newName) {
}

//==============================================================================
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
bool HangingValleyAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor *HangingValleyAudioProcessor::createEditor() {
  return new HangingValleyAudioProcessorEditor (this);
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
