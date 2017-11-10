#include "MenuController.h"


MenuController::MenuController() {
}

MenuController::~MenuController() {
}

void MenuController::launch() {
    setInternalStep(MENU_MAIN);
}

void MenuController::handleEvents(Event* event) {
    if (transition.isRunning()) {
        return;
    }
    switch(step) {
        case MENU_MAIN : main.handleEvents(event); break;
        case MENU_OPTIONS : options.handleEvents(event); break;
        case MENU_RECORDS : records.handleEvents(event); break;
        default : break;
    }
}

void MenuController::loop() {
    if (transition.isRunning()) {
        return;
    }
    switch(step) {
        case MENU_MAIN : break;
        case MENU_OPTIONS : break;
        case MENU_RECORDS : break;
        default : break;
    }
}

void MenuController::draw() {
    switch(step) {
        case MENU_MAIN : main.draw(); break;
        case MENU_OPTIONS : options.draw(); break;
        case MENU_RECORDS : records.draw(); break;
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

void MenuController::setStep(MenuStep newStep) {
    nextStep = newStep;
    transition.start();
}

void MenuController::setInternalStep(MenuStep newStep) {
    step = newStep;
    switch(step) {
        case MENU_MAIN : main.launch(); break;
        case MENU_OPTIONS : options.launch(); break;
        case MENU_RECORDS : records.launch(); break;
        default : break;
    }
}

RecordsController* MenuController::getRecordsController() {
    return &records;
}

OptionsController* MenuController::getOptionsController() {
    return &options;
}
