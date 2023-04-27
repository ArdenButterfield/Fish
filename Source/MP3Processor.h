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

#include "RingBuffer.h"

#include <vector>
#include <array>
#include <lame.h>
#include <stdlib.h>
#include <memory>
#include <limits>

class MP3Processor {
public:
    MP3Processor();
    ~MP3Processor();
    bool init(const int sampleRate, const int maxSamplesPerBlock);
    void deInit();
    void addNextInput(float *left_input, float* right_input, const int num_block_samples);
    bool hasReadyOutput();
    void changeBitrate(float fish);
    bool copy_output(float* left, float* right, const int num_block_samples);
    int samples_in_output_queue();
    bool initialFlush();
    
private:
    bool bInitialized = false;
    void *lame_enc_handler = nullptr;
    void *lame_dec_handler = nullptr;
    std::vector<unsigned char> mp3Buffer;
    std::array<short, 20000> decodedLeftChannel = {0};
    std::array<short, 20000> decodedRightChannel = {0};
    std::vector<float> readBuf;
    int max_samples_per_block;
    std::unique_ptr<QueueBuffer<float>> outputBufferL;
    std::unique_ptr<QueueBuffer<float>> outputBufferR;
    
    int input_buf_size;
    int mp3_buf_size;
    
    // Values from the LAME documentation
    const std::array<int, 9> allowed_samplerates = {
        8000,
        11025,
        12000,
        16000,
        22050,
        24000,
        32000,
        44100,
        48000
    };
    
};
