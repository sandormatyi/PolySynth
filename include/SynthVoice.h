#pragma once

#include <Audio.h>
#include "MidiNote.h"
#include "SynthParameter.h"

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

  AudioSynthWaveform detuneLfo;
  AudioSynthWaveform pwmLfo;

  AudioSynthWaveformSineModulated osc1;
  AudioSynthWaveformPWM osc2;
  AudioSynthNoisePink noise;
  AudioMixer4 mixer;
  AudioSynthWaveformDc filterDc;
  AudioFilterStateVariable filter;
  AudioEffectEnvelope envelopeFilter;
  AudioEffectEnvelope envelopeAmp;

  AudioConnection patchCord5 = {detuneLfo, osc1};
  AudioConnection patchCord2 = {osc1, 0, mixer, 0};

  AudioConnection patchCord6 = {pwmLfo, osc2};
  AudioConnection patchCord1 = {osc2, 0, mixer, 1};

  AudioConnection patchCord3 = {noise, 0, mixer, 2};
  AudioConnection patchCord9 = {mixer, envelopeAmp};
  AudioConnection patchCord7 = {envelopeAmp, 0, filter, 0};

  AudioConnection patchCord4 = {filterDc, envelopeFilter};
  AudioConnection patchCord8 = {envelopeFilter, 0, filter, 1};
};