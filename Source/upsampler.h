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

class Upsampler
{
public:
    Upsampler(const unsigned int ratio_) : ratio(ratio_)
    {
        step = 0;
        prev_sample = 0.f;
        curr_sample = 0.f;
    }
    
    // ~Upsampler() {};
    
    int upsample(float* input, float* output, int input_length, int output_length)
    {
#if 0
        if (output_length / ratio != input_length) {
            // Failure
            std::cout << "Failure\n";
            return -1;
        }
#endif
        int out_i, in_i;
        in_i = 0;
        for (out_i = 0; out_i < output_length; ++out_i) {
            if (step == 0) {
                prev_sample = curr_sample;
                curr_sample = input[in_i];
                in_i++;
            }
            output[out_i] = lerp(prev_sample, curr_sample, ((float)step) / ((float)ratio));
            
            step++;
            step %= ratio;
        }
        return out_i;
    }
    
    void clear() {
        step = 0;
        prev_sample = 0.f;
        curr_sample = 0.f;
    }
    
private:
    float lerp(const float prev, const float next, const float amount)
    {
        return ((next - prev) * amount) + prev;
    }
    
    const unsigned int ratio;
    float prev_sample, curr_sample;
    unsigned int step;
    
};
