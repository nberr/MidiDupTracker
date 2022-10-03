/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiDupTrackerAudioProcessor::MidiDupTrackerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

MidiDupTrackerAudioProcessor::~MidiDupTrackerAudioProcessor()
{
}

//==============================================================================
const juce::String MidiDupTrackerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiDupTrackerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiDupTrackerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MidiDupTrackerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MidiDupTrackerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiDupTrackerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiDupTrackerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiDupTrackerAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String MidiDupTrackerAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void MidiDupTrackerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void MidiDupTrackerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    for (size_t note_idx = 0; note_idx < TOTAL_NUM_MIDI; ++note_idx) {
        notes[note_idx] = 0;
    }
}

void MidiDupTrackerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiDupTrackerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MidiDupTrackerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(buffer);
    
    if (midiMessages.isEmpty()) {
        return;
    }
    
    auto editor = static_cast<MidiDupTrackerAudioProcessorEditor *>(getActiveEditor());
    
    for (auto metadata : midiMessages) {
        
        auto msg = metadata.getMessage();
        int note = msg.getNoteNumber();
        size_t note_idx = static_cast<size_t>(note);
        
        if (msg.isNoteOn()) {
            
            if (notes[note_idx] > 0) {
                /* note already on */
                editor->logMessage("Note " + juce::String(note) + " on " + juce::String(notes[note_idx]) + " time(s)");
                
            }
            else {
                /* first time turning note on */
                editor->logMessage("Note " + juce::String(note) + " on");
            }
            
            notes[note_idx]++;
        }
        else if (msg.isNoteOff()) {
            
            if (notes[note_idx] <= 0) {
                /* note already off */
                editor->logMessage("Note " + juce::String(note) + " off " + juce::String(notes[note_idx]) + " time(s)");
                
            }
            else {
                /* first time turning note on */
                editor->logMessage("Note " + juce::String(note) + " off");
            }
            
            notes[note_idx]--;
        }
    }
}

//==============================================================================
bool MidiDupTrackerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiDupTrackerAudioProcessor::createEditor()
{
    return new MidiDupTrackerAudioProcessorEditor (*this);
}

//==============================================================================
void MidiDupTrackerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void MidiDupTrackerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiDupTrackerAudioProcessor();
}
