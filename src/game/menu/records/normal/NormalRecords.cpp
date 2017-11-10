#include "NormalRecords.h"

#include "../../../MainController.h"
#include "../../../../engine/window/WindowManager.h"
#include "../../../../engine/resources/ResourceManager.h"
#include "../../../../engine/audio/AudioManager.h"
#include "../../../../engine/texts/TextManager.h"
#include "../../../../config/ConfigurationManager.h"

NormalRecords::NormalRecords() : line(3), full(false), ulti(false), rush(false), surface(0), surfaceFull(0), surfaceUlti(0), surfaceRush(0), skin(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/menus/cadres.png");
    link = ResourceManager::getInstance()->loadImage("data/images/menus/curseur.png", true);
    rang = ResourceManager::getInstance()->loadImage("data/images/menus/rangs.png");
}

NormalRecords::~NormalRecords() {
    ResourceManager::getInstance()->free(image);
    ResourceManager::getInstance()->free(link);
    ResourceManager::getInstance()->free(rang);
    delete surface;
    delete surfaceFull;
    delete surfaceUlti;
    delete surfaceRush;
}

void NormalRecords::init() {

    globalSave = GlobalSave::getInstance();
    skin = ConfigurationManager::getInstance()->getSkin();

    delete surface;
    surface = new Surface(320, 240);

    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 20; i++) {
            surface->put(image, i * 16, j * 16, 16, 16, 16, 16);
        }
    }

    // title
    int letterSize = TextManager::getInstance()->getWSpace();
    Text* text = TextManager::getInstance()->getText((globalSave->isExpertUnlocked()) ? 698 : 697);
    int size = text->getLength() * letterSize;
    int blocs = (size + 15) / 16;

    int x = 16;
    surface->put(image, x, 0, 0, 0, 16, 16);
    surface->put(image, x, 16, 0, 16, 16, 16);
    surface->put(image, x, 32, 0, 32, 16, 16);

    for (int i = 0; i <= blocs; i++) {
        x += 16;
        surface->put(image, x, 0, 16, 0, 16, 16);
        surface->put(image, x, 16, 16, 64, 16, 16);
        surface->put(image, x, 32, 16, 32, 16, 16);
    }

    x += 16;
    surface->put(image, x, 0, 32, 0, 16, 16);
    surface->put(image, x, 16, 32, 16, 16, 16);
    surface->put(image, x, 32, 32, 32, 16, 16);

    text->displayOnSurface(32 + 8 + ((blocs * 16) - size) / 2, 16, surface);

    delete text;

    cadre(128, 8, 176, 32, surface);
    cadre(16, 48, 240, 32, surface); cadre(272, 48, 32, 32, surface);
    cadre(16, 96, 240, 32, surface); cadre(272, 96, 32, 32, surface);
    cadre(16, 144, 240, 32, surface); cadre(272, 144, 32, 32, surface);
    cadre(16, 192, 136, 32, surface);

    // best time :
    text = TextManager::getInstance()->getText(700);
    size = text->getLength() * letterSize;
    text->displayOnSurface(140, 16, surface);
    delete text;

    int time = globalSave->getBestNormalTime();
    if (time == -1) {
        text = new Text("??:??:??");
    } else {
        ostringstream os;
        int hours = time / 3600;
        int minutes = (time % 3600) / 60;
        int seconds = time % 60;
        if (hours < 10) os << "0";
        os << hours << ":";
        if (minutes < 10) os << "0";
        os << minutes << ":";
        if (seconds < 10) os << "0";
        os << seconds;
        text = new Text(os.str());
    }
    text->displayOnSurface(140 + size + letterSize, 16, surface);
    delete text;

    // 100%
    text = TextManager::getInstance()->getText(701);
    text->displayOnSurface(44, 56, surface);
    delete text;
    surface->put(rang, 280, 56, globalSave->isNormalFull() ? 0 : 16, 0, 16, 16);

    // ultime
    text = TextManager::getInstance()->getText(702);
    text->displayOnSurface(44, 104, surface);
    delete text;
    surface->put(rang, 280, 104, globalSave->isNormalUlti() ? 0 : 16, 0, 16, 16);

    // rapidite
    text = TextManager::getInstance()->getText(703);
    text->displayOnSurface(44, 152, surface);
    delete text;
    surface->put(rang, 280, 152, globalSave->isNormalRush() ? 0 : 16, 0, 16, 16);

    // back
    text = TextManager::getInstance()->getText(694);
    size = text->getLength() * letterSize;
    text->displayOnSurface(16 + (136 - size) / 2, 200, surface);
    delete text;

    delete surfaceFull;
    surfaceFull = new Surface(224, 40);
    cadre(0, 0, 224, 40, surfaceFull);
    text = TextManager::getInstance()->getText(704);
    size = text->getLength() * letterSize;
    text->displayOnSurface((224 - size) / 2, 9+2, surfaceFull);
    delete text;

    delete surfaceUlti;
    surfaceUlti = new Surface(224, 40);
    cadre(0, 0, 224, 40, surfaceUlti);
    text = TextManager::getInstance()->getText(705);
    size = text->getLength() * letterSize;
    text->displayOnSurface((224 - size) / 2, 9+2, surfaceUlti);
    delete text;

    delete surfaceRush;
    surfaceRush = new Surface(224, 40);
    cadre(0, 0, 224, 40, surfaceRush);
    text = TextManager::getInstance()->getText(706);
    size = text->getLength() * letterSize;
    text->displayOnSurface((224 - size) / 2, 9+2, surfaceRush);
    delete text;
}

void NormalRecords::cadre(int x, int y, int w, int h, Surface* surf) {

    // center
    for (int j = y + 16; j < y + h - 16; j += 16) {
        for (int i = x + 16; i < x + w - 16; i += 16) {
            surf->put(image, i, j, 16, 64, 16, 16);
        }
    }

    //top and bottom
    for (int i = x + 16; i < x + w - 16; i += 16) {
        surf->put(image, i, y, 16, 48, 16, 16);
        surf->put(image, i, y + h - 16, 16, 80, 16, 16);
    }

    //left and right
    for (int j = y + 16; j < y + h - 16; j += 16) {
        surf->put(image, x, j, 0, 64, 16, 16);
        surf->put(image, x + w - 16, j, 32, 64, 16, 16);
    }

    //top left corner
    surf->put(image, x, y, 0, 48, 16, 16);

    //top right corner
    surf->put(image, x + w - 16, y, 32, 48, 16, 16);

    //bottom left corner
    surf->put(image, x, y + h - 16, 0, 80, 16, 16);

    //bottom right corner
    surf->put(image, x + w - 16, y + h - 16, 32, 80, 16, 16);
}

void NormalRecords::handleEvents(Event* event) {
    if (event->isPushed(kReturn)) {
        switch (line) {
            case 0 :
                if (full) {
                    full = false;
                    AudioManager::getInstance()->playSound(TS_MENU2);
                } else {
                    full = true;
                    AudioManager::getInstance()->playSound(TS_MENU1);
                }
                break;
            case 1 :
                if (ulti) {
                    ulti = false;
                    AudioManager::getInstance()->playSound(TS_MENU2);
                } else {
                    ulti = true;
                    AudioManager::getInstance()->playSound(TS_MENU1);
                }
                break;
                break;
            case 2 :
                if (rush) {
                    rush = false;
                    AudioManager::getInstance()->playSound(TS_MENU2);
                } else {
                    rush = true;
                    AudioManager::getInstance()->playSound(TS_MENU1);
                }
                break;
                break;
            case 3 :
                MainController::getInstance()->getMenuController()->getRecordsController()->setStep(RECORDS_MAIN);
                AudioManager::getInstance()->playSound(TS_MENU2);
                break;
        }
        return;
    }

    if (event->isPushed(kUp)) {
        line--;
        if (line < 0) line = 3;
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
    if (event->isPushed(kDown)) {
        line++;
        if (line > 3) line = 0;
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
}

void NormalRecords::draw() {
    WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);

    WindowManager::getInstance()->draw(link, skin * 16, 0, 16, 21, 26, 53 + 48 * line);

    if (full) {
        WindowManager::getInstance()->draw(surfaceFull->toImage(), 0, 0, 224, 40, 48, 92);
    } else if (ulti) {
        WindowManager::getInstance()->draw(surfaceUlti->toImage(), 0, 0, 224, 40, 48, 92);
    } else if (rush) {
        WindowManager::getInstance()->draw(surfaceRush->toImage(), 0, 0, 224, 40, 48, 92);
    }
}
