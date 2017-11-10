#include "NormalRecordsController.h"

NormalRecordsController::NormalRecordsController() {
}

NormalRecordsController::~NormalRecordsController() {
}

void NormalRecordsController::launch() {
    normal.init();
}

void NormalRecordsController::handleEvents(Event* event) {
    normal.handleEvents(event);
}

void NormalRecordsController::draw() {
    normal.draw();
}
