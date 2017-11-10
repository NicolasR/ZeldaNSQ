#include "Metronome.h"

Metronome::Metronome(int max, int delay) : value(0), max(max), delay(delay) {
}

Metronome::~Metronome() {
}

void Metronome::reset() {
    value = 0;
    chrono.reset();
}

void Metronome::loop() {
    if (chrono.getElapsedTime() >= delay) {
        value++;
        if (value >= max) {
            value = 0;
        }
        chrono.reset();
    }
}

int Metronome::getValue() {
    return value;
}
