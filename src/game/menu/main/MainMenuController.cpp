#include "MainMenuController.h"

MainMenuController::MainMenuController() {
}

MainMenuController::~MainMenuController() {
}

void MainMenuController::launch() {
    menu.init();
}

void MainMenuController::handleEvents(Event* event) {
    menu.handleEvents(event);
}

void MainMenuController::draw() {
    menu.draw();
}
