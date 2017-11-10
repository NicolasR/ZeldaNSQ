#include "Logo.h"

#include "../MainController.h"
#include "../../engine/window/WindowManager.h"
#include "../../engine/resources/ResourceManager.h"
#include "../../engine/audio/AudioManager.h"

#include "../../config/ConfigurationManager.h"

Logo::Logo() : anim(0), animMax(15), vanim(60), first(true) {
    wasFrench = ConfigurationManager::getInstance()->isFrench();
    if (wasFrench) {
        image = ResourceManager::getInstance()->loadImage("data/images/logos/logo_fond.png");
    } else {
        image = ResourceManager::getInstance()->loadImage("data/images/logos/logo_fond_us.png");
    }
    imageLogo = ResourceManager::getInstance()->loadImage("data/images/logos/logo.png");
}

Logo::~Logo() {
    ResourceManager::getInstance()->free(image);
    ResourceManager::getInstance()->free(imageLogo);
}

void Logo::init() {
    if (ConfigurationManager::getInstance()->isFrench() != wasFrench) {
        wasFrench = !wasFrench;
        ResourceManager::getInstance()->free(image);
        if (wasFrench) {
            image = ResourceManager::getInstance()->loadImage("data/images/logos/logo_fond.png");
        } else {
            image = ResourceManager::getInstance()->loadImage("data/images/logos/logo_fond_us.png");
        }
    }
    anim = 0;
    chrono.reset();
}

void Logo::handleEvents(Event* event) {
    if (anim < animMax) return;
    if (event->isPushed(kReturn)) {
        AudioManager::getInstance()->playSound(TS_MENU1);
        MainController::getInstance()->setStep(TITRE);
    }
}

void Logo::loop() {
    if (anim == animMax) return;
    if (first) {
        if (chrono.getElapsedTime() >= 2000) {
            first = false;
            chrono.reset();
        }
        return;
    }
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        chrono.reset();
    }
}

void Logo::draw() {
    if (first) return;
    WindowManager::getInstance()->draw(image, 0, 0, 320, 240, 0, 0);

    WindowManager::getInstance()->draw(imageLogo, 144 * (anim % 4), 144 * (anim / 4), 144, 144, 88, 60);

}
