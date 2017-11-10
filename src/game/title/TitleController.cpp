#include "TitleController.h"

TitleController::TitleController() {
}

TitleController::~TitleController() {
}

void TitleController::launch() {
    title.init();
}

void TitleController::handleEvents(Event* event) {
    title.handleEvents(event);
}

void TitleController::loop() {
    title.loop();
}

void TitleController::draw() {
    title.draw();
}
