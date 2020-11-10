#include "ParameterList.h"
#include "SynthParameter.h"
#include "Synth/SynthOsc.h"

ParameterList::ParameterList()
{
#define ADD(id, min, max, def) m_parameters[SynthParams::id] = {SynthParams::id, #id, min, max, def};

  ADD(MasterGain, 0.0f, 1.0f, 0.5f);
  ADD(MasterPan, 0.0f, 1.0f, 0.5f);

  // Voice parameters
  ADD(Osc1Mix, 0.0f, 0.25f, 0.25f);
  ADD(Osc1Type, -1.0f, float(int(OscType::NumTypes) - 1), 0.0f);
  ADD(Osc1LFOAmp, 0.0f, 1.0f, 0.01f);
  ADD(Osc1LFOFreq, 0.0f, 32.0f, 0.5f);

  ADD(Osc2Mix, 0.0f, 0.25f, 0.25f);
  ADD(Osc2Type, -1.0f, float(int(OscType::NumTypes) - 1), 0.0f);
  ADD(Osc2LFOAmp, 0.0f, 1.0f, 0.6f);
  ADD(Osc2LFOFreq, 0.0f, 32.0f, 0.5f);

  ADD(EnvAttack, 0.0f, 5000.0f, 10.5f);
  ADD(EnvDecay, 0.0f, 5000.0f, 2.5f);
  ADD(EnvSustain, 0.0f, 1.0f, 0.5f);
  ADD(EnvRelease, 0.0f, 5000.0f, 300.0f);
  ADD(FilterCutoff, 20.0f, 17000.0f, 6000.0f);
  ADD(FilterReso, 0.7f, 5.0f, 3.0f);
  ADD(FilterEnvAttack, 0.0f, 5000.0f, 100.0f);
  ADD(FilterEnvDecay, 0.0f, 5000.0f, 2.5f);
  ADD(FilterEnvSustain, 0.0f, 1.0f, 1.0f);
  ADD(FilterEnvRelease, 0.0f, 5000.0f, 5000.0f);
  ADD(FilterEnvOctave, -7.0f, 7.0f, -3.5f);

  // Engine parameters
  ADD(DryMix, 0.0f, 0.15f, 0.15f);
  ADD(ChorusMix, 0.0f, 1.0f, 0.0f);
  ADD(ChorusVoices, 1.0f, 32.0f, 2.0f);
  ADD(FlangeMix, 0.0f, 1.0f, 0.0f);
  ADD(FlangeOffset, 0.0f, 512.0f, 128.0f);
  ADD(FlangeDepth, 0.0f, 512.0f, 128.0f);
  ADD(FlangeRate, 0.0f, 32.0f, 3.0f);
  ADD(ReverbMix, 0.0f, 1.0f, 0.0f);
  ADD(ReverbSize, 0.0f, 1.0f, 0.0f);
  ADD(ReverbDamping, 0.0f, 1.0f, 1.0f);

#undef ADD
}