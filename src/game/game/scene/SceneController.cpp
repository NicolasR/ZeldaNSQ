#include "SceneController.h"

#include "../../MainController.h"

SceneController::SceneController() {
}

SceneController::~SceneController() {
}

void SceneController::launch(Save* sv) {
    scene.init(sv);
}

void SceneController::handleActions(Action* action) {
    scene.handleActions(action);
}

void SceneController::loop() {
    scene.loop();
}

void SceneController::draw() {
    scene.draw();
}

Scene* SceneController::getScene() {
    return &scene;
}

void SceneController::setStep(SceneStep s) {
    scene.setStep(s);
}

void SceneController::saveData() {
    scene.saveData();
}
