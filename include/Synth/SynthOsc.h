#pragma once

#include "Wavetables/organ_samples.h"
#include <Audio.h>
#include <memory>

enum class OscType {
  None = -1,
  Sine = WAVEFORM_SINE,
  Saw = WAVEFORM_SAWTOOTH,
  Square = WAVEFORM_SQUARE,
  Triangle = WAVEFORM_TRIANGLE,
  Arbitrary = WAVEFORM_ARBITRARY,
  Pulse = WAVEFORM_PULSE,
  ReverseSaw = WAVEFORM_SAWTOOTH_REVERSE,
  SnH = WAVEFORM_SAMPLE_HOLD,
  TriangleVar = WAVEFORM_TRIANGLE_VARIABLE,
  PWM,
  Wavetable,

  NumTypes
};

class SynthOsc
{
public:
  void init();

  AudioStream& getOutput();
  AudioSynthWaveform& getLFO();
  void setOscType(OscType type);

  void noteOn(int noteNumber, int velocity);

private:
  OscType m_oscType = OscType::None;

  AudioSynthWaveform m_modLFO;
  AudioSynthWaveformPWM m_oscPWM;
  AudioSynthWaveformModulated m_oscWaveform;
  AudioSynthWavetable m_oscWavetable;
  AudioMixer4 m_output;

  AudioConnection m_conn_LFO_PWM{m_modLFO, m_oscPWM};
  AudioConnection m_conn_LFO_Waveform {m_modLFO, 0, m_oscWaveform, 1};

  AudioConnection m_conn_PWM_out{m_oscPWM, 0, m_output, 0};
  AudioConnection m_conn_Waveform_out{m_oscWaveform, 0, m_output, 1};
  AudioConnection m_conn_Wavetable_out{m_oscWavetable, 0, m_output, 2};
};