#include "Synth/SynthVoice.h"
#include "Synth/SynthEngine.h"

void SynthVoice::playNote(MidiNote midiNote)
{
  if (m_engine == nullptr) return;

  Serial.printf("Playing note %d\n", midiNote.note);

  isPlaying = true;
  note = midiNote;

  //const auto frequency = MidiNote::getFrequency(note.note);
  //const auto osc1Coarse = int(m_engine->getParam(SynthParams::Osc1Coarse).getValue());
  //const auto osc1Frequency = MidiNote::getFrequency(note.note + osc1Coarse);
  //const auto amplitude = note.velocity / 127.0;

  AudioNoInterrupts();
  m_osc1.noteOn(midiNote.note, midiNote.velocity);
  m_osc2.noteOn(midiNote.note, midiNote.velocity);
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

  m_osc1.init();
  m_osc2.init();

  engine->getParam(SynthParams::Osc1Mix).addListener(this);
  engine->getParam(SynthParams::Osc1Type).addListener(this);
  engine->getParam(SynthParams::Osc1LFOAmp).addListener(this);
  engine->getParam(SynthParams::Osc1LFOFreq).addListener(this);
  engine->getParam(SynthParams::Osc2Mix).addListener(this);
  engine->getParam(SynthParams::Osc2Type).addListener(this);
  engine->getParam(SynthParams::Osc2LFOAmp).addListener(this);
  engine->getParam(SynthParams::Osc2LFOFreq).addListener(this);
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
  m_engine->getParam(SynthParams::Osc1Type).removeListener(this);
  m_engine->getParam(SynthParams::Osc1LFOAmp).removeListener(this);
  m_engine->getParam(SynthParams::Osc1LFOFreq).removeListener(this);
  m_engine->getParam(SynthParams::Osc2Mix).removeListener(this);
  m_engine->getParam(SynthParams::Osc2Type).removeListener(this);
  m_engine->getParam(SynthParams::Osc2LFOAmp).removeListener(this);
  m_engine->getParam(SynthParams::Osc2LFOFreq).removeListener(this);
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
  case SynthParams::Osc1Type:
    Serial.printf("Osc type set to %d\n", int(param.getValue()));

    m_osc1.setOscType(OscType(int(param.getValue())));
    break;
  case SynthParams::Osc2Type:
    Serial.printf("Osc type set to %d\n", int(param.getValue()));

    m_osc2.setOscType(OscType(int(param.getValue())));
    break;
  case SynthParams::Osc1LFOAmp:
    m_osc1.getLFO().amplitude(param.getValue());
    m_osc1.getLFO().offset(-param.getValue() / 2);
    break;
  case SynthParams::Osc1LFOFreq:
    m_osc1.getLFO().frequency(param.getValue());
    break;
  case SynthParams::Osc2LFOAmp:
    m_osc2.getLFO().amplitude(param.getValue());
    m_osc2.getLFO().offset(-param.getValue() / 2);
    break;
  case SynthParams::Osc2LFOFreq:
    m_osc2.getLFO().frequency(param.getValue());
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
