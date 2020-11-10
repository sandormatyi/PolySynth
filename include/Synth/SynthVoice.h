#pragma once

#include "MidiNote.h"
#include "SynthParameter.h"
#include "Synth/SynthOsc.h"
#include <Audio.h>

class SynthEngine;

class SynthVoice : public SynthParameter::Listener
{
public:
  SynthVoice() = default;
  SynthVoice(const SynthVoice&) = delete;

  bool isPlaying = false;
  MidiNote note;

  void playNote(MidiNote midiNote);
  void stop();
  void init(SynthEngine* engine);
  void clear();

  AudioStream& getOutput();

  void parameterChanged(SynthParameter& param) override;

private:
  SynthEngine* m_engine = nullptr;
  SynthOsc m_osc1;
  SynthOsc m_osc2;

  AudioMixer4 mixer;
  AudioSynthWaveformDc filterDc;
  AudioFilterStateVariable filter;
  AudioEffectEnvelope envelopeFilter;
  AudioEffectEnvelope envelopeAmp;

  AudioConnection patchCord2 = {m_osc1.getOutput(), 0, mixer, 0};
  AudioConnection patchCord1 = {m_osc2.getOutput(), 0, mixer, 1};

  AudioConnection patchCord9 = {mixer, envelopeAmp};
  AudioConnection patchCord7 = {envelopeAmp, 0, filter, 0};

  AudioConnection patchCord4 = {filterDc, envelopeFilter};
  AudioConnection patchCord8 = {envelopeFilter, 0, filter, 1};
};