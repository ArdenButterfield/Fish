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

class Downsampler
{
public:
    Downsampler(const unsigned int ratio_) : ratio(ratio_)
    {
        step = 0;
    }
    
    // ~Downsampler() {};
    
    int downsample(float* input, float* output, int input_length)
    {
        int output_index = 0;
        for (int i = 0; i < input_length; ++i) {
            if (step == 0) {
                output[output_index] = input[i];
                output_index++;
            }
            step++;
            step %= ratio;
        }
        return output_index;
    }
    
    void clear()
    {
        step = 0;
    }
        
private:
    const unsigned int ratio;
    unsigned int step;
    
};
