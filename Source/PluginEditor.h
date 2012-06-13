#ifndef __PLUGINEDITOR_H_A6134158__
#define __PLUGINEDITOR_H_A6134158__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class NotNotchFilterAudioProcessorEditor  : public AudioProcessorEditor {
public:

  NotNotchFilterAudioProcessorEditor(NotNotchFilterAudioProcessor *ownerFilter);

  ~NotNotchFilterAudioProcessorEditor();

  //==============================================================================
  // This is just a standard Juce paint method...
  void paint(Graphics& g);

};


#endif  // __PLUGINEDITOR_H_A6134158__
