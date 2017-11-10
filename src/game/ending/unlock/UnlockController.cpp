#include "UnlockController.h"

UnlockController::UnlockController() {
}

UnlockController::~UnlockController() {
}

void UnlockController::launch() {
    unlock.init();
}

void UnlockController::handleEvents(Event* event) {
    unlock.handleEvents(event);
}

void UnlockController::loop() {
}

void UnlockController::draw() {
    unlock.draw();
}
