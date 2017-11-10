#include "GlobalOptionsController.h"

GlobalOptionsController::GlobalOptionsController() {
}

GlobalOptionsController::~GlobalOptionsController() {
}

void GlobalOptionsController::launch() {
    global.init();
}

void GlobalOptionsController::handleEvents(Event* event) {
    global.handleEvents(event);
}

void GlobalOptionsController::draw() {
    global.draw();
}
