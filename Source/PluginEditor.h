#ifndef __PLUGINEDITOR_H_A6134158__
#define __PLUGINEDITOR_H_A6134158__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class HangingValleyAudioProcessorEditor  : public AudioProcessorEditor {
public:

  HangingValleyAudioProcessorEditor(HangingValleyAudioProcessor *ownerFilter);

  ~HangingValleyAudioProcessorEditor();

  //==============================================================================
  // This is just a standard Juce paint method...
  void paint(Graphics& g);

};


#endif  // __PLUGINEDITOR_H_A6134158__
