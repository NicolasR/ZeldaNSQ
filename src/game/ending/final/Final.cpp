#include "Final.h"

#include "../../MainController.h"
#include "../../GlobalSave.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/texts/TextManager.h"
#include "../../../engine/audio/AudioManager.h"

Final::Final() : anim(0), animMax(31), vanim(128) {
    ostringstream os;
    for (int i = 0; i < 32; i++) {
        if (i < 10) os << "0";
        os << i;
        string filename = "data/images/ending/final/final" + os.str() + ".png";
        images[i] = ResourceManager::getInstance()->loadImage(filename);
        os.str("");
    }
}

Final::~Final() {
    for (int i = 0; i < 32; i++) {
        ResourceManager::getInstance()->free(images[i]);
    }
}

void Final::init() {
    anim = 0;
    chrono.reset();
}

void Final::handleEvents(Event* event) {
    if (event->isPushed(kReturn)) {
        AudioManager::getInstance()->playSound(TS_MENU1);
        AudioManager::getInstance()->stopMusic();
        if (GlobalSave::getInstance()->haveUnlockLink()
            || GlobalSave::getInstance()->haveUnlockExpert()
            || GlobalSave::getInstance()->haveUnlockOniLink()) {
            MainController::getInstance()->getEndingController()->setStep(END_UNLOCK);
        } else {
            MainController::getInstance()->setStep(LOGO);
        }
    }
}

void Final::loop() {
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Final::draw() {
    WindowManager::getInstance()->draw(images[anim], 0, 0, 320, 240, 0, 0);
}
