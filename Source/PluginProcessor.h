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

/*
 The Plugin Processor controls the processing of samples, and interfaces with
 the Plugin Editor which manages the graphical user interface.
 */

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include <vector>
#include <atomic>
#include <iostream>
#include <algorithm>

#include "MP3Processor.h"

#include "filterCalc/FilterCalc.h"
#include "stk-filters/BiQuad.h"

#include "upsampler.h"
#include "downsampler.h"

/*
 Set DOWNSAMPLE to zero to turn off down/upsampling.
 */
#define DOWNSAMPLE 1


class FishAudioProcessor  : public juce::AudioProcessor,
                            public juce::AudioProcessorValueTreeState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    FishAudioProcessor();
    ~FishAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorValueTreeState& getValueTreeState();
    void parameterChanged (const juce::String &parameterID, float newValue) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    static juce::String fishParameterID;

private:
    
#if DOWNSAMPLE
    const unsigned int DOWNSAMPLE_RATIO = 4;
    
    Upsampler upsampler_l = Upsampler(DOWNSAMPLE_RATIO);
    Upsampler upsampler_r = Upsampler(DOWNSAMPLE_RATIO);
    Downsampler downsampler_l = Downsampler(DOWNSAMPLE_RATIO);
    Downsampler downsampler_r = Downsampler(DOWNSAMPLE_RATIO);
    
    std::vector<float> downsampled_l;
    std::vector<float> downsampled_r;
    
    stk::BiQuad filter_post_L, filter_post_R;
#endif
    
    void updateParameters();
    bool params_need_updating;
    
    juce::AudioProcessorValueTreeState parameters;
    std::atomic<float> fishUserParameter {0.0f};

    MP3Processor mp3Processor;
    std::vector<float> inputStereoBuffer;
    std::vector<float> outputStereoBuffer;
    size_t inputStereoPos = 0;
    bool readyToOutput = false;
    int sample_counter;
    int mp3_bitrate;
    const int DEFAULT_MODE; // STEREO
    float lsamp, rsamp, prevlsamp, prevrsamp;
    
    stk::BiQuad filter_lo_L, filter_lo_R;
    
    float fs;
    const float Q = 0.71; // Decently flat passband, without a noticeable spike
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FishAudioProcessor)    
};
