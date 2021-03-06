#ifndef DRUM_H
#define DRUM_H

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
    bool isNoteOn;
    unsigned long noteOnTime;
    int velocityCeiling;
    int getSample();
    void setMidiNoteOn(int velocity);
    void setMidiNoteOff();
    
  public:
    Drum(int pin);
    void update();
};

#endif
