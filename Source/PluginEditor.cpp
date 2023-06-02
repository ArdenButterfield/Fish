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
#include <chrono>

//==============================================================================
FishAudioProcessorEditor::FishAudioProcessorEditor (FishAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), lastTime(-1.0), spinAlong(0.0)
{
    setLookAndFeel(&fishLookAndFeel);
    
    addAndMakeVisible(fishSlider);
    setSize (320, 320);

    startTimerHz(60);
}

FishAudioProcessorEditor::~FishAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

template<std::floating_point F>
inline auto interp(F low, F high, F along) -> F {
    return (high - low) * along + low;
}

void FishAudioProcessorEditor::timerCallback() {
    auto delta = getDeltaTime(juce::Time::getMillisecondCounterHiRes());

    auto sliderAlong = fishSlider.getValue();

    spinAlong = fmod(spinAlong + delta * interp(MIN_ROTATIONS_PER_SECOND, MAX_ROTATIONS_PER_SECOND, sliderAlong), 1.0);

    repaint();
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
}

auto FishAudioProcessorEditor::getDeltaTime(double now) -> double {
    if (lastTime == -1.0) {
        lastTime = now;
        return 0.0;
    }

    auto deltaMillis = now - lastTime;
    lastTime = now;

    return deltaMillis / (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(1)).count();
}

//==============================================================================
void FishAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto sliderAlong = fishSlider.getValue();
    auto currentFrame = (int)floor((double)fish_frames.size() * spinAlong);

    /*
     There are two fish animations, the normal one and the inverted "ghost" image. Both are big enough
     to fill the frame, we crop them so that the normal one takes up the space from the slider thumb height
     to the top of the screen and the ghost one takes up the space from the bottom of the screen to the
     slider thumb height. That way, as the slider is dragged up (more distortion) we see more of the ghost
     image.
     */

    g.drawImage(fish_frames[currentFrame],
                0,
                0,
                getWidth(),
                std::ceil(getHeight() - sliderAlong * getHeight()),
                0,
                0,
                640,
                std::ceil(640 - sliderAlong * 640));
    g.drawImage(ghost_frames[currentFrame],
                0,
                std::ceil(getHeight() - sliderAlong * getHeight()),
                getWidth(),
                std::ceil(getHeight() * sliderAlong + 1),
                0,
                std::ceil(640 - sliderAlong * 640),
                640,
                std::ceil(sliderAlong * 640));
}

void FishAudioProcessorEditor::resized()
{
    fishSlider.setBounds(getLocalBounds());
}
