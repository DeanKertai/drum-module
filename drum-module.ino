#include "src/drum.h";

int DRUM_COUNT = 1;

Drum drums[] = {
    Drum(A0),
};

void setup() {
    Serial.begin(31250);
}

void loop() {
    for (int i = 0; i < DRUM_COUNT; i++) {
        drums[i].update();
    }
}
