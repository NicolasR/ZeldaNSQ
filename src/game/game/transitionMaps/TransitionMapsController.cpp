#include "TransitionMapsController.h"

TransitionMapsController::TransitionMapsController() {
}

TransitionMapsController::~TransitionMapsController() {
}

void TransitionMapsController::launch() {
    transitionMaps.init();
}

void TransitionMapsController::loop() {
    transitionMaps.loop();
}

void TransitionMapsController::draw() {
    transitionMaps.draw();
}
