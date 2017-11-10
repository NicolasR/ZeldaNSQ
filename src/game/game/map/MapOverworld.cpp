#include "MapOverworld.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

#include "../../../config/ConfigurationManager.h"

MapOverworld::MapOverworld() : anim(0), animMax(1), vanim(480), started(false), xLink(0), yLink(0), offsetSkin(0) {
    imageMap = ResourceManager::getInstance()->loadImage("data/images/maps/overworld.png");
    imageLink = ResourceManager::getInstance()->loadImage("data/images/maps/link.png", true);
}

MapOverworld::~MapOverworld() {
    ResourceManager::getInstance()->free(imageMap);
    ResourceManager::getInstance()->free(imageLink);
}

void MapOverworld::handleActions(Action* action) {
    if (!started) return;
    if (action->isAction(ENTER) || action->isAction(DISPLAY_MAP) || action->isAction(QUIT_GAME)) {
        started = false;
        MainController::getInstance()->getGameController()->hideMap();
        AudioManager::getInstance()->playSound(TS_MENU2);
        return;
    }
}

void MapOverworld::loop() {
    if (!started) return;
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void MapOverworld::draw() {
    WindowManager::getInstance()->draw(imageMap, 0, 0, 320, 240, 0, 0);
    if (anim == 0) WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, xLink - 7, yLink - 8);
}

void MapOverworld::launch() {
    offsetSkin = ConfigurationManager::getInstance()->getSkin() * 15;
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    int mapId = scene->getMap()->getId();
    Link* link = scene->getLink();

    xLink = (getOffsetXForMap(mapId) * 20 * 16 + link->getX() + 8)/18;
    yLink = (getOffsetYForMap(mapId) * 15 * 16 + link->getY() + 24)/18;

    anim = 0;
    chrono.reset();

    started = true;
}

int MapOverworld::getOffsetXForMap(int mapId) {
    switch (mapId) {
        case 1 : return 0;
        case 2 : return 4;
        case 3 : return 6;
        case 4 : return 10;
        case 5 : return 10;
        case 6 : return 14;
        case 7 : return 14;
        case 8 : return 0;
        case 9 : return 7;
        case 10 : return 11;
        case 11 : return 8;
        case 12 : return 0;
        case 13 : return 4;
        case 14 : return 0;
        default : return -1;
    }
}

int MapOverworld::getOffsetYForMap(int mapId) {
    switch (mapId) {
        case 1 : return 14;
        case 2 : return 10;
        case 3 : return 14;
        case 4 : return 14;
        case 5 : return 10;
        case 6 : return 14;
        case 7 : return 6;
        case 8 : return 10;
        case 9 : return 6;
        case 10 : return 6;
        case 11 : return 0;
        case 12 : return 6;
        case 13 : return 6;
        case 14 : return 0;
        default : return -1;
    }
}
