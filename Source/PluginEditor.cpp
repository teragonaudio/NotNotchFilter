/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PluginEditor::PluginEditor (AudioProcessor *owner, teragon::PluginParameterSet &p, teragon::ResourceCache *r)
    : AudioProcessorEditor(owner),
      parameters(p),
      resources(r)
{
    addAndMakeVisible (frequencyKnob = new teragon::ImageKnob (parameters["frequency"],
                                                               resources->get("large_knob")));
    frequencyKnob->setName ("frequency knob");

    addAndMakeVisible (frequencyKnob2 = new teragon::ImageKnob (parameters["frequency"],
                                                                resources->get("large_knob")));
    frequencyKnob2->setName ("frequency knob");

    addAndMakeVisible (frequencyKnob3 = new teragon::ImageKnob (parameters["frequency"],
                                                                resources->get("large_knob")));
    frequencyKnob3->setName ("frequency knob");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (403, 190);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PluginEditor::~PluginEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    frequencyKnob = nullptr;
    frequencyKnob2 = nullptr;
    frequencyKnob3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PluginEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff3f3f3f));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PluginEditor::resized()
{
    frequencyKnob->setBounds (16, 16, 113, 113);
    frequencyKnob2->setBounds (145, 16, 113, 113);
    frequencyKnob3->setBounds (274, 16, 113, 113);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PluginEditor" componentName=""
                 parentClasses="public AudioProcessorEditor" constructorParams="AudioProcessor *owner, teragon::PluginParameterSet &amp;p, teragon::ResourceCache *r"
                 variableInitialisers="AudioProcessorEditor(owner),&#10;parameters(p),&#10;resources(r)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="403" initialHeight="190">
  <BACKGROUND backgroundColour="ff3f3f3f"/>
  <GENERICCOMPONENT name="frequency knob" id="cbc3f5c5fff9e09b" memberName="frequencyKnob"
                    virtualName="teragon::ImageKnob" explicitFocusOrder="0" pos="16 16 113 113"
                    class="teragon::ImageKnob" params="parameters[&quot;frequency&quot;],&#10;resources-&gt;get(&quot;large_knob&quot;)"/>
  <GENERICCOMPONENT name="frequency knob" id="13c569138892eb13" memberName="frequencyKnob2"
                    virtualName="teragon::ImageKnob" explicitFocusOrder="0" pos="145 16 113 113"
                    class="teragon::ImageKnob" params="parameters[&quot;frequency&quot;],&#10;resources-&gt;get(&quot;large_knob&quot;)"/>
  <GENERICCOMPONENT name="frequency knob" id="1f8ba387e5cceed1" memberName="frequencyKnob3"
                    virtualName="teragon::ImageKnob" explicitFocusOrder="0" pos="274 16 113 113"
                    class="teragon::ImageKnob" params="parameters[&quot;frequency&quot;],&#10;resources-&gt;get(&quot;large_knob&quot;)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
