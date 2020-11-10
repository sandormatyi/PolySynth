#pragma once
#include <Audio.h>

extern const AudioSynthWavetable::sample_data organ_samples[4];
const uint8_t organ_ranges[] = {
    56,
    68,
    80,
    127,
};

const AudioSynthWavetable::instrument_data organ = {4, organ_ranges, organ_samples};

extern const uint32_t sample_0_organ_OrgueC2[2560];

extern const uint32_t sample_1_organ_OrgueC3[1536];

extern const uint32_t sample_2_organ_OrgueC4[1536];

extern const uint32_t sample_3_organ_OrgueC6[1408];
