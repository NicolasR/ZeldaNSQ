#include "GlobalOptions.h"

#include "../../../MainController.h"
#include "../../../../engine/window/WindowManager.h"
#include "../../../../engine/resources/ResourceManager.h"
#include "../../../../engine/audio/AudioManager.h"
#include "../../../../engine/texts/TextManager.h"

#include "../../../../config/ConfigurationManager.h"

#include "../../../GlobalSave.h"

GlobalOptions::GlobalOptions() : line(3), surface(0), surfaceTmp(0), skin(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/menus/cadres.png");
    link = ResourceManager::getInstance()->loadImage("data/images/menus/curseur.png", true);
}

GlobalOptions::~GlobalOptions() {
    ResourceManager::getInstance()->free(image);
    ResourceManager::getInstance()->free(link);
    delete surface;
    delete surfaceTmp;
}

void GlobalOptions::init() {

    delete surface;
    surface = new Surface(320, 240);

    delete surfaceTmp;
    surfaceTmp = new Surface(320, 240);

    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 20; i++) {
            surfaceTmp->put(image, i * 16, j * 16, 16, 16, 16, 16);
        }
    }

    cadre(16, 48, 288, 32, surfaceTmp);
    cadre(16, 96, 288, 32, surfaceTmp);
    cadre(16, 144, 288, 32, surfaceTmp);
    cadre(16, 192, 136, 32, surfaceTmp);

    refresh();
}

void GlobalOptions::refresh() {
    surface->put(surfaceTmp->toImage(), 0, 0, 0, 0, 320, 240);

    // title
    int letterSize = TextManager::getInstance()->getWSpace();
    Text* text = TextManager::getInstance()->getText(707);
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

    text = TextManager::getInstance()->getText(708);
    text->displayOnSurface(44, 56, surface);
    delete text;

    text = TextManager::getInstance()->getText(709);
    text->displayOnSurface(44, 104, surface);
    delete text;

    text = TextManager::getInstance()->getText(getTextForSkin());
    size = text->getLength() * letterSize;
    text->displayOnSurface(290 - size, 104, surface);
    delete text;

    text = TextManager::getInstance()->getText(710);
    text->displayOnSurface(44, 152, surface);
    delete text;

    text = TextManager::getInstance()->getText(694);
    size = text->getLength() * letterSize;
    text->displayOnSurface(16 + (136 - size) / 2, 200, surface);
    delete text;
}

int GlobalOptions::getTextForSkin() {
    skin = ConfigurationManager::getInstance()->getSkin();
    if (skin < 3) return 711 + skin;
    return 711;
}

void GlobalOptions::cadre(int x, int y, int w, int h, Surface* surf) {

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

void GlobalOptions::handleEvents(Event* event) {
    if (event->isPushed(kReturn)) {

        switch (line) {
            case 2 :
                MainController::getInstance()->getMenuController()->getOptionsController()->setStep(OPTIONS_AUDIO);
                AudioManager::getInstance()->playSound(TS_MENU1);
                break;
            case 3 :
                ConfigurationManager::getInstance()->save();
                MainController::getInstance()->getMenuController()->setStep(MENU_MAIN);
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
    if (event->isPushed(kLeft)) {
        if (line == 0) {
            ConfigurationManager* conf = ConfigurationManager::getInstance();
            conf->setLang(conf->getLang() == 0 ? 1 : 0);
            refresh();
            AudioManager::getInstance()->playSound(TS_MENU3);
        }
        if (line == 1) {
            ConfigurationManager* conf = ConfigurationManager::getInstance();
            skin = conf->getSkin();
            int maxSkin = GlobalSave::getInstance()->isHolydaysLinkUnlock() ? 2 : 1;
            if (--skin < 0) skin = maxSkin;
            conf->setSkin(skin);
            refresh();
            AudioManager::getInstance()->playSound(TS_MENU3);
        }
    }
    if (event->isPushed(kRight)) {
        if (line == 0) {
            ConfigurationManager* conf = ConfigurationManager::getInstance();
            conf->setLang(conf->getLang() == 0 ? 1 : 0);
            refresh();
            AudioManager::getInstance()->playSound(TS_MENU3);
        }
        if (line == 1) {
            ConfigurationManager* conf = ConfigurationManager::getInstance();
            skin = conf->getSkin();
            int maxSkin = GlobalSave::getInstance()->isHolydaysLinkUnlock() ? 2 : 1;
            if (++skin > maxSkin) skin = 0;
            conf->setSkin(skin);
            refresh();
            AudioManager::getInstance()->playSound(TS_MENU3);
        }
    }
}

void GlobalOptions::draw() {
    WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);

    WindowManager::getInstance()->draw(link, skin * 16, 0, 16, 21, 26, 53 + 48 * line);
}
