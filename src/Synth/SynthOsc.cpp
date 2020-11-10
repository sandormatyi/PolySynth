#include "Synth/SynthOsc.h"
#include "MidiNote.h"

void SynthOsc::init()
{
  m_modLFO.begin(WAVEFORM_SINE);

  m_output.gain(0, 1.0f);
  m_output.gain(1, 1.0f);
  m_output.gain(2, 1.0f);
  m_output.gain(3, 0.0f);

  m_oscWavetable.setInstrument(organ);

  m_oscWaveform.frequencyModulation(0.1f);
  m_oscWaveform.phaseModulation(180.0f);
}

AudioStream& SynthOsc::getOutput()
{
  return m_output;
}

AudioSynthWaveform& SynthOsc::getLFO()
{
  return m_modLFO;
}

void SynthOsc::setOscType(OscType type)
{
  if (type == m_oscType)
    return;

  m_oscType = type;

  AudioNoInterrupts();

  m_conn_PWM_out.disconnect();
  m_conn_Waveform_out.disconnect();
  m_conn_Wavetable_out.disconnect();

  switch (type) {
  case OscType::None:
    return;

  case OscType::PWM:
    m_conn_LFO_PWM.connect();
    break;

  case OscType::Wavetable:
    m_conn_Wavetable_out.connect();
    break;

  case OscType::Sine:
  case OscType::Saw:
  case OscType::Square:
  case OscType::Triangle:
  case OscType::Arbitrary:
  case OscType::Pulse:
  case OscType::ReverseSaw:
  case OscType::SnH:
  case OscType::TriangleVar:
    m_oscWaveform.begin(short(type));
    m_conn_Waveform_out.connect();
    break;

  default:
    break;
  }


  AudioInterrupts();
}

void SynthOsc::noteOn(int noteNumber, int velocity)
{
  const auto frequency = MidiNote::getFrequency(noteNumber);
  const auto amplitude = velocity / 127.0;

  switch (m_oscType) {
  case OscType::PWM:
    m_oscPWM.frequency(frequency);
    m_oscPWM.amplitude(amplitude);
    break;

  case OscType::Wavetable:
    m_oscWavetable.playFrequency(frequency, velocity);
    break;

  case OscType::Sine:
  case OscType::Saw:
  case OscType::Square:
  case OscType::Triangle:
  case OscType::Arbitrary:
  case OscType::Pulse:
  case OscType::ReverseSaw:
  case OscType::SnH:
  case OscType::TriangleVar:
    m_oscWaveform.begin(amplitude, frequency, short(m_oscType));
    break;

  default:
    break;
  }
}