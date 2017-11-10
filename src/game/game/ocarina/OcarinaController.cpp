#include "OcarinaController.h"

OcarinaController::OcarinaController() {
}

OcarinaController::~OcarinaController() {
}

void OcarinaController::launch() {
    ocarina.start();
}

void OcarinaController::setSong(int id) {
    ocarina.setSong(id);
}

void OcarinaController::handleActions(Action* action) {
    ocarina.handleActions(action);
}

void OcarinaController::loop() {
    ocarina.loop();
}
