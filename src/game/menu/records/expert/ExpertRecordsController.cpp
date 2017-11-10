#include "ExpertRecordsController.h"

ExpertRecordsController::ExpertRecordsController() {
}

ExpertRecordsController::~ExpertRecordsController() {
}

void ExpertRecordsController::launch() {
    expert.init();
}

void ExpertRecordsController::handleEvents(Event* event) {
    expert.handleEvents(event);
}

void ExpertRecordsController::draw() {
    expert.draw();
}
