#include "GameTrocController.h"

GameTrocController::GameTrocController() {
}

GameTrocController::~GameTrocController() {
}

void GameTrocController::launch() {
    troc.start();
}

void GameTrocController::handleActions(Action* action) {
    troc.handleActions(action);
}

void GameTrocController::loop() {
    troc.loop();
}

void GameTrocController::draw() {
    troc.draw();
}

Troc* GameTrocController::getTroc() {
    return &troc;
}
