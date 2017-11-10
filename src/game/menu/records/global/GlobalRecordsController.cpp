#include "GlobalRecordsController.h"

GlobalRecordsController::GlobalRecordsController() {
}

GlobalRecordsController::~GlobalRecordsController() {
}

void GlobalRecordsController::launch() {
    global.init();
}

void GlobalRecordsController::handleEvents(Event* event) {
    global.handleEvents(event);
}

void GlobalRecordsController::draw() {
    global.draw();
}
