#include "Help.h"

#include "../../MainController.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/audio/AudioManager.h"
#include "../../../engine/texts/TextManager.h"

#include "../../../config/ConfigurationManager.h"

#include "../../GlobalSave.h"

Help::Help() : page(0), surface(0), surfaceTmp(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/menus/cadres.png");
}

Help::~Help() {
    ResourceManager::getInstance()->free(image);
    delete surface;
    delete surfaceTmp;
}

void Help::init() {
    page = 0;

    delete surface;
    surface = new Surface(320, 240);

    delete surfaceTmp;
    surfaceTmp = new Surface(320, 240);

    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 20; i++) {
            surfaceTmp->put(image, i * 16, j * 16, 16, 16, 16, 16);
        }
    }

    cadre(16, 56, 288, 128, surfaceTmp);
    cadre(16, 200, 288, 32, surfaceTmp);

    refresh();
}

void Help::refresh() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    Inventory* inventory = link->getInventory();

    surface->put(surfaceTmp->toImage(), 0, 0, 0, 0, 320, 240);

    if (page == 0) {
        // title
        int letterSize = TextManager::getInstance()->getWSpace();
        Text* text = TextManager::getInstance()->getText(724);
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


        text = TextManager::getInstance()->getText(725);
        text->displayOnSurface(24, 64, surface);
        delete text;

        text = TextManager::getInstance()->getText(726);
        text->displayOnSurface(24, 80, surface);
        delete text;

        text = TextManager::getInstance()->getText(727);
        text->displayOnSurface(24, 96, surface);
        delete text;

        if (inventory->hasObject(BOTTES)) {
            text = TextManager::getInstance()->getText(728);
        } else {
            text = TextManager::getInstance()->getText(729);
        }
        text->displayOnSurface(24, 112, surface);
        delete text;

        if (link->getEpee()) {
            text = TextManager::getInstance()->getText(730);
        } else {
            text = TextManager::getInstance()->getText(731);
        }
        text->displayOnSurface(24, 128, surface);
        delete text;

        if (link->getEpee()) {
            text = TextManager::getInstance()->getText(732);
        } else {
            text = TextManager::getInstance()->getText(733);
        }
        text->displayOnSurface(24, 144, surface);
        delete text;

        text = TextManager::getInstance()->getText(734);
        text->displayOnSurface(24, 160, surface);
        delete text;

        text = TextManager::getInstance()->getText(735);
        text->displayOnSurface(24, 208, surface);
        delete text;
    } else if (page == 1) {
        // title
        int letterSize = TextManager::getInstance()->getWSpace();
        Text* text = TextManager::getInstance()->getText(736);
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


        text = TextManager::getInstance()->getText(737);
        text->displayOnSurface(24, 64, surface);
        delete text;

        if (inventory->hasObject(BOOMERANG)) {
            text = TextManager::getInstance()->getText(738);
        } else {
            text = TextManager::getInstance()->getText(739);
        }
        text->displayOnSurface(24, 80, surface);
        delete text;

        if (inventory->hasObject(CARTE_ILE)) {
            text = TextManager::getInstance()->getText(740);
        } else {
            text = TextManager::getInstance()->getText(741);
        }
        text->displayOnSurface(24, 96, surface);
        delete text;

        if (inventory->hasObject(SAC_TROC)) {
            text = TextManager::getInstance()->getText(742);
        } else {
            text = TextManager::getInstance()->getText(743);
        }
        text->displayOnSurface(24, 112, surface);
        delete text;

        if (GlobalSave::getInstance()->isOniLinkUnlocked()) {
            text = TextManager::getInstance()->getText(744);
        } else {
            text = TextManager::getInstance()->getText(745);
        }
        text->displayOnSurface(24, 128, surface);
        delete text;

        if (scene->getAvancement() >= AV_NAVI_IS_MOVABLE) {
            text = TextManager::getInstance()->getText(746);
        } else {
            text = TextManager::getInstance()->getText(747);
        }
        text->displayOnSurface(24, 144, surface);
        delete text;

        if (scene->getAvancement() >= AV_NAVI_CAN_HEAL) {
            text = TextManager::getInstance()->getText(748);
        } else {
            text = TextManager::getInstance()->getText(749);
        }
        text->displayOnSurface(24, 160, surface);
        delete text;

        text = TextManager::getInstance()->getText(750);
        text->displayOnSurface(24, 208, surface);
        delete text;
    } else if (page == 2) {
        // title
        int letterSize = TextManager::getInstance()->getWSpace();
        Text* text = TextManager::getInstance()->getText(751);
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


        if (inventory->hasObject(NAVI_ATT)) {
            text = TextManager::getInstance()->getText(752);
        } else {
            text = TextManager::getInstance()->getText(753);
        }
        text->displayOnSurface(24, 64, surface);
        delete text;

        if (inventory->hasObject(NAVI_DEF)) {
            text = TextManager::getInstance()->getText(754);
        } else {
            text = TextManager::getInstance()->getText(755);
        }
        text->displayOnSurface(24, 80, surface);
        delete text;

        if (inventory->hasObject(NAVI_ULT)) {
            text = TextManager::getInstance()->getText(756);
        } else {
            text = TextManager::getInstance()->getText(757);
        }
        text->displayOnSurface(24, 96, surface);
        delete text;

        text = TextManager::getInstance()->getText(758);
        text->displayOnSurface(24, 112, surface);
        delete text;

        text = TextManager::getInstance()->getText(759);
        text->displayOnSurface(24, 128, surface);
        delete text;

        text = TextManager::getInstance()->getText(760);
        text->displayOnSurface(24, 144, surface);
        delete text;

        text = TextManager::getInstance()->getText(761);
        text->displayOnSurface(24, 208, surface);
        delete text;
    }


}

void Help::cadre(int x, int y, int w, int h, Surface* surf) {

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

void Help::handleActions(Action* action) {
    if (action->isAction(ENTER) || action->isAction(AIDE)) {
        MainController::getInstance()->getGameController()->setStep(GAME_MAIN);
        AudioManager::getInstance()->playSound(TS_MENU2);
        return;
    }
    if (action->isAction(PUSH_LEFT) && page > 0) {
        page--;
        AudioManager::getInstance()->playSound(TS_MENU3);
        refresh();
        return;
    }
    if (action->isAction(PUSH_RIGHT) && page < 2) {
        page++;
        AudioManager::getInstance()->playSound(TS_MENU3);
        refresh();
        return;
    }
}

void Help::draw() {
    WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);
}
