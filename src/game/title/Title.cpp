#include "Title.h"

#include "../MainController.h"
#include "../../engine/window/WindowManager.h"
#include "../../engine/resources/ResourceManager.h"
#include "../../engine/audio/AudioManager.h"
#include "../../engine/texts/TextManager.h"

Title::Title() : animCiel(0), animMaxCiel(639), vanimCiel(128),
                anim(0), animMax(43), vanim(128),
                text(0), textSize(0), ready(false), alpha(0) {
    metronomeText = new Metronome(2, 1000);
    ostringstream os;
    for (int i = 0; i < 44; i++) {
        if (i < 10) os << "0";
        os << i;
        string filename = "data/images/title/fond/ET_" + os.str() + ".png";
        images[i] = ResourceManager::getInstance()->loadImage(filename, true);
        os.str("");
    }
    for (int i = 0; i < 23; i++) {
        if (i < 10) os << "0";
        os << i;
        string filename = "data/images/title/navi/navi_" + os.str() + ".png";
        navi[i] = ResourceManager::getInstance()->loadImage(filename, true);
        os.str("");
    }

    ciel = ResourceManager::getInstance()->loadImage("data/images/title/ciel.png");
    logo = ResourceManager::getInstance()->loadImage("data/images/title/titre.png", true);
}

Title::~Title() {
    delete metronomeText;
    for (int i = 0; i < 44; i++) {
        ResourceManager::getInstance()->free(images[i]);
    }
    for (int i = 0; i < 23; i++) {
        ResourceManager::getInstance()->free(navi[i]);
    }
    ResourceManager::getInstance()->free(ciel);
    ResourceManager::getInstance()->free(logo);
    delete text;
}

void Title::init() {
    AudioManager::getInstance()->playMusic(0);
    text = TextManager::getInstance()->getText(0);
    textSize = text->getLength() * TextManager::getInstance()->getWSpace();
    animCiel = 0;
    chronoCiel.reset();
    anim = 0;
    chrono.reset();
    ready = false;
    alpha = 0;
    logo->setAlpha(alpha);
}

void Title::handleEvents(Event* event) {
    if (event->isPushed(kReturn)) {
        if (ready) {
            AudioManager::getInstance()->playSound(TS_MENU1);
            AudioManager::getInstance()->stopMusic();
            MainController::getInstance()->setStep(MENU);
        } else {
            alpha = 255;
            logo->setAlpha(alpha);
            ready = true;
            metronomeText->reset();
        }
    }
}

void Title::loop() {
    metronomeText->loop();
    if (alpha > 0 && alpha < 255) {
        alpha+=4;
        if (alpha > 255) alpha = 255;
        logo->setAlpha(alpha);
        if (alpha == 255) {
            ready = true;
            metronomeText->reset();
        }
    }
    if (chronoCiel.getElapsedTime() >= vanimCiel) {
        animCiel++;
        if (animCiel > animMaxCiel) {
            animCiel = 0;
        }
        chronoCiel.reset();
    }
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (!alpha && anim == 22) alpha = 1;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Title::draw() {
    WindowManager::getInstance()->draw(ciel, animCiel, 0, 320, 240, 0, 0);
    WindowManager::getInstance()->draw(images[anim], 0, 0, 320, 240, 0, 0);

    if (!alpha) WindowManager::getInstance()->draw(navi[anim], 0, 0, 320, 240, 0, 0);
    if (alpha && !ready) WindowManager::getInstance()->draw(navi[22], 0, 0, 320, 240, 0, 0);
    if (alpha) WindowManager::getInstance()->draw(logo, 0, 0, 159, 128, 87, 45);


    if (ready && metronomeText->getValue()) text->display((320 - textSize) / 2, 200);
}
