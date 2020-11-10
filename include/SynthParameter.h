#pragma once

#include <inttypes.h>
#include <string>
#include <vector>

class SynthParameter
{
public:
  class Listener
  {
  public:
    virtual void parameterChanged(SynthParameter& param) = 0;
  };

  SynthParameter() = default;
  SynthParameter(uint8_t id, std::string name, float minValue = 0.0f, float maxValue = 1.0f, float defaultValue = 0.0f);

  float getValue() const;
  float getNormalizedValue() const;

  void setValue(float value);
  void setNormalizedValue(float normalizedValue);

  void reset();

  void addListener(Listener* listener);
  void removeListener(Listener* listener);
  void notifyListeners();

  uint8_t getId() const { return m_id; }
  const std::string& getName() const { return m_name; }

private:
  uint8_t m_id;
  std::string m_name;
  float m_minValue;
  float m_maxValue;
  float m_defaultValue;
  float m_value;

  std::vector<Listener*> m_listeners;
};