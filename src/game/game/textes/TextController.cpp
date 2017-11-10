#include "TextController.h"

TextController::TextController() {
}

TextController::~TextController() {
}

void TextController::launch() {
    text.start();
}

void TextController::setTextId(int textId) {
    text.setTextId(textId);
}

void TextController::handleActions(Action* action) {
    text.handleActions(action);
}

void TextController::loop() {
    text.loop();
}

void TextController::draw() {
    text.draw();
}
