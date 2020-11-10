#pragma once
#include <Audio.h>

extern const AudioSynthWavetable::sample_data analogSaw_samples[2];
const uint8_t analogSaw_ranges[] = {
    127,
    127,
};

const AudioSynthWavetable::instrument_data analogSaw = {2, analogSaw_ranges, analogSaw_samples};

extern const uint32_t sample_0_analogSaw_AnalogSaw1060L[44160];

extern const uint32_t sample_1_analogSaw_AnalogSaw1060R[44160];
