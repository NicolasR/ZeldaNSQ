#include "GameMenuController.h"

GameMenuController::GameMenuController() {
}

GameMenuController::~GameMenuController() {
}

void GameMenuController::init() {
    menu.init();
}

void GameMenuController::launch() {
    menu.start();
}

void GameMenuController::handleActions(Action* action) {
    menu.handleActions(action);
}

void GameMenuController::loop() {
    menu.loop();
}

void GameMenuController::draw() {
    menu.draw();
}

Menu* GameMenuController::getMenu() {
    return &menu;
}
