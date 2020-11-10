#include "SynthVoice.h"
#include "SynthEngine.h"

void SynthVoice::playNote(MidiNote midiNote)
{
  if (m_engine == nullptr) return;

  Serial.printf("Playing note %d\n", midiNote.note);

  isPlaying = true;
  note = midiNote;

  const auto frequency = MidiNote::getFrequency(note.note);
  const auto osc1Coarse = int(m_engine->getParam(SynthParams::Osc1Coarse).getValue());
  const auto osc1Frequency = MidiNote::getFrequency(note.note + osc1Coarse);
  const auto amplitude = note.velocity / 127.0;

  AudioNoInterrupts();
  osc1.frequency(osc1Frequency);
  osc2.frequency(frequency);
  osc1.amplitude(amplitude);
  osc2.amplitude(amplitude);
  envelopeAmp.noteOn();
  envelopeFilter.noteOn();
  AudioInterrupts();
}

void SynthVoice::stop()
{
  AudioNoInterrupts();
  envelopeAmp.noteOff();
  envelopeFilter.noteOff();
  AudioInterrupts();

  isPlaying = false;
}

void SynthVoice::init(SynthEngine* engine)
{
  m_engine = engine;

  noise.amplitude(0.2f);
  detuneLfo.begin(WAVEFORM_SINE);
  pwmLfo.begin(WAVEFORM_SINE);

  engine->getParam(SynthParams::Osc1Mix).addListener(this);
  engine->getParam(SynthParams::Osc2Mix).addListener(this);
  engine->getParam(SynthParams::Osc3Mix).addListener(this);
  engine->getParam(SynthParams::Osc1Coarse).addListener(this);
  engine->getParam(SynthParams::Osc1DetuneLFOAmp).addListener(this);
  engine->getParam(SynthParams::Osc1DetuneLFOFreq).addListener(this);
  engine->getParam(SynthParams::Osc2PWMLFOAmp).addListener(this);
  engine->getParam(SynthParams::Osc2PWMLFOFreq).addListener(this);
  engine->getParam(SynthParams::EnvAttack).addListener(this);
  engine->getParam(SynthParams::EnvDecay).addListener(this);
  engine->getParam(SynthParams::EnvSustain).addListener(this);
  engine->getParam(SynthParams::EnvRelease).addListener(this);
  engine->getParam(SynthParams::FilterCutoff).addListener(this);
  engine->getParam(SynthParams::FilterReso).addListener(this);
  engine->getParam(SynthParams::FilterEnvAttack).addListener(this);
  engine->getParam(SynthParams::FilterEnvDecay).addListener(this);
  engine->getParam(SynthParams::FilterEnvSustain).addListener(this);
  engine->getParam(SynthParams::FilterEnvRelease).addListener(this);
  engine->getParam(SynthParams::FilterEnvOctave).addListener(this);
}

void SynthVoice::clear()
{
  m_engine->getParam(SynthParams::Osc1Mix).removeListener(this);
  m_engine->getParam(SynthParams::Osc2Mix).removeListener(this);
  m_engine->getParam(SynthParams::Osc3Mix).removeListener(this);
  m_engine->getParam(SynthParams::Osc1Coarse).removeListener(this);
  m_engine->getParam(SynthParams::Osc1DetuneLFOAmp).removeListener(this);
  m_engine->getParam(SynthParams::Osc1DetuneLFOFreq).removeListener(this);
  m_engine->getParam(SynthParams::Osc2PWMLFOAmp).removeListener(this);
  m_engine->getParam(SynthParams::Osc2PWMLFOFreq).removeListener(this);
  m_engine->getParam(SynthParams::EnvAttack).removeListener(this);
  m_engine->getParam(SynthParams::EnvDecay).removeListener(this);
  m_engine->getParam(SynthParams::EnvSustain).removeListener(this);
  m_engine->getParam(SynthParams::EnvRelease).removeListener(this);
  m_engine->getParam(SynthParams::FilterCutoff).removeListener(this);
  m_engine->getParam(SynthParams::FilterReso).removeListener(this);
  m_engine->getParam(SynthParams::FilterEnvAttack).removeListener(this);
  m_engine->getParam(SynthParams::FilterEnvDecay).removeListener(this);
  m_engine->getParam(SynthParams::FilterEnvSustain).removeListener(this);
  m_engine->getParam(SynthParams::FilterEnvRelease).removeListener(this);
  m_engine->getParam(SynthParams::FilterEnvOctave).removeListener(this);

  m_engine = nullptr;
}

AudioStream& SynthVoice::getOutput()
{
  return filter;
}

void SynthVoice::parameterChanged(SynthParameter& param)
{
  switch (param.getId()) {
  case SynthParams::Osc1Mix:
    mixer.gain(0, param.getValue());
    break;
  case SynthParams::Osc2Mix:
    mixer.gain(1, param.getValue());
    break;
  case SynthParams::Osc3Mix:
    mixer.gain(2, param.getValue());
    break;
  case SynthParams::Osc1DetuneLFOAmp:
    detuneLfo.amplitude(param.getValue());
    detuneLfo.offset(-param.getValue() / 2);
    break;
  case SynthParams::Osc1DetuneLFOFreq:
    detuneLfo.frequency(param.getValue());
    break;
  case SynthParams::Osc2PWMLFOAmp:
    pwmLfo.amplitude(param.getValue());
    pwmLfo.offset(-param.getValue() / 2);
    break;
  case SynthParams::Osc2PWMLFOFreq:
    pwmLfo.frequency(param.getValue());
    break;
  case SynthParams::EnvAttack:
    envelopeAmp.attack(param.getValue());
    break;
  case SynthParams::EnvDecay:
    envelopeAmp.decay(param.getValue());
    break;
  case SynthParams::EnvSustain:
    envelopeAmp.sustain(param.getValue());
    break;
  case SynthParams::EnvRelease:
    envelopeAmp.release(param.getValue());
    break;
  case SynthParams::FilterCutoff:
    filter.frequency(param.getValue());
    break;
  case SynthParams::FilterReso:
    filter.resonance(param.getValue());
    break;
  case SynthParams::FilterEnvAttack:
    envelopeFilter.attack(param.getValue());
    break;
  case SynthParams::FilterEnvDecay:
    envelopeFilter.decay(param.getValue());
    break;
  case SynthParams::FilterEnvSustain:
    envelopeFilter.sustain(param.getValue());
    break;
  case SynthParams::FilterEnvRelease:
    envelopeFilter.release(param.getValue());
    break;
  case SynthParams::FilterEnvOctave:
    AudioNoInterrupts();
    filterDc.amplitude((param.getValue() < 0) ? -1.0f : 1.0f);
    filter.octaveControl(fabs(param.getValue()));
    AudioInterrupts();
    break;
  }
}
