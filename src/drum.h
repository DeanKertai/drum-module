#ifndef DRUM_H
#define DRUM_H

#include <Arduino.h>

class Drum {
  private:
    int pin;
    int threshold;
    unsigned int midiNote;
    unsigned long sampleDuration;
    unsigned long cutoffDuration;
    unsigned long decayDuration;
    bool isInCutoff;
    bool isInDecay;
    int valueWhenTriggered;
    unsigned long cutoffStartMicros;
    unsigned long decayStartMicros;
    unsigned long TEMP_TIMESTAMP;
    int getSample();
    
  public:
    Drum(int pin);
    void update();
};

#endif
