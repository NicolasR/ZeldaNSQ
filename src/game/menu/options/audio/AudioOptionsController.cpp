#include "AudioOptionsController.h"

AudioOptionsController::AudioOptionsController() {
}

AudioOptionsController::~AudioOptionsController() {
}

void AudioOptionsController::launch() {
    audio.init();
}

void AudioOptionsController::handleEvents(Event* event) {
    audio.handleEvents(event);
}

void AudioOptionsController::draw() {
    audio.draw();
}
