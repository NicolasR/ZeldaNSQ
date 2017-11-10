#include "StoryTextArea.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"
#include "../../../engine/texts/TextManager.h"

#include "../../MainController.h"

StoryTextArea::StoryTextArea() : anim(0), animMax(0), vanim(40), ready(false), text(0), id(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/ending/fin.png");
}

StoryTextArea::~StoryTextArea() {
    ResourceManager::getInstance()->free(image);
    delete text;
}

void StoryTextArea::handleActions(Action* action) {
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

void StoryTextArea::loop() {
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

void StoryTextArea::draw() {
    WindowManager::getInstance()->draw(image, 0, 0, 320, 240, 0, 0);
    text->displayBox(8, 8, anim);
}

void StoryTextArea::init() {
    AudioManager::getInstance()->playMusic(39);
    setTextId(655);
    start();
}

void StoryTextArea::start() {
    chrono.reset();
    anim = 0;
    animMax = text->getLengthInBox();
    ready = false;
}

void StoryTextArea::stop() {
    AudioManager::getInstance()->playSound(TS_MENU1);
    MainController::getInstance()->getEndingController()->setStep(END_SCORE);
}

void StoryTextArea::setTextId(int textId) {
    delete text;
    text = TextManager::getInstance()->getText(textId);
    text->setBox(256 - 16 + 64, 64 - 16 + 158 + 32);
    id = textId;
}

bool StoryTextArea::hasLogicalNext() {
    int next = 0;
    if (id < 656) {
        next = id + 1;
    } else {
        return false;
    }
    setTextId(next);
    return true;
}
