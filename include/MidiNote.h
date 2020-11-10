#pragma once

#include <inttypes.h>
#include <math.h>

struct MidiNote 
{
  uint8_t channel;
  uint8_t note;
  uint8_t velocity;

  static double getFrequency(int midiNote) 
  {
      return 440.0 * powf(2.0, (midiNote - 69) * 0.08333333333333333);
  }
};