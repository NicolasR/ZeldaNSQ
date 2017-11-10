#include "ScoreController.h"

ScoreController::ScoreController() {
}

ScoreController::~ScoreController() {
}

void ScoreController::launch() {
    score.init();
}

void ScoreController::handleEvents(Event* event) {
    score.handleEvents(event);
}

void ScoreController::loop() {
}

void ScoreController::draw() {
    score.draw();
}
