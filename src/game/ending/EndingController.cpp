#include "EndingController.h"

EndingController::EndingController() {
}

EndingController::~EndingController() {
}

void EndingController::launch() {
    setInternalStep(END_STORY);
}

void EndingController::handleEvents(Event* event) {
    if (transition.isRunning()) {
        return;
    }
    switch(step) {
        case END_STORY : story.handleEvents(event); break;
        case END_SCORE : score.handleEvents(event); break;
        case END_CREDITS : credits.handleEvents(event); break;
        case END_END : final.handleEvents(event); break;
        case END_UNLOCK : unlock.handleEvents(event); break;
        default : break;
    }
    // Escape should not end the game
    if (event->isPushed(kEscape)) {
        event->setEvent(kEscape, false);
    }
}

void EndingController::loop() {
    if (transition.isRunning()) {
        return;
    }
    switch(step) {
        case END_STORY : story.loop(); break;
        case END_SCORE : score.loop(); break;
        case END_CREDITS : credits.loop(); break;
        case END_END : final.loop(); break;
        case END_UNLOCK : unlock.loop(); break;
        default : break;
    }
}

void EndingController::draw() {
    switch(step) {
        case END_STORY : story.draw(); break;
        case END_SCORE : score.draw(); break;
        case END_CREDITS : credits.draw(); break;
        case END_END : final.draw(); break;
        case END_UNLOCK : unlock.draw(); break;
        default : break;
    }

    if (transition.isRunning()) {
        transition.loop();
        transition.draw();

        if (transition.isWaiting()) {
            setInternalStep(nextStep);
            transition.restart();
        } else if (transition.isAchieved()) {
            transition.reset();
        }
    }
}

void EndingController::setStep(EndingStep newStep) {
    nextStep = newStep;
    transition.start();
}

void EndingController::setInternalStep(EndingStep newStep) {
    step = newStep;
    switch(step) {
        case END_STORY : story.launch(); break;
        case END_SCORE : score.launch(); break;
        case END_CREDITS : credits.launch(); break;
        case END_END : final.launch(); break;
        case END_UNLOCK : unlock.launch(); break;
        default : break;
    }
}
