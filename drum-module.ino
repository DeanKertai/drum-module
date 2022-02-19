#include "src/drum.h";

const int drumCount = 1;

Drum drums[] = {
    Drum(A0),
};

void setup() {
    Serial.begin(31250);
}

void loop() {
    for (int i = 0; i < drumCount; i++) {
        drums[i].update();
    }
}
