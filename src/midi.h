#ifndef MIDI_H
#define MIDI_H

#include <Arduino.h>

namespace MIDI {
    void sendNoteOn(int pitch, int velocity) {
        Serial.write(0x90);
        Serial.write(pitch);
        Serial.write(velocity);
    }

    void sendNoteOff(int pitch) {
        Serial.write(0x80);
        Serial.write(pitch);
        Serial.write(0);
    }
}

#endif
