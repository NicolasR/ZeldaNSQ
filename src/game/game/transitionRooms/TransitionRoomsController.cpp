#include "TransitionRoomsController.h"

TransitionRoomsController::TransitionRoomsController() {
}

TransitionRoomsController::~TransitionRoomsController() {
}

void TransitionRoomsController::launch() {
    transitionRooms.init();
}

void TransitionRoomsController::loop() {
    transitionRooms.loop();
}
