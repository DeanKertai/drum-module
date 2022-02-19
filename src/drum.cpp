#include "drum.h"
#include "utils.h"
#include "midi.h"

Drum::Drum(int inputPin) {
  pin = inputPin;
  midiNote = 38;
  threshold = 10;
  sampleDuration = 15000;
  cutoffDuration = 5000;
  decayDuration = 50000;
  cutoffStartMicros = 0;
  decayStartMicros = 0;
  valueWhenTriggered = 0;  // What was the value last time the pad was triggered
  isInCutoff = false;
  isInDecay = false;
  isNoteOn = false;
  noteOnTime = 0;
  velocityCeiling = 512;
}

int Drum::getSample() {
  unsigned long start = micros();
  int rawValue = analogRead(pin);
  unsigned long sampleCount = 1;
  double totalValue = rawValue;
  if (rawValue >= threshold) {
    while (Utils::isInPeriod(start, sampleDuration)) {
      rawValue = analogRead(pin);
      totalValue += rawValue;
      sampleCount++;
    }
  }
  return (int)(totalValue / sampleCount);
}

void Drum::setMidiNoteOn(int velocity) {
  isNoteOn = true;
  noteOnTime = micros();
  MIDI::sendNoteOn(midiNote, velocity);
}

void Drum::setMidiNoteOff() {
  isNoteOn = false;
  MIDI::sendNoteOff(midiNote);
}

void Drum::update() {
    // Exit cutoff period if enough time has passed
    if (isInCutoff && !Utils::isInPeriod(cutoffStartMicros, cutoffDuration)) {
      isInCutoff = false;
      isInDecay = true;
      decayStartMicros = micros();
    }

    // Don't update when in the cutoff period
    if (isInCutoff) {
      return;
    }

    // Exit decay period when enough time has passed
    if (isInDecay && !Utils::isInPeriod(decayStartMicros, decayDuration)) {
      isInDecay = false;
    }

    // Get the sensor value sample
    int sensorValue = getSample();

    // If we're in the decay period, only accept sensor values higher than the decay threshold
    if (isInDecay) {
      unsigned long decayEndTimestamp = decayStartMicros + decayDuration;
      unsigned long timeUntilDecayEnd = decayEndTimestamp - micros();
      float decayPercentComplete = (float)timeUntilDecayEnd / (float)decayDuration;
      int decayThreshold = valueWhenTriggered * decayPercentComplete;
      if (sensorValue < decayThreshold) {
        return;
      }
    }

    // Turn midi note off after 100ms
    if (isNoteOn && !Utils::isInPeriod(noteOnTime, 100000)) {
      setMidiNoteOff();
    }
    
    if (sensorValue > threshold) {
      isInCutoff = true;
      isInDecay = false;
      cutoffStartMicros = micros();
      valueWhenTriggered = sensorValue;

      if (isNoteOn) {
        setMidiNoteOff();
      }

      float velocityPercent = (float)sensorValue / (float)velocityCeiling;
      if (velocityPercent > 1.0) {
        velocityPercent = 1.0;
      }
      int velocity = velocityPercent * 127.0;
      setMidiNoteOn(velocity);
    }
}
