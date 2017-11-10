#include "AudioOptions.h"

#include "../../../MainController.h"
#include "../../../../engine/window/WindowManager.h"
#include "../../../../engine/resources/ResourceManager.h"
#include "../../../../engine/audio/AudioManager.h"
#include "../../../../engine/texts/TextManager.h"

#include "../../../../config/ConfigurationManager.h"

#include "../../../GlobalSave.h"

AudioOptions::AudioOptions() : line(2), surface(0), skin(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/menus/cadres.png");
    link = ResourceManager::getInstance()->loadImage("data/images/menus/curseur.png", true);
    niveau = ResourceManager::getInstance()->loadImage("data/images/menus/niveau.png", true);
}

AudioOptions::~AudioOptions() {
    ResourceManager::getInstance()->free(image);
    ResourceManager::getInstance()->free(link);
    ResourceManager::getInstance()->free(niveau);
    delete surface;
}

void AudioOptions::init() {

    skin = ConfigurationManager::getInstance()->getSkin();
    volume = ConfigurationManager::getInstance()->getVolume();
    volson = ConfigurationManager::getInstance()->getVolson();

    delete surface;
    surface = new Surface(320, 240);

    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 20; i++) {
            surface->put(image, i * 16, j * 16, 16, 16, 16, 16);
        }
    }

    // title
    int letterSize = TextManager::getInstance()->getWSpace();
    Text* text = TextManager::getInstance()->getText(710);
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

    cadre(16, 48+16, 288, 32, surface);
    cadre(16, 144-16, 288, 32, surface);
    cadre(16, 192, 136, 32, surface);

    text = TextManager::getInstance()->getText(714);
    text->displayOnSurface(44, 56+16, surface);
    delete text;

    text = TextManager::getInstance()->getText(715);
    text->displayOnSurface(44, 104+32, surface);
    delete text;

    text = TextManager::getInstance()->getText(694);
    size = text->getLength() * letterSize;
    text->displayOnSurface(16 + (136 - size) / 2, 200, surface);
    delete text;

    surface->put(image, 128, 72, 0, 96, 16, 16);
    for (int i = 144; i < 264; i+=16) {
        surface->put(image, i, 72, 16, 96, 16, 16);
    }
    surface->put(image, 264, 72, 32, 96, 16, 16);

    surface->put(image, 128, 136, 0, 96, 16, 16);
    for (int i = 144; i < 264; i+=16) {
        surface->put(image, i, 136, 16, 96, 16, 16);
    }
    surface->put(image, 264, 136, 32, 96, 16, 16);
}

void AudioOptions::cadre(int x, int y, int w, int h, Surface* surf) {

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

void AudioOptions::handleEvents(Event* event) {
    if (event->isPushed(kReturn)) {
        if (line == 2) {
            ConfigurationManager::getInstance()->save();
            MainController::getInstance()->getMenuController()->getOptionsController()->setStep(OPTIONS_MAIN);
            AudioManager::getInstance()->playSound(TS_MENU2);
        }
        return;
    }

    if (event->isPushed(kUp)) {
        line--;
        if (line < 0) line = 2;
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
    if (event->isPushed(kDown)) {
        line++;
        if (line > 2) line = 0;
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
    if (event->isPushed(kLeft)) {
        if (line == 0 && volume > 0) {
            volume -= 16;
            ConfigurationManager::getInstance()->setVolume(volume);
            AudioManager::getInstance()->playSound(TS_MENU3);
        }
        if (line == 1 && volson > 0) {
            volson -= 16;
            ConfigurationManager::getInstance()->setVolson(volson);
            AudioManager::getInstance()->playSound(TS_MENU3);
        }
    }
    if (event->isPushed(kRight)) {
        if (line == 0 && volume < 128) {
            volume += 16;
            ConfigurationManager::getInstance()->setVolume(volume);
            AudioManager::getInstance()->playSound(TS_MENU3);
        }
        if (line == 1 && volson < 128) {
            volson += 16;
            ConfigurationManager::getInstance()->setVolson(volson);
            AudioManager::getInstance()->playSound(TS_MENU3);
        }
    }
}

void AudioOptions::draw() {
    WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);

    WindowManager::getInstance()->draw(link, skin * 16, 0, 16, 21, 26, 53 + 16 + 64 * line);

    WindowManager::getInstance()->draw(niveau, 0, 0, 8, 16, 136 + volume, 72);
    WindowManager::getInstance()->draw(niveau, 0, 0, 8, 16, 136 + volson, 136);
}
