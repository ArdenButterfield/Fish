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

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>

#include <BinaryData.h>

#include <array>

#include "PluginProcessor.h"
#include "LookAndFeel.h"

//==============================================================================
/*
 The GUI of this plugin is very simple: there is a spinning fish, and dragging
 verically wipes between a normal and an inverted version of the fish. Dragging
 up results in more distoriton and vice versa. The fish also spins faster the
 higher up the slider is.
*/
class FishAudioProcessorEditor  :
        public juce::AudioProcessorEditor,
        public juce::Timer,
        public juce::Slider::Listener
{
public:
    FishAudioProcessorEditor (FishAudioProcessor&);
    ~FishAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    FishAudioProcessor& audioProcessor;
    
    FishLookAndFeel fishLookAndFeel;

    juce::Slider fishSlider {
        juce::Slider::LinearBarVertical,
        juce::Slider::NoTextBox
    };
    
    juce::AudioProcessorValueTreeState::SliderAttachment fishAttachment {
        audioProcessor.getValueTreeState(),
        "fish",
        fishSlider
    };
    bool dirty_timer;
    void setTimer(double slider_val);
    
    static const int NUM_FISH_FRAMES = 35;
    int frame_step;
    
    // The fish image was originally a GIF, it is here separated as PNGs to give greater control
    // over frame rate and skipping frames.
    const std::array<juce::Image, NUM_FISH_FRAMES>fish_frames = {
        juce::ImageCache::getFromMemory(BinaryData::fishunique0_png, BinaryData::fishunique0_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique1_png, BinaryData::fishunique1_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique2_png, BinaryData::fishunique2_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique3_png, BinaryData::fishunique3_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique4_png, BinaryData::fishunique4_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique5_png, BinaryData::fishunique5_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique6_png, BinaryData::fishunique6_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique7_png, BinaryData::fishunique7_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique8_png, BinaryData::fishunique8_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique9_png, BinaryData::fishunique9_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique10_png, BinaryData::fishunique10_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique11_png, BinaryData::fishunique11_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique12_png, BinaryData::fishunique12_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique13_png, BinaryData::fishunique13_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique14_png, BinaryData::fishunique14_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique15_png, BinaryData::fishunique15_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique16_png, BinaryData::fishunique16_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique17_png, BinaryData::fishunique17_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique18_png, BinaryData::fishunique18_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique19_png, BinaryData::fishunique19_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique20_png, BinaryData::fishunique20_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique21_png, BinaryData::fishunique21_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique22_png, BinaryData::fishunique22_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique23_png, BinaryData::fishunique23_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique24_png, BinaryData::fishunique24_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique25_png, BinaryData::fishunique25_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique26_png, BinaryData::fishunique26_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique27_png, BinaryData::fishunique27_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique28_png, BinaryData::fishunique28_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique29_png, BinaryData::fishunique29_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique30_png, BinaryData::fishunique30_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique31_png, BinaryData::fishunique31_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique32_png, BinaryData::fishunique32_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique33_png, BinaryData::fishunique33_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishunique34_png, BinaryData::fishunique34_pngSize)
    };
    
    const std::array<juce::Image, NUM_FISH_FRAMES>ghost_frames = {
        juce::ImageCache::getFromMemory(BinaryData::fishghost0_png, BinaryData::fishghost0_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost1_png, BinaryData::fishghost1_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost2_png, BinaryData::fishghost2_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost3_png, BinaryData::fishghost3_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost4_png, BinaryData::fishghost4_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost5_png, BinaryData::fishghost5_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost6_png, BinaryData::fishghost6_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost7_png, BinaryData::fishghost7_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost8_png, BinaryData::fishghost8_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost9_png, BinaryData::fishghost9_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost10_png, BinaryData::fishghost10_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost11_png, BinaryData::fishghost11_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost12_png, BinaryData::fishghost12_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost13_png, BinaryData::fishghost13_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost14_png, BinaryData::fishghost14_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost15_png, BinaryData::fishghost15_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost16_png, BinaryData::fishghost16_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost17_png, BinaryData::fishghost17_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost18_png, BinaryData::fishghost18_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost19_png, BinaryData::fishghost19_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost20_png, BinaryData::fishghost20_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost21_png, BinaryData::fishghost21_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost22_png, BinaryData::fishghost22_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost23_png, BinaryData::fishghost23_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost24_png, BinaryData::fishghost24_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost25_png, BinaryData::fishghost25_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost26_png, BinaryData::fishghost26_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost27_png, BinaryData::fishghost27_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost28_png, BinaryData::fishghost28_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost29_png, BinaryData::fishghost29_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost30_png, BinaryData::fishghost30_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost31_png, BinaryData::fishghost31_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost32_png, BinaryData::fishghost32_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost33_png, BinaryData::fishghost33_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::fishghost34_png, BinaryData::fishghost34_pngSize)
    };

    int current_frame;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FishAudioProcessorEditor)
};
