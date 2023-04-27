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
FishAudioProcessorEditor::FishAudioProcessorEditor (FishAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel(&fishLookAndFeel);
    
    addAndMakeVisible(fishSlider);
    setSize (320, 320);
    current_frame = 0;

    dirty_timer = true;
    timerCallback();
}

FishAudioProcessorEditor::~FishAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void FishAudioProcessorEditor::timerCallback()
{
    /*
     The timer sets the rate at which frames are changed in the fish animation.
     */
    if (dirty_timer) {
        setTimer(fishSlider.getValue());
        dirty_timer = false;
    }
    current_frame += frame_step;
    current_frame %= NUM_FISH_FRAMES;
    repaint();
}

void FishAudioProcessorEditor::setTimer(double slider_val)
{
    /*
     At higher slider values, we want the fish to spin faster. This is accomplished by a
     combination of setting the rate at which we cycle through fish frames, and (to achieve
     higher rotation speeds without needing very high frame-rates) skipping frames.
     */
    double hz = slider_val * 200 + 10;
    frame_step = 1;
    while (hz > 30) {
        frame_step++;
        hz /= 2;
    }
    startTimerHz(hz);
}


void FishAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    /*
     When the slider moves, we want to change the rate at which the fish spins. However, we don't
     want to reset the timer immediately here, because the slider value changed function is
     called many times in quick succession during a drag, and resetting the timer again and again
     before it has time to callback will cause the fish animation to behave unusually during the
     drag.
     
     Instead, we mark the timer as dirty, and do not change the spin rate until the next time the
     timer callback is reached.
     */
    dirty_timer = true;
}

//==============================================================================
void FishAudioProcessorEditor::paint (juce::Graphics& g)
{
    /*
     There are two fish animations, the normal one and the inverted "ghost" image. Both are big enough
     to fill the frame, we crop them so that the normal one takes up the space from the slider thumb height
     to the top of the screen and the ghost one takes up the space from the bottom of the screen to the
     slider thumb height. That way, as the slider is dragged up (more distortion) we see more of the ghost
     image.
     */
    float frac = fishSlider.getValue();
    
    g.drawImage(fish_frames[current_frame],
                0,
                0,
                getWidth(),
                std::ceil(getHeight() - frac * getHeight()),
                0,
                0,
                640,
                std::ceil(640 - frac * 640));
    g.drawImage(ghost_frames[current_frame],
                0,
                std::ceil(getHeight() - frac * getHeight()),
                getWidth(),
                std::ceil(getHeight() * frac + 1),
                0,
                std::ceil(640 - frac * 640),
                640,
                std::ceil(frac * 640));
}

void FishAudioProcessorEditor::resized()
{
    fishSlider.setBounds(getLocalBounds());
}
