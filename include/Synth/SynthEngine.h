#pragma once

#include <Audio.h>
#include <array>

#include "ParameterList.h"
#include "SynthVoice.h"

class SynthEngine : public SynthParameter::Listener
{
private:
  static constexpr int s_chorusDelayLength = 32 * AUDIO_BLOCK_SAMPLES;
  static constexpr int s_flangeDelayLength = 32 * AUDIO_BLOCK_SAMPLES;
  static constexpr int s_numVoices = 16;

public:
  SynthEngine() = default;

  void noteOn(MidiNote note);
  void noteOff(MidiNote note);
  void allNotesOff();

  bool init();

  ParameterList& getParameters();
  SynthParameter& getParam(SynthParams paramIdx);

  AudioStream& getOutput() { return m_output; }

  void parameterChanged(SynthParameter& param) override;

private:
  SynthVoice* getFirstFreeVoice();
  SynthVoice* getVoiceForNote(MidiNote& note);

  std::array<SynthVoice, s_numVoices> m_voices;
  ParameterList m_parameterList;

  short m_delaylineChorus_L[s_chorusDelayLength];
  short m_delaylineChorus_R[s_chorusDelayLength];
  short m_delaylineFlange_L[s_flangeDelayLength];
  short m_delaylineFlange_R[s_flangeDelayLength];

  std::array<AudioMixer4, 4> m_mixerVoiceGroups;  //xy=388,474
  AudioMixer4 m_mixerVoices;          //xy=388,474
  AudioEffectFlange m_flanger_L;      //xy=419,238
  AudioEffectFlange m_flanger_R;      //xy=419,238
  AudioEffectChorus m_chorus_L;       //xy=420,199
  AudioEffectChorus m_chorus_R;       //xy=420,199
  AudioEffectFreeverbStereo m_reverb; //xy=424,154
  AudioMixer4 m_mixerFX_L;            //xy=584,148
  AudioMixer4 m_mixerFX_R;            //xy=584,148
  AudioAmplifier m_amp_L;             //xy=748,171
  AudioAmplifier m_amp_R;             //xy=750,130
  AudioOutputI2S m_output;            //xy=891,154
  AudioControlSGTL5000 m_master;

  AudioConnection patchCoord_0{m_voices[0].getOutput(), 0, m_mixerVoiceGroups[0], 0};
  AudioConnection patchCoord_1{m_voices[1].getOutput(), 0, m_mixerVoiceGroups[0], 1};
  AudioConnection patchCoord_2{m_voices[2].getOutput(), 0, m_mixerVoiceGroups[0], 2};
  AudioConnection patchCoord_3{m_voices[3].getOutput(), 0, m_mixerVoiceGroups[0], 3};
  AudioConnection patchCoord_4{m_voices[4].getOutput(), 0, m_mixerVoiceGroups[1], 0};
  AudioConnection patchCoord_5{m_voices[5].getOutput(), 0, m_mixerVoiceGroups[1], 1};
  AudioConnection patchCoord_6{m_voices[6].getOutput(), 0, m_mixerVoiceGroups[1], 2};
  AudioConnection patchCoord_7{m_voices[7].getOutput(), 0, m_mixerVoiceGroups[1], 3};
  AudioConnection patchCoord_8{m_voices[8].getOutput(), 0, m_mixerVoiceGroups[2], 0};
  AudioConnection patchCoord_9{m_voices[9].getOutput(), 0, m_mixerVoiceGroups[2], 1};
  AudioConnection patchCoord_10{m_voices[10].getOutput(), 0, m_mixerVoiceGroups[2], 2};
  AudioConnection patchCoord_11{m_voices[11].getOutput(), 0, m_mixerVoiceGroups[2], 3};
  AudioConnection patchCoord_12{m_voices[12].getOutput(), 0, m_mixerVoiceGroups[3], 0};
  AudioConnection patchCoord_13{m_voices[13].getOutput(), 0, m_mixerVoiceGroups[3], 1};
  AudioConnection patchCoord_14{m_voices[14].getOutput(), 0, m_mixerVoiceGroups[3], 2};
  AudioConnection patchCoord_15{m_voices[15].getOutput(), 0, m_mixerVoiceGroups[3], 3};
  
  AudioConnection patchCoord1{m_mixerVoiceGroups[0], 0, m_mixerVoices, 0};
  AudioConnection patchCoord2{m_mixerVoiceGroups[1], 0, m_mixerVoices, 1};
  AudioConnection patchCoord3{m_mixerVoiceGroups[2], 0, m_mixerVoices, 2};
  AudioConnection patchCoord4{m_mixerVoiceGroups[3], 0, m_mixerVoices, 3};

  AudioConnection patchCord5{m_mixerVoices, m_reverb};
  AudioConnection patchCord6{m_mixerVoices, m_chorus_L};
  AudioConnection patchCord7{m_mixerVoices, m_chorus_R};
  AudioConnection patchCord8{m_mixerVoices, m_flanger_L};
  AudioConnection patchCord9{m_mixerVoices, m_flanger_R};

  AudioConnection patchCord10{m_mixerVoices, 0, m_mixerFX_L, 0};
  AudioConnection patchCord11{m_reverb, 0, m_mixerFX_L, 1};
  AudioConnection patchCord12{m_chorus_L, 0, m_mixerFX_L, 2};
  AudioConnection patchCord13{m_flanger_L, 0, m_mixerFX_L, 3};

  AudioConnection patchCord14{m_mixerVoices, 0, m_mixerFX_R, 0};
  AudioConnection patchCord15{m_reverb, 1, m_mixerFX_R, 1};
  AudioConnection patchCord16{m_chorus_R, 0, m_mixerFX_R, 2};
  AudioConnection patchCord17{m_flanger_R, 0, m_mixerFX_R, 3};

  AudioConnection patchCord18{m_mixerFX_L, m_amp_L};
  AudioConnection patchCord19{m_mixerFX_R, m_amp_R};
  AudioConnection patchCord20{m_amp_L, 0, m_output, 0};
  AudioConnection patchCord21{m_amp_R, 0, m_output, 1};
};