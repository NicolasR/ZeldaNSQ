#include "CreditsController.h"

CreditsController::CreditsController() {
}

CreditsController::~CreditsController() {
}

void CreditsController::launch() {
    credits.init();
}

void CreditsController::handleEvents(Event* event) {
    credits.handleEvents(event);
}

void CreditsController::loop() {
    credits.loop();
}

void CreditsController::draw() {
    credits.draw();
}
