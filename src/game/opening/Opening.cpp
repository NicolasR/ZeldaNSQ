#include "Opening.h"

#include "../MainController.h"
#include "../../engine/window/WindowManager.h"
#include "../../engine/resources/ResourceManager.h"
#include "../../engine/audio/AudioManager.h"
#include "../../engine/texts/TextManager.h"

Opening::Opening() : etape(0) {
    imageFond1 = ResourceManager::getInstance()->loadImage("data/images/logos/fond1.png", true);
    imageFond2 = ResourceManager::getInstance()->loadImage("data/images/logos/fond2.png");

    ostringstream oss;
    for (int i = 0; i < 6; i++) {
        oss.str(""); oss << (i + 1);
        imageIntro[i] = ResourceManager::getInstance()->loadImage(("data/images/logos/intro" + oss.str() + ".png").c_str());
    }


    metronome = new Metronome(32, 40);
    metronome->reset();
}

Opening::~Opening() {
    ResourceManager::getInstance()->free(imageFond1);
    ResourceManager::getInstance()->free(imageFond2);
    for (int i = 0; i < 6; i++) {
        ResourceManager::getInstance()->free(imageIntro[i]);
    }
}

void Opening::init() {
    etape = 0;
    metronome->reset();
    AudioManager::getInstance()->playMusic(6);
}

void Opening::loop() {
    metronome->loop();
}

void Opening::draw() {

    int anim = metronome->getValue();

    int srcX = 0;

    for (int j=0; j<240; j+=16) {
        for (int i=0; i<320; i+=16) {
            srcX = 16 - anim % 16;
            if (i > 16 && i < 288 && j > 144 && j < 224) {
                srcX += 32;
            }
            WindowManager::getInstance()->draw(imageFond2, srcX, anim % 16, 16, 16, i, j);
        }
    }

    for (int j = 0; j < 240; j += 32) {
        for (int i = 0; i < 320; i += 32) {
            srcX = anim;
            if (i > 16 && i < 288 && j > 144 && j < 224) {
                srcX += 64;
            }
            WindowManager::getInstance()->draw(imageFond1, srcX, anim, 32, 32, i, j);
        }
    }

    WindowManager::getInstance()->draw(imageIntro[etape], 0, 0, 320, 111, 0, 17);
}

void Opening::nextStep() {
    if (etape < 5) {
        etape++;
    }
}
