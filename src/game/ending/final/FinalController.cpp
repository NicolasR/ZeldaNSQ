#include "FinalController.h"

FinalController::FinalController() {
}

FinalController::~FinalController() {
}

void FinalController::launch() {
    final.init();
}

void FinalController::handleEvents(Event* event) {
    final.handleEvents(event);
}

void FinalController::loop() {
    final.loop();
}

void FinalController::draw() {
    final.draw();
}
