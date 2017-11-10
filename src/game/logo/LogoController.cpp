#include "LogoController.h"

LogoController::LogoController() {
}

LogoController::~LogoController() {
}

void LogoController::launch() {
    logo.init();
}

void LogoController::handleEvents(Event* event) {
    logo.handleEvents(event);
}

void LogoController::loop() {
    logo.loop();
}

void LogoController::draw() {
    logo.draw();
}
