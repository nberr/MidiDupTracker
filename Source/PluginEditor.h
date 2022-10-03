/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MidiDupTrackerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MidiDupTrackerAudioProcessorEditor (MidiDupTrackerAudioProcessor&);
    ~MidiDupTrackerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void logMessage(juce::String message);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiDupTrackerAudioProcessor& audioProcessor;
    
    juce::TextEditor messageDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiDupTrackerAudioProcessorEditor)
};
