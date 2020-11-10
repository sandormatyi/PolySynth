#include "SynthParameter.h"

SynthParameter::SynthParameter(uint8_t id, std::string name, float minValue, float maxValue, float defaultValue)
    : m_id(id), m_name(name), m_minValue(minValue), m_maxValue(maxValue), m_defaultValue(defaultValue), m_value(defaultValue)
{
}

float SynthParameter::getValue() const
{
  return m_value;
}

float SynthParameter::getNormalizedValue() const
{
  return (m_value - m_minValue) / (m_maxValue - m_minValue);
}

void SynthParameter::setValue(float value)
{
  if (value > m_maxValue) value = m_maxValue;
  if (value < m_minValue) value = m_minValue;

  if (m_value != value) {
    m_value = value;
    notifyListeners();
  }
}

void SynthParameter::setNormalizedValue(float normalizedValue)
{
  const auto value = normalizedValue * (m_maxValue - m_minValue) + m_minValue;
  setValue(value);
}

void SynthParameter::reset()
{
  setValue(m_defaultValue);
}

void SynthParameter::addListener(SynthParameter::Listener* listener)
{
  m_listeners.push_back(listener);
  listener->parameterChanged(*this);
}

void SynthParameter::removeListener(SynthParameter::Listener* listener)
{
  for (size_t i = 0; i < m_listeners.size(); ++i) {
    if (m_listeners[i] == listener) {
      m_listeners.erase(m_listeners.begin() + i);
      break;
    }
  }
}

void SynthParameter::notifyListeners()
{
  for (auto listener : m_listeners)
    listener->parameterChanged(*this);
}