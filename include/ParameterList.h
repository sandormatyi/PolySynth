#pragma once

#include "SynthParameter.h"
#include <array>

enum SynthParams {
  MasterGain,
  MasterPan,

  // Voice parameters
  Osc1Mix,
  Osc1Type,
  Osc1LFOAmp,
  Osc1LFOFreq,

  Osc2Mix,
  Osc2Type,
  Osc2LFOAmp,
  Osc2LFOFreq,
  
  EnvAttack,
  EnvDecay,
  EnvSustain,
  EnvRelease,
  FilterCutoff,
  FilterReso,
  FilterEnvAttack,
  FilterEnvDecay,
  FilterEnvSustain,
  FilterEnvRelease,
  FilterEnvOctave,

  // Engine parameters
  DryMix,
  ChorusMix,
  ChorusVoices,
  FlangeMix,
  FlangeOffset,
  FlangeDepth,
  FlangeRate,
  ReverbMix,
  ReverbSize,
  ReverbDamping,

  NumParams
};

class ParameterList
{
public:
  ParameterList();

  SynthParameter& get(SynthParams paramIdx) { return m_parameters[paramIdx]; }

private:
  std::array<SynthParameter, SynthParams::NumParams> m_parameters;
};