/*
Copyright (C) 2023  Arden Butterfield

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FishAudioProcessor::FishAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
parameters(*this,
           nullptr,
           juce::Identifier("Fish"),
           std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"fish", 1}, "Fish", juce::NormalisableRange<float>(0.0,1.0),0.0)),
DEFAULT_MODE(0)
{
    parameters.addParameterListener("fish", this);
    sample_counter = 0;
}

FishAudioProcessor::~FishAudioProcessor()
{
    parameters.removeParameterListener("fish", this);
}

juce::AudioProcessorValueTreeState& FishAudioProcessor::getValueTreeState()
{
    return parameters;
}

void FishAudioProcessor::parameterChanged (const juce::String &parameterID, float newValue)
{
    if (parameterID == "fish") {
        fishUserParameter = newValue;
    }
    params_need_updating = true;
}


//==============================================================================
const juce::String FishAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FishAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FishAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FishAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FishAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FishAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FishAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FishAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FishAudioProcessor::getProgramName (int index)
{
    return {};
}

void FishAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FishAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mp3Processor.init((const int)sampleRate, samplesPerBlock);
    fs = sampleRate;
    updateParameters();
    mp3Processor.initialFlush();
    lsamp = 0;
    rsamp = 0;
    prevlsamp = 0;
    prevrsamp = 0;
    
#if DOWNSAMPLE
    // Bigger than it needs to be (really it just needs to be samplesPerBlock / downsample rate + 1.
    // Just as safety i guess.
    downsampled_l.resize(samplesPerBlock);
    downsampled_r.resize(samplesPerBlock);
    
    downsampler_l.clear();
    downsampler_r.clear();
    upsampler_l.clear();
    upsampler_r.clear();
    
    float coeffs[5];
    // We can lowpass safely at 5k, since with the downsampling and MP3 compression
    // there isn't anything going on above that. This gets rid of any pesky artifacts
    // from the upsampling.
    FilterCalc::calcCoeffsLPF(coeffs, 5000, 0.5, fs);
    filter_post_L.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    filter_post_R.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
#endif

}

void FishAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FishAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FishAudioProcessor::updateParameters()
{
#if DOWNSAMPLE
    // range from 4,000 to 1,000
    float cutoff_freq = std::pow(4.0, 1.0 - fishUserParameter) * 1000.0;
#else
    // range from 10,000 to 1,000
    float cutoff_freq = std::pow(10.0, 1.0 - fishUserParameter) * 1000.0;
#endif
    float coeffs[5];
    FilterCalc::calcCoeffsLPF(coeffs, cutoff_freq, Q, fs);
    filter_lo_L.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    filter_lo_R.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    
    mp3Processor.changeBitrate(fishUserParameter);
    params_need_updating = false;
}

void FishAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (params_need_updating) {
        updateParameters();
    }

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float* channelData_l;
    float* channelData_r;
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    const int num_block_samples = buffer.getNumSamples();
    // TODO: resize MP3 stuff if num_block_samples goes over the promised max block size
    
    // Prevents clipping in LAME if the signal is close to clipping
    buffer.applyGain(0.5f);
    
    // If we're have mono input, Fish currently just copies it into the L and R channels
    // and treats it as joint stereo, as we would with stereo input.
    if (totalNumInputChannels == 2) {
        channelData_l = buffer.getWritePointer(0);
        channelData_r = buffer.getWritePointer(1);
        for (int i = 0; i < num_block_samples; ++i) {
            channelData_l[i] = filter_lo_L.tick(channelData_l[i]);
        }
        
        for (int i = 0; i < num_block_samples; ++i) {
            channelData_r[i] = filter_lo_R.tick(channelData_r[i]);
        }
        
    } else if (totalNumInputChannels == 1) {
        channelData_l = buffer.getWritePointer(0);
        channelData_r = nullptr;
        for (int i = 0; i < num_block_samples; ++i) {
            channelData_l[i] = filter_lo_L.tick(channelData_l[i]);
        }
    } else {
        std::cout << "Only works in mono or stereo.\n";
        buffer.applyGain(2.0f);
        return;
    }

#if DOWNSAMPLE
    unsigned int l_frames, r_frames;
    l_frames = downsampler_l.downsample(channelData_l,
                                        downsampled_l.data(),
                                        num_block_samples);
    
    if (totalNumInputChannels == 2) {
        r_frames = downsampler_r.downsample(channelData_r,
                                            downsampled_r.data(),
                                            num_block_samples);
    } else {
        r_frames = downsampler_r.downsample(channelData_l,
                                            downsampled_r.data(),
                                            num_block_samples);
    }
    
    // l_frames and r_frames should be equal

    // Encode and decode, stores to buffer inside mp3Processor.
    mp3Processor.addNextInput(downsampled_l.data(), downsampled_r.data(), l_frames);
    
    // Copy out from buffer.
    mp3Processor.copy_output(downsampled_l.data(), downsampled_r.data(), l_frames);
    
    upsampler_l.upsample(downsampled_l.data(),
                         channelData_l,
                         l_frames,
                         num_block_samples);
    
    // Lowpass again to remove high-freq noise from linear interpolation.
    for (int i = 0; i < num_block_samples; ++i) {
        channelData_l[i] = filter_post_L.tick(channelData_l[i]);
    }
    
    if (totalNumInputChannels == 2) {
        upsampler_r.upsample(downsampled_r.data(),
                             channelData_r,
                             r_frames,
                             num_block_samples);
        
        for (int i = 0; i < num_block_samples; ++i) {
            channelData_r[i] = filter_post_R.tick(channelData_r[i]);
        }
    }
#else
    mp3Processor.addNextInput(channelData_l, channelData_r, num_block_samples);
    
    mp3Processor.copy_output(channelData_l, channelData_r, num_block_samples);
#endif
    
    buffer.applyGain(2.0f);
}

//==============================================================================
bool FishAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FishAudioProcessor::createEditor()
{
    return new FishAudioProcessorEditor (*this);
}

//==============================================================================
void FishAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FishAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(parameters.state.getType())) {
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FishAudioProcessor();
}
