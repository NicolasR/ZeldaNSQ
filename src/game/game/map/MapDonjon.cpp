#include "MapDonjon.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"
#include "../../../engine/texts/TextManager.h"

#include "../../../config/ConfigurationManager.h"

#include "../../MainController.h"

MapDonjon::MapDonjon() : anim(0), animMax(1), vanim(480), started(false), xLink(0), yLink(0), offsetSkin(0), surface(0), surface2(0) {
    wasFrench = ConfigurationManager::getInstance()->isFrench();
    if (wasFrench) {
        imageMap = ResourceManager::getInstance()->loadImage("data/images/maps/donjon.png");
        imageLevel = ResourceManager::getInstance()->loadImage("data/images/maps/level.png");
        imageLevel2 = ResourceManager::getInstance()->loadImage("data/images/maps/level2.png");
    } else {
        imageMap = ResourceManager::getInstance()->loadImage("data/images/maps/donjon_us.png");
        imageLevel = ResourceManager::getInstance()->loadImage("data/images/maps/level_us.png");
        imageLevel2 = ResourceManager::getInstance()->loadImage("data/images/maps/level2_us.png");
    }

    imageLink = ResourceManager::getInstance()->loadImage("data/images/maps/link.png", true);
    imageBoussole = ResourceManager::getInstance()->loadImage("data/images/maps/boussole.png", true);

    for (int i = 0; i < 5; i++) imageEtages[i] = 0;
    for (int i = 0; i < 5; i++) imageEtagesN[i] = 0;
    previousMapId = -1;
}

MapDonjon::~MapDonjon() {
    delete surface;
    delete surface2;
    ResourceManager::getInstance()->free(imageMap);
    ResourceManager::getInstance()->free(imageLevel);
    ResourceManager::getInstance()->free(imageLevel2);
    ResourceManager::getInstance()->free(imageLink);
    ResourceManager::getInstance()->free(imageBoussole);
    for (int i = 0; i < 5; i++) ResourceManager::getInstance()->free(imageEtages[i]);
    for (int i = 0; i < 5; i++) ResourceManager::getInstance()->free(imageEtagesN[i]);
}

void MapDonjon::handleActions(Action* action) {
    if (!started) return;
    if (action->isAction(ENTER) || action->isAction(DISPLAY_MAP) || action->isAction(QUIT_GAME)) {
        started = false;
        MainController::getInstance()->getGameController()->hideMap();
        AudioManager::getInstance()->playSound(TS_MENU2);
        return;
    }

    if (MainController::getInstance()->getGameController()->getSceneController()->getScene()->getCoffre(mapId - 57, 0)) {

        int old = etage;
        int min = getEtageMin();
        int max = getEtageMax();

        if (action->isAction(PUSH_UP)) {
            if (++etage > max) etage = min;
        }
        if (action->isAction(PUSH_DOWN)) {
            if (--etage < min) etage = max;
        }

        if (old != etage) {
            AudioManager::getInstance()->playSound(TS_MENU3);
            prepareMap();
            anim = 0;
            chrono.reset();
        }

    }
}

void MapDonjon::loop() {
    if (!started) return;
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void MapDonjon::draw() {
    WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    if (scene->getCoffre(mapId - 57, 0)) {
        WindowManager::getInstance()->draw(surface2->toImage(), 0, 0, 160, 160, 128, 48);

        ItemHelper::getInstance()->drawItem(TI_CARTE_DONJON, 28 + 8, 192 + 8);

        int min = getEtageMin();
        int max = getEtageMax();
        WindowManager::getInstance()->draw(imageLevel, 0, (2 - max) * 16, 32, (max - min + 1) * 16, 40, 64 + (2 - max) * 16);
        WindowManager::getInstance()->draw(imageLevel2, 0, (2 - etage) * 16, 32, 16, 40, 64 + (2 - etage) * 16);

        if (anim == 0 && scene->getMap()->getEtage() == etage) {
            WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, xLink - 6, yLink - 4);
        }
    }
    if (scene->getCoffre(mapId - 57, 1)) {
        ItemHelper::getInstance()->drawItem(TI_BOUSSOLE, 48 + 8, 192 + 8);
    }
    if (scene->getCoffre(mapId - 57, 2)) {
        ItemHelper::getInstance()->drawItem(TI_CLE_BOSS, 68 + 8, 192 + 8);
    }
}

void MapDonjon::launch() {

    if (ConfigurationManager::getInstance()->isFrench() != wasFrench) {
        wasFrench = !wasFrench;
        ResourceManager::getInstance()->free(imageMap);
        ResourceManager::getInstance()->free(imageLevel);
        ResourceManager::getInstance()->free(imageLevel2);
        if (wasFrench) {
            imageMap = ResourceManager::getInstance()->loadImage("data/images/maps/donjon.png");
            imageLevel = ResourceManager::getInstance()->loadImage("data/images/maps/level.png");
            imageLevel2 = ResourceManager::getInstance()->loadImage("data/images/maps/level2.png");
        } else {
            imageMap = ResourceManager::getInstance()->loadImage("data/images/maps/donjon_us.png");
            imageLevel = ResourceManager::getInstance()->loadImage("data/images/maps/level_us.png");
            imageLevel2 = ResourceManager::getInstance()->loadImage("data/images/maps/level2_us.png");
        }
    }

    offsetSkin = ConfigurationManager::getInstance()->getSkin() * 15;

    delete surface;
    surface = new Surface(320, 240);

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    mapId = scene->getMap()->getId();
    etage = scene->getMap()->getEtage();

    surface->put(imageMap, 0, 0, 0, 0, 320, 240);

    TextManager::getInstance()->getText(603 + mapId - 58)->displayOnSurface(120 + 2, 24 + 2, surface);

    if (scene->getCoffre(mapId - 57, 0)) {
        surface->put(imageLink, 24 + 1, 64 + (2 - etage) * 16 + 2, 0, offsetSkin, 14, 12);

        if (scene->getCoffre(mapId - 57, 1) && bossIsAlive()) {
            surface->put(imageBoussole, 72 + 4, 64 + (2 - getEtageBoss()) * 16 + 4, 2, 0, 8, 8);
        }

        loadImages();

        prepareMap();
    }

    anim = 0;
    chrono.reset();

    started = true;
}

void MapDonjon::prepareMap() {
    delete surface2;
    surface2 = new Surface(160, 160);
    surface2->put(imageMap, 0, 0, 128, 48, 160, 160);

    int w = getWidth();
    int h = getHeight();
    int dx = getOffsetX();
    int dy = getOffsetY();

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            surface2->put(scene->getRooms(mapId - 58, i + dx, j + dy) ? imageEtages[2 - etage] : imageEtagesN[2 - etage],
                          (((10 - w) / 2) + i) * 16, (((10 - h) / 2) + j) * 16, i * 16, j * 16, 16, 16);
        }
    }

    if (scene->getMap()->getEtage() == etage) {
        Link* link = scene->getLink();
        xLink = 128 + (((10 - w) / 2)) * 16 + ((link->getX() - getOffsetX() * 320) / 20);
        yLink = 48 + (((10 - h) / 2)) * 16 + ((link->getY() - getOffsetY() * 240) / 15);
        if (mapId == 61) {
            xLink = 128 + (((10 - w) / 2)) * 16 + (((link->getX() % (320*7)) - getOffsetX() * 320) / 20);
        }
    }

    if (scene->getCoffre(mapId - 57, 1)) {
        switch (mapId) {
            case 58 :
                if (etage == 0) {
                    if (!scene->getCoffre(mapId - 57, 4)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 2)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 5)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7 + 8, (((10 - h) / 2) + 3) * 16 + 7 - 4, 0, 0, 2, 2);
                    }
                } else if (etage == -1) {
                    if (!scene->getCoffre(mapId - 57, 6)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7, (((10 - h) / 2) + 1) * 16 + 7 + 2, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 7)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7, (((10 - h) / 2) + 3) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 8)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7 + 8, (((10 - h) / 2) + 1) * 16 + 7 + 6, 0, 0, 2, 2);
                    }
                } else if (etage == -2) {
                    if (bossIsAlive()) { // boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 4, (((10 - h) / 2) + 1) * 16 + 4, 2, 0, 8, 8);
                    }
                    if (!scene->getCoffre(mapId - 57, 31)) { // mini boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 5, (((10 - h) / 2) + 4) * 16 + 5, 10, 0, 6, 6);
                    }
                    if (!scene->getCoffre(mapId - 57, 9)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7 - 1, (((10 - h) / 2) + 1) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 10)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7 + 4, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getLink()->getInventory()->hasMedaillons(0)) { // medaillon
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getLink()->getInventory()->hasObject(BOOMERANG)) { // boomerang
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                }
                break;
            case 59 :
                if (etage == 0) {
                    if (!scene->getCoffre(mapId - 57, 5)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 1) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 7)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 11)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 7 + 4, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == -1) {
                    if (!scene->getCoffre(mapId - 57, 2)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 10)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == -2) {
                    if (bossIsAlive()) { // boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 4, (((10 - h) / 2) + 2) * 16 + 4, 2, 0, 8, 8);
                    }
                    if (!scene->getCoffre(mapId - 57, 33)) { // mini boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 6) * 16 + 5, (((10 - h) / 2) + 4) * 16 + 5, 10, 0, 6, 6);
                    }
                    if (!scene->getLink()->getInventory()->hasMedaillons(1)) { // medaillon
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 1) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getLink()->getInventory()->hasObject(GANTS)) { // gants
                        surface2->put(imageBoussole, (((10 - w) / 2) + 6) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 4)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 6)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 8)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7 + 4, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 12)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 6) * 16 + 7, (((10 - h) / 2) + 1) * 16 + 7, 0, 0, 2, 2);
                    }
                }
                break;
            case 60 :
                if (etage == 2) {
                    if (!scene->getCoffre(mapId - 57, 6)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == 1) {
                    if (!scene->getCoffre(mapId - 57, 4)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 1) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == 0) {
                    if (bossIsAlive()) { // boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 4, (((10 - h) / 2) + 1) * 16 + 4, 2, 0, 8, 8);
                    }
                    if (!scene->getLink()->getInventory()->hasMedaillons(2)) { // medaillon
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 5)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 7)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7 + 4, 0, 0, 2, 2);
                    }
                } else if (etage == -1) {
                    if (!scene->getCoffre(mapId - 57, 28)) { // mini boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 5, (((10 - h) / 2) + 0) * 16 + 5, 10, 0, 6, 6);
                    }
                    if (!scene->getLink()->getInventory()->hasObject(ARC)) { // arc
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 2)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 8)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7 + 4, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 9)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 7, (((10 - h) / 2) + 6) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 10)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 6) * 16 + 7, (((10 - h) / 2) + 3) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 11)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 6) * 16 + 7, (((10 - h) / 2) + 6) * 16 + 7, 0, 0, 2, 2);
                    }
                }
                break;
            case 61 :
                if (etage == -1) {
                    if (!scene->getCoffre(mapId - 57, 4)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 5)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7 + 2, (((10 - h) / 2) + 3) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 6)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 7)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7 + 4, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 8)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 10)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 7 + 4, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 11)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 7, (((10 - h) / 2) + 3) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 14)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 6) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == -2) {
                    if (!scene->getCoffre(mapId - 57, 37)) { // mini boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 5, (((10 - h) / 2) + 1) * 16 + 5, 10, 0, 6, 6);
                    }
                    if (bossIsAlive()) { // boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 4, (((10 - h) / 2) + 1) * 16 + 4, 2, 0, 8, 8);
                    }
                    if (!scene->getLink()->getInventory()->hasMedaillons(3)) { // medaillon
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getLink()->getInventory()->hasObject(GRAPPIN)) { // grappin
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 2)) { // clé boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 7, (((10 - h) / 2) + 1) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 9)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7 + 6, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 13)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 6) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                }
                break;
            case 62 :
                if (etage == 0) {
                    if (!scene->getCoffre(mapId - 57, 6)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 10)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == -1) {
                    if (!scene->getCoffre(mapId - 57, 47)) { // mini boss 1
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 5, (((10 - h) / 2) + 1) * 16 + 5, 10, 0, 6, 6);
                    }
                    if (!scene->getCoffre(mapId - 57, 48)) { // mini boss 2 a
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 5, (((10 - h) / 2) + 1) * 16 + 5, 10, 0, 6, 6);
                    }
                    if (!scene->getLink()->getInventory()->hasObject(BAGUETTE_FEU)) { // baguette de feu
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 2)) { // clé boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7 + 4, (((10 - h) / 2) + 1) * 16 + 7 - 4, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 5)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 12)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 13)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == -2) {
                    if (!scene->getCoffre(mapId - 57, 49) && scene->getCoffre(mapId - 57, 48)) { // mini boss 2 b
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 5, (((10 - h) / 2) + 1) * 16 + 5, 10, 0, 6, 6);
                    }
                    if (bossIsAlive()) { // boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 4, (((10 - h) / 2) + 2) * 16 + 4, 2, 0, 8, 8);
                    }
                    if (!scene->getLink()->getInventory()->hasMedaillons(4)) { // medaillon
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7, (((10 - h) / 2) + 1) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getLink()->getInventory()->hasObject(CANNE_SOMARIA)) { // canne de somaria
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 4)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 7)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 8)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 9)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 11)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                }
                break;
            case 63 :
                if (etage == 0) {
                    if (!scene->getCoffre(mapId - 57, 5)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == -1) {
                    if (!scene->getCoffre(mapId - 57, 27)) { // mini boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 5, (((10 - h) / 2) + 1) * 16 + 5, 10, 0, 6, 6);
                    }
                    if (!scene->getLink()->getInventory()->hasObject(CAPE_INVISIBILITE)) { // cape d'invisibilité
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == -2) {
                    if (bossIsAlive()) { // boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 4, (((10 - h) / 2) + 1) * 16 + 4, 2, 0, 8, 8);
                    }
                    if (!scene->getLink()->getInventory()->hasMedaillons(5)) { // medaillon
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 2)) { // clé boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 4)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 3) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 6)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 7)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 1) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 8)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 7, (((10 - h) / 2) + 3) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 28)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7 - 4, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                }
                break;
            case 64 :
                if (etage == 0) {
                    if (!scene->getCoffre(mapId - 57, 2)) { // clé boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 11)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7 - 4, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 13)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 6) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 14)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 6) * 16 + 7, (((10 - h) / 2) + 3) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == -1) {
                    if (!scene->getCoffre(mapId - 57, 49)) { // mini boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 5, (((10 - h) / 2) + 1) * 16 + 5, 10, 0, 6, 6);
                    }
                    if (!scene->getLink()->getInventory()->hasObject(BAGUETTE_GLACE)) { // baguette de glace
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 4)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 1) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 7)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7 + 4, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                } else if (etage == -2) {
                    if (bossIsAlive()) { // boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 4, (((10 - h) / 2) + 1) * 16 + 4, 2, 0, 8, 8);
                    }
                    if (!scene->getLink()->getInventory()->hasMedaillons(6)) { // medaillon
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 5)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7 - 2, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 6)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7 + 4, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 8)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 7 - 4, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 9)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 5) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 10)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 6) * 16 + 7, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                }
                break;
            case 65 :
                if (etage == 0) {
                    if (bossIsAlive()) { // boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 4, (((10 - h) / 2) + 1) * 16 + 4, 2, 0, 8, 8);
                    }
                    if (!scene->getCoffre(mapId - 57, 4)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7 - 4, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 5)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 1) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 6)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7 - 4, (((10 - h) / 2) + 4) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 7)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 8)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 9)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 10)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 4) * 16 + 7, (((10 - h) / 2) + 3) * 16 + 7 + 4, 0, 0, 2, 2);
                    }
                } else if (etage == 1) {
                    if (!scene->getCoffre(mapId - 57, 2)) { // clé boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 2) * 16 + 7, (((10 - h) / 2) + 2) * 16 + 7, 0, 0, 2, 2);
                    }
                }
                break;
            case 66 :
                if (etage == -2) {
                    if (bossIsAlive()) { // boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 1) * 16 + 4, (((10 - h) / 2) + 1) * 16 + 4, 2, 0, 8, 8);
                    }
                    if (!scene->getCoffre(mapId - 57, 2)) { // clé boss
                        surface2->put(imageBoussole, (((10 - w) / 2) + 0) * 16 + 7, (((10 - h) / 2) + 3) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 6)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 0) * 16 + 7, 0, 0, 2, 2);
                    }
                    if (!scene->getCoffre(mapId - 57, 7)) {
                        surface2->put(imageBoussole, (((10 - w) / 2) + 3) * 16 + 7, (((10 - h) / 2) + 3) * 16 + 7, 0, 0, 2, 2);
                    }
                }
                break;
            default : return;
        }
    }
}

int MapDonjon::getWidth() {
    switch (mapId) {
        case 58 : return 5;
        case 59 : case 64 : return 7;
        case 60 : return 1 + (2-etage) * 2;
        case 61 :
            if (etage == 0) return 5;
            return 7;
        case 62 : case 63 : return 6;
        case 65 : return 5 - etage * 2;
        case 66 : return 4;
        default : return 0;
    }
}

int MapDonjon::getHeight() {
    switch (mapId) {
        case 58 : case 59 : case 61 : case 62 : case 63 : case 64 : return 5;
        case 60 : return 1 + (2-etage) * 2;
        case 65 : return 5 - etage * 2;
        case 66 : return 4;
        default : return 0;
    }
}

int MapDonjon::getOffsetX() {
    switch (mapId) {
        case 58 : return etage * (-5);
        case 59 : case 62 : case 63 : return 0;
        case 60 :
            if (etage == -1) return 5;
            if (etage == 2) return 3;
            return 0;
        case 61 :
            if (etage == 0) return 1;
            return 0;
        case 64 :
            if (etage == -2 || etage == -1) return 0;
            return 7;
        case 65 : return etage * 6;
        case 66 : return etage * (-4);
        default : return 0;
    }
}

int MapDonjon::getOffsetY() {
    switch (mapId) {
        case 58 : case 66 : return 0;
        case 59 : case 61 : case 62 : case 63 : return 10 + etage * 5;
        case 60 :
            if (etage == -1) return 1;
            if (etage == 0) return 3;
            return 0;
        case 64 :
            if (etage == -2 || etage == 0) return 0;
            return 5;
        case 65 : return etage;
        default : return 0;
    }
}

void MapDonjon::loadImages() {
    if (previousMapId != mapId) {
        for (int i = 0; i < 5; i++) {
            ResourceManager::getInstance()->free(imageEtages[i]);
            imageEtages[i] = 0;
            ResourceManager::getInstance()->free(imageEtagesN[i]);
            imageEtagesN[i] = 0;
        }

        switch (mapId) {
            case 58 :
                imageEtages[2] = ResourceManager::getInstance()->loadImage("data/images/maps/d1e0.png");
                imageEtages[3] = ResourceManager::getInstance()->loadImage("data/images/maps/d1e-1.png");
                imageEtages[4] = ResourceManager::getInstance()->loadImage("data/images/maps/d1e-2.png");
                imageEtagesN[2] = ResourceManager::getInstance()->loadImage("data/images/maps/nd1e0.png");
                imageEtagesN[3] = ResourceManager::getInstance()->loadImage("data/images/maps/nd1e-1.png");
                imageEtagesN[4] = ResourceManager::getInstance()->loadImage("data/images/maps/nd1e-2.png");
                break;
            case 59 :
                imageEtages[2] = ResourceManager::getInstance()->loadImage("data/images/maps/d2e0.png");
                imageEtages[3] = ResourceManager::getInstance()->loadImage("data/images/maps/d2e-1.png");
                imageEtages[4] = ResourceManager::getInstance()->loadImage("data/images/maps/d2e-2.png");
                imageEtagesN[2] = ResourceManager::getInstance()->loadImage("data/images/maps/nd2e0.png");
                imageEtagesN[3] = ResourceManager::getInstance()->loadImage("data/images/maps/nd2e-1.png");
                imageEtagesN[4] = ResourceManager::getInstance()->loadImage("data/images/maps/nd2e-2.png");
                break;
            case 60 :
                imageEtages[0] = ResourceManager::getInstance()->loadImage("data/images/maps/d3e2.png");
                imageEtages[1] = ResourceManager::getInstance()->loadImage("data/images/maps/d3e1.png");
                imageEtages[2] = ResourceManager::getInstance()->loadImage("data/images/maps/d3e0.png");
                imageEtages[3] = ResourceManager::getInstance()->loadImage("data/images/maps/d3e-1.png");
                imageEtagesN[0] = ResourceManager::getInstance()->loadImage("data/images/maps/nd3e2.png");
                imageEtagesN[1] = ResourceManager::getInstance()->loadImage("data/images/maps/nd3e1.png");
                imageEtagesN[2] = ResourceManager::getInstance()->loadImage("data/images/maps/nd3e0.png");
                imageEtagesN[3] = ResourceManager::getInstance()->loadImage("data/images/maps/nd3e-1.png");
                break;
            case 61 :
                imageEtages[2] = ResourceManager::getInstance()->loadImage("data/images/maps/d4e0.png");
                imageEtages[3] = ResourceManager::getInstance()->loadImage("data/images/maps/d4e-1.png");
                imageEtages[4] = ResourceManager::getInstance()->loadImage("data/images/maps/d4e-2.png");
                imageEtagesN[2] = ResourceManager::getInstance()->loadImage("data/images/maps/nd4e0.png");
                imageEtagesN[3] = ResourceManager::getInstance()->loadImage("data/images/maps/nd4e-1.png");
                imageEtagesN[4] = ResourceManager::getInstance()->loadImage("data/images/maps/nd4e-2.png");
                break;
            case 62 :
                imageEtages[2] = ResourceManager::getInstance()->loadImage("data/images/maps/d5e0.png");
                imageEtages[3] = ResourceManager::getInstance()->loadImage("data/images/maps/d5e-1.png");
                imageEtages[4] = ResourceManager::getInstance()->loadImage("data/images/maps/d5e-2.png");
                imageEtagesN[2] = ResourceManager::getInstance()->loadImage("data/images/maps/nd5e0.png");
                imageEtagesN[3] = ResourceManager::getInstance()->loadImage("data/images/maps/nd5e-1.png");
                imageEtagesN[4] = ResourceManager::getInstance()->loadImage("data/images/maps/nd5e-2.png");
                break;
            case 63 :
                imageEtages[2] = ResourceManager::getInstance()->loadImage("data/images/maps/d6e0.png");
                imageEtages[3] = ResourceManager::getInstance()->loadImage("data/images/maps/d6e-1.png");
                imageEtages[4] = ResourceManager::getInstance()->loadImage("data/images/maps/d6e-2.png");
                imageEtagesN[2] = ResourceManager::getInstance()->loadImage("data/images/maps/nd6e0.png");
                imageEtagesN[3] = ResourceManager::getInstance()->loadImage("data/images/maps/nd6e-1.png");
                imageEtagesN[4] = ResourceManager::getInstance()->loadImage("data/images/maps/nd6e-2.png");
                break;
            case 64 :
                imageEtages[1] = ResourceManager::getInstance()->loadImage("data/images/maps/d7e1.png");
                imageEtages[2] = ResourceManager::getInstance()->loadImage("data/images/maps/d7e0.png");
                imageEtages[3] = ResourceManager::getInstance()->loadImage("data/images/maps/d7e-1.png");
                imageEtages[4] = ResourceManager::getInstance()->loadImage("data/images/maps/d7e-2.png");
                imageEtagesN[1] = ResourceManager::getInstance()->loadImage("data/images/maps/nd7e1.png");
                imageEtagesN[2] = ResourceManager::getInstance()->loadImage("data/images/maps/nd7e0.png");
                imageEtagesN[3] = ResourceManager::getInstance()->loadImage("data/images/maps/nd7e-1.png");
                imageEtagesN[4] = ResourceManager::getInstance()->loadImage("data/images/maps/nd7e-2.png");
                break;
            case 65 :
                imageEtages[1] = ResourceManager::getInstance()->loadImage("data/images/maps/d8e1.png");
                imageEtages[2] = ResourceManager::getInstance()->loadImage("data/images/maps/d8e0.png");
                imageEtagesN[1] = ResourceManager::getInstance()->loadImage("data/images/maps/nd8e1.png");
                imageEtagesN[2] = ResourceManager::getInstance()->loadImage("data/images/maps/nd8e0.png");
                break;
            case 66 :
                imageEtages[2] = ResourceManager::getInstance()->loadImage("data/images/maps/d9e0.png");
                imageEtages[3] = ResourceManager::getInstance()->loadImage("data/images/maps/d9e-1.png");
                imageEtages[4] = ResourceManager::getInstance()->loadImage("data/images/maps/d9e-2.png");
                imageEtagesN[2] = ResourceManager::getInstance()->loadImage("data/images/maps/nd9e0.png");
                imageEtagesN[3] = ResourceManager::getInstance()->loadImage("data/images/maps/nd9e-1.png");
                imageEtagesN[4] = ResourceManager::getInstance()->loadImage("data/images/maps/nd9e-2.png");
                break;
            default : return;
        }

        previousMapId = mapId;
    }
}

int MapDonjon::getEtageMin() {
    switch (mapId) {
        case 58 : case 59 : case 61 : case 62 : case 63 : case 64 : case 66 : return -2;
        case 60 : return -1;
        case 65 : return 0;
        default : return 0;
    }
}

int MapDonjon::getEtageMax() {
    switch (mapId) {
        case 58 : case 59 : case 61 : case 62 : case 63 : case 66 : return 0;
        case 60 : return 2;
        case 64 : case 65 : return 1;
        default : return 0;
    }
}

int MapDonjon::getEtageBoss() {
    switch (mapId) {
        case 58 : case 59 : case 61 : case 62 : case 63 : case 64 : case 66 : return -2;
        case 60 : case 65 : return 0;
        default : return 0;
    }
}

bool MapDonjon::bossIsAlive() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    switch (mapId) {
        case 58 : return !scene->getLink()->getInventory()->hasCoeur(5);
        case 59 : return !scene->getLink()->getInventory()->hasCoeur(6);
        case 60 : return !scene->getLink()->getInventory()->hasCoeur(7);
        case 61 : return !scene->getLink()->getInventory()->hasCoeur(8);
        case 62 : return !scene->getLink()->getInventory()->hasCoeur(9);
        case 63 : return !scene->getLink()->getInventory()->hasCoeur(10);
        case 64 : return !scene->getLink()->getInventory()->hasCoeur(11);
        case 65 : return !scene->getLink()->getInventory()->hasCoeur(12);
        case 66 : return true;
        default : return false;
    }
}
