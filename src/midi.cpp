#include "midi.h"

void MIDI::sendNoteOn(int pitch, int velocity) {
    Serial.write(commandNoteOn);
    Serial.write(pitch);
    Serial.write(velocity);
}

void MIDI::sendNoteOff(int pitch) {
    Serial.write(commandNoteOff);
    Serial.write(pitch);
    Serial.write(0);  // Zero velocity
}
