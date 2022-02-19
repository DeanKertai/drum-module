#include "drum.h"
#include "utils.h"

Drum::Drum(int inputPin) {
  pin = inputPin;
  midiNote = 38;
  threshold = 5;
  sampleDuration = 5000;
  cutoffDuration = 30000;
  decayDuration = 80000;
  cutoffStartMicros = 0;
  decayStartMicros = 0;
  isInCutoff = false;
  isInDecay = false;
  valueWhenTriggered = 0;  // What was the value last time the pad was triggered
}

int Drum::getSample() {
  unsigned long start = micros();
  int rawValue = analogRead(pin);
  int maxValue = rawValue;
  if (rawValue >= threshold) {
    while (Utils::isInPeriod(start, sampleDuration)) {
      rawValue = analogRead(pin);
      if (rawValue > maxValue) {
        maxValue = rawValue;
      }
    }
  }
  return maxValue;
}

void Drum::update() {
    // Exit cutoff period if enough time has passed
    if (isInCutoff && !Utils::isInPeriod(cutoffStartMicros, cutoffDuration)) {
      isInCutoff = false;
      isInDecay = true;
      decayStartMicros = micros();
      TEMP_TIMESTAMP = micros();
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
    
    if (sensorValue > threshold) {
      isInCutoff = true;
      isInDecay = false;
      cutoffStartMicros = micros();
      valueWhenTriggered = sensorValue;

      Serial.println(valueWhenTriggered);
    }
}
