#ifndef MIDI_H
#define MIDI_H

#include <Arduino.h>

namespace MIDI {
    const char commandNoteOn = 0x90;
    const char commandNoteOff = 0x80;
    void sendNoteOn(int pitch, int velocity);
    void sendNoteOff(int pitch);
}

#endif
