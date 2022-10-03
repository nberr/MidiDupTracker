/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiDupTrackerAudioProcessorEditor::MidiDupTrackerAudioProcessorEditor (MidiDupTrackerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    messageDisplay.setMultiLine (true);
    messageDisplay.setReturnKeyStartsNewLine (true);
    messageDisplay.setReadOnly (true);
    messageDisplay.setScrollbarsShown (true);
    messageDisplay.setCaretVisible (false);
    messageDisplay.setPopupMenuEnabled (true);
    messageDisplay.setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x32ffffff));
    messageDisplay.setColour (juce::TextEditor::outlineColourId, juce::Colour (0x1c000000));
    messageDisplay.setColour (juce::TextEditor::shadowColourId, juce::Colour (0x16000000));
    
    addAndMakeVisible (messageDisplay);
}

MidiDupTrackerAudioProcessorEditor::~MidiDupTrackerAudioProcessorEditor()
{
}

//==============================================================================
void MidiDupTrackerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MidiDupTrackerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    messageDisplay.setBounds(getLocalBounds());
}

//==============================================================================
void MidiDupTrackerAudioProcessorEditor::logMessage(juce::String message)
{
    messageDisplay.moveCaretToEnd();
    messageDisplay.insertTextAtCaret (message + juce::newLine);
}
