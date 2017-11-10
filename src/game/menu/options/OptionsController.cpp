#include "OptionsController.h"


OptionsController::OptionsController() {
}

OptionsController::~OptionsController() {
}

void OptionsController::launch() {
    setInternalStep(OPTIONS_MAIN);
}

void OptionsController::handleEvents(Event* event) {
    if (transition.isRunning()) {
        return;
    }
    switch(step) {
        case OPTIONS_MAIN : global.handleEvents(event); break;
        case OPTIONS_AUDIO : audio.handleEvents(event); break;
        default : break;
    }
}

void OptionsController::loop() {
    if (transition.isRunning()) {
        return;
    }
    switch(step) {
        case OPTIONS_MAIN : break;
        case OPTIONS_AUDIO : break;
        default : break;
    }
}

void OptionsController::draw() {
    switch(step) {
        case OPTIONS_MAIN : global.draw(); break;
        case OPTIONS_AUDIO : audio.draw(); break;
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

void OptionsController::setStep(OptionsStep newStep) {
    nextStep = newStep;
    transition.start();
}

void OptionsController::setInternalStep(OptionsStep newStep) {
    step = newStep;
    switch(step) {
        case OPTIONS_MAIN : global.launch(); break;
        case OPTIONS_AUDIO : audio.launch(); break;
        default : break;
    }
}
