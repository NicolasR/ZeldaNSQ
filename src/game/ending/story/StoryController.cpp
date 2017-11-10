#include "StoryController.h"

#include "../../../config/KeyBinder.h"

StoryController::StoryController() {
}

StoryController::~StoryController() {
}

void StoryController::launch() {
    story.init();
}

void StoryController::handleEvents(Event* event) {
    Action* action = KeyBinder::getInstance()->bind(event);
    story.handleActions(action);
}

void StoryController::loop() {
    story.loop();
}

void StoryController::draw() {
    story.draw();
}
