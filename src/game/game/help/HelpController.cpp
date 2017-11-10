#include "HelpController.h"

HelpController::HelpController() {
}

HelpController::~HelpController() {
}

void HelpController::launch() {
    help.init();
}

void HelpController::handleActions(Action* action) {
    help.handleActions(action);
}

void HelpController::loop() {
}

void HelpController::draw() {
    help.draw();
}
