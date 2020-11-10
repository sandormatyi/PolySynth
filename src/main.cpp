#include "MidiNote.h"
#include "SynthEngine.h"
#include "SynthVoice.h"
#include <Audio.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>
#include <array>

SynthEngine engine;

void noteOnHandler(uint8_t channel, uint8_t note, uint8_t velocity)
{
  engine.noteOn({channel, note, velocity});
}

void noteOffHandler(uint8_t channel, uint8_t note, uint8_t velocity)
{
  engine.noteOff({channel, note, velocity});
}

void controlChangeHandler(uint8_t channel, uint8_t control, uint8_t value)
{
  Serial.printf("CC %d set to %d\n", control, value);
  const float normalizedValue = value / 127.0f;
  const auto paramIdx = control - 10;
  if (paramIdx >= 0 && paramIdx < SynthParams::NumParams) {
    auto& param = engine.getParam(SynthParams(paramIdx));
    param.setNormalizedValue(normalizedValue);
    Serial.printf("%s: %f\n", param.getName().c_str(), param.getValue());
    return;
  }

  if (control == 123) {
    engine.allNotesOff();
  }
}

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 2000)
    ;

  AudioMemory(128);

  usbMIDI.setHandleNoteOn(noteOnHandler);
  usbMIDI.setHandleNoteOff(noteOffHandler);
  usbMIDI.setHandleControlChange(controlChangeHandler);

  if (!engine.init()) {
    Serial.printf("Failed to init engine");
  }

  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();
}

uint32_t lastPrintTime = 0;

void loop()
{
  usbMIDI.read();

  const auto time = millis();
  if (time > lastPrintTime + 2000) {
    Serial.print("Proc = ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" (");    
    Serial.print(AudioProcessorUsageMax());
    Serial.print("),  Mem = ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" (");    
    Serial.print(AudioMemoryUsageMax());
    Serial.println(")");
    lastPrintTime = time;
  }
}