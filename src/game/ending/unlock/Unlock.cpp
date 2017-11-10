#include "Unlock.h"

#include "../../MainController.h"
#include "../../GlobalSave.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/texts/TextManager.h"
#include "../../../engine/audio/AudioManager.h"

Unlock::Unlock() : surface(0) {
}

Unlock::~Unlock() {
    delete surface;
}

void Unlock::init() {

    GlobalSave* global = GlobalSave::getInstance();

    delete surface;
    surface = new Surface(320, 240);

    WImage* image = ResourceManager::getInstance()->loadImage("data/images/ending/fin.png");
    surface->put(image, 0, 0, 0, 0, 320, 240);
    ResourceManager::getInstance()->free(image);

    int letterSize = TextManager::getInstance()->getWSpace();

    // CONGRATULATIONS !
    Text* text = TextManager::getInstance()->getText(716);
    int size = text->getLength() * letterSize;
    text->displayOnSurface(160 - (size / 2), 10, surface);
    delete text;

    // UNLOCK :
    text = TextManager::getInstance()->getText(717);
    text->displayOnSurface(10, 50, surface);
    delete text;

    int line = 80;
    if (global->haveUnlockLink()) {
        text = TextManager::getInstance()->getText(718);
        text->displayOnSurface(10, line, surface);
        delete text;
        line+= 20;
        text = TextManager::getInstance()->getText(719);
        text->displayOnSurface(10, line, surface);
        delete text;
        line+= 30;
    }

    if (global->haveUnlockExpert()) {
        text = TextManager::getInstance()->getText(720);
        text->displayOnSurface(10, line, surface);
        delete text;
        line+= 20;
        text = TextManager::getInstance()->getText(721);
        text->displayOnSurface(10, line, surface);
        delete text;
        line+= 30;
    }

    if (global->haveUnlockOniLink()) {
        text = TextManager::getInstance()->getText(722);
        text->displayOnSurface(10, line, surface);
        delete text;
        line+= 20;
        text = TextManager::getInstance()->getText(723);
        text->displayOnSurface(10, line, surface);
        delete text;
        line+= 30;
    }

    global->resetUnlock();
}

void Unlock::handleEvents(Event* event) {
    if (event->isPushed(kReturn)) {
        AudioManager::getInstance()->playSound(TS_MENU1);
        MainController::getInstance()->setStep(LOGO);
    }
}

void Unlock::draw() {
    WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);
}
