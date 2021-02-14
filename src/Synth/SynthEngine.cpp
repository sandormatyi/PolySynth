#include "Synth/SynthEngine.h"

SynthVoice* SynthEngine::getFirstFreeVoice()
{
  for (auto& voice : m_voices)
    if (!voice.isPlaying) return &voice;

  return nullptr;
}

SynthVoice* SynthEngine::getVoiceForNote(MidiNote& note)
{
  for (auto& voice : m_voices)
    if (voice.isPlaying && voice.note.note == note.note) return &voice;

  return nullptr;
}

void SynthEngine::noteOn(MidiNote note)
{
  Serial.printf("MIDI note %d received\n", note.note);

  SynthVoice* voice = getFirstFreeVoice();
  if (voice != nullptr) {
    voice->playNote(note);
  }
}

void SynthEngine::noteOff(MidiNote note)
{
  SynthVoice* voice = getVoiceForNote(note);
  if (voice != nullptr) {
    voice->stop();
  }
}

void SynthEngine::allNotesOff()
{
  for (auto& voice : m_voices)
    voice.stop();
}

bool SynthEngine::init()
{
  const auto chorusVoices = int(m_parameterList.get(SynthParams::ChorusVoices).getValue());
  if (!m_chorus_L.begin(m_delaylineChorus_L, s_chorusDelayLength, chorusVoices)) {
    Serial.printf("Chorus failed to initalize");
    return false;
  }
  if (!m_chorus_R.begin(m_delaylineChorus_R, s_chorusDelayLength, chorusVoices)) {
    Serial.printf("Chorus failed to initalize");
    return false;
  }

  const auto flangeOffset = int(m_parameterList.get(SynthParams::FlangeOffset).getValue());
  const auto flangeDepth = int(m_parameterList.get(SynthParams::FlangeDepth).getValue());
  const auto flangeRate = m_parameterList.get(SynthParams::FlangeRate).getValue();
  if (!m_flanger_L.begin(m_delaylineFlange_L, s_flangeDelayLength, flangeOffset, flangeDepth, flangeRate)) {
    Serial.printf("Flanger failed to initialize");
    return false;
  }
  if (!m_flanger_R.begin(m_delaylineFlange_R, s_flangeDelayLength, flangeOffset, flangeDepth, flangeRate)) {
    Serial.printf("Flanger failed to initialize");
    return false;
  }

  for (auto& voice : m_voices)
    voice.init(this);

  getParam(SynthParams::MasterGain).addListener(this);
  getParam(SynthParams::MasterPan).addListener(this);
  getParam(SynthParams::DryMix).addListener(this);
  getParam(SynthParams::ChorusMix).addListener(this);
  getParam(SynthParams::ChorusVoices).addListener(this);
  getParam(SynthParams::FlangeMix).addListener(this);
  getParam(SynthParams::FlangeOffset).addListener(this);
  getParam(SynthParams::FlangeDepth).addListener(this);
  getParam(SynthParams::FlangeRate).addListener(this);
  getParam(SynthParams::ReverbMix).addListener(this);
  getParam(SynthParams::ReverbSize).addListener(this);
  getParam(SynthParams::ReverbDamping).addListener(this);

  for (size_t i = 0; i < m_mixerVoiceGroups.size(); ++i) {
    for (size_t j = 0; j < 4; ++j) {
      m_mixerVoiceGroups[i].gain(j, 0.25f);
    }
  }
  for (size_t i = 0; i < 4; ++i) {
    m_mixerVoices.gain(i, 0.25f);
  }

  m_master.enable();
  m_master.volume(1.0f);
  m_master.autoVolumeDisable();
  return true;
}

ParameterList& SynthEngine::getParameters()
{
  return m_parameterList;
}

SynthParameter& SynthEngine::getParam(SynthParams paramIdx)
{
  return m_parameterList.get(paramIdx);
}

void SynthEngine::parameterChanged(SynthParameter& param)
{
  switch (param.getId()) {
  case SynthParams::DryMix:
    m_mixerFX_L.gain(0, param.getValue());
    m_mixerFX_R.gain(0, param.getValue());
    break;
  case SynthParams::ChorusMix:
    m_mixerFX_L.gain(2, param.getValue());
    m_mixerFX_R.gain(2, param.getValue());
    break;
  case SynthParams::ChorusVoices:
    m_chorus_L.voices(int(param.getValue()));
    m_chorus_R.voices(int(param.getValue()));
    break;
  case SynthParams::FlangeMix:
    m_mixerFX_L.gain(3, param.getValue());
    m_mixerFX_R.gain(3, param.getValue());
    break;
  case SynthParams::FlangeOffset:
  case SynthParams::FlangeDepth:
  case SynthParams::FlangeRate: {
    const auto offset = getParam(SynthParams::FlangeOffset).getValue();
    const auto depth = getParam(SynthParams::FlangeDepth).getValue();
    const auto rate = getParam(SynthParams::FlangeRate).getValue();
    m_flanger_L.voices(int(offset), int(depth), rate);
    m_flanger_R.voices(int(offset), int(depth), rate);
    break;
  }
  case SynthParams::ReverbMix:
    m_mixerFX_L.gain(1, param.getValue());
    m_mixerFX_R.gain(1, param.getValue());
    break;
  case SynthParams::ReverbSize:
    m_reverb.roomsize(param.getValue());
    break;
  case SynthParams::ReverbDamping:
    m_reverb.damping(param.getValue());
    break;
  case SynthParams::MasterGain:
    m_master.volume(param.getValue());
    break;
  case SynthParams::MasterPan: {
    const auto left = cosf(param.getValue() * PI / 2);
    const auto right = sinf(param.getValue() * PI / 2);
    m_amp_L.gain(left);
    m_amp_R.gain(right);
    break;
  }
  }
}
