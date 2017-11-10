#include "RecordsController.h"


RecordsController::RecordsController() {
}

RecordsController::~RecordsController() {
}

void RecordsController::launch() {
    setInternalStep(RECORDS_MAIN);
}

void RecordsController::handleEvents(Event* event) {
    if (transition.isRunning()) {
        return;
    }
    switch(step) {
        case RECORDS_MAIN : global.handleEvents(event); break;
        case RECORDS_NORMAL : normal.handleEvents(event); break;
        case RECORDS_EXPERT : expert.handleEvents(event); break;
        default : break;
    }
}

void RecordsController::loop() {
    if (transition.isRunning()) {
        return;
    }
    switch(step) {
        case RECORDS_MAIN : break;
        case RECORDS_NORMAL : break;
        default : break;
    }
}

void RecordsController::draw() {
    switch(step) {
        case RECORDS_MAIN : global.draw(); break;
        case RECORDS_NORMAL : normal.draw(); break;
        case RECORDS_EXPERT : expert.draw(); break;
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

void RecordsController::setStep(RecordsStep newStep) {
    nextStep = newStep;
    transition.start();
}

void RecordsController::setInternalStep(RecordsStep newStep) {
    step = newStep;
    switch(step) {
        case RECORDS_MAIN : global.launch(); break;
        case RECORDS_NORMAL : normal.launch(); break;
        case RECORDS_EXPERT : expert.launch(); break;
        default : break;
    }
}
