#include "OpeningTextArea.h"

#include "../../engine/resources/ResourceManager.h"
#include "../../engine/window/WindowManager.h"
#include "../../engine/audio/AudioManager.h"
#include "../../engine/texts/TextManager.h"

#include "../MainController.h"

OpeningTextArea::OpeningTextArea() : anim(0), animMax(0), vanim(40), ready(false), text(0), id(0) {
}

OpeningTextArea::~OpeningTextArea() {
    delete text;
}

void OpeningTextArea::handleActions(Action* action) {
    if (action->isAction(ENTER)) {
        if (ready) {
            if (text->hasNext()) {
                text->next();
                start();
                AudioManager::getInstance()->playSound(TS_TEXTNEXT);
            } else if (hasLogicalNext()) {
                start();
                AudioManager::getInstance()->playSound(TS_TEXTNEXT);
            } else {
                stop();
                AudioManager::getInstance()->playSound(TS_TEXTEND);
            }
        } else {
            anim = animMax;
            ready = true;
        }
        return;
    }
}

void OpeningTextArea::loop() {
    if (!ready) {
        if (chrono.getElapsedTime() >= vanim) {
            do {
                ready = (++anim == animMax);
            } while (!ready && text->charAtInBox(anim) == ' ');
            AudioManager::getInstance()->playSound(TS_TEXT, 1);
            chrono.reset();
        }
    }
}

void OpeningTextArea::draw() {
    text->displayBox(32 + 8, 158 + 8, anim);
}

void OpeningTextArea::init() {
    setTextId(48);
    start();
}

void OpeningTextArea::start() {
    chrono.reset();
    anim = 0;
    animMax = text->getLengthInBox();
    ready = false;
}

void OpeningTextArea::stop() {
    MainController::getInstance()->setStep(GAME);
}

void OpeningTextArea::setTextId(int textId) {
    delete text;
    text = TextManager::getInstance()->getText(textId);
    text->setBox(256 - 16, 64 - 16);
    id = textId;
}

bool OpeningTextArea::hasLogicalNext() {
    int next = 0;
    if (id < 53) {
        next = id + 1;
    } else {
        return false;
    }
    setTextId(next);
    MainController::getInstance()->getOpeningController()->nextStep();
    return true;
}
