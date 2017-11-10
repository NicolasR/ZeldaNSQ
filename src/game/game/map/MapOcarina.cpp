#include "MapOcarina.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"
#include "../../../engine/texts/TextManager.h"

#include "../../MainController.h"

#include "../../../config/ConfigurationManager.h"

MapOcarina::MapOcarina() : started(false), teleport(false), position(0), offsetSkin(0) {
    imageMap = ResourceManager::getInstance()->loadImage("data/images/maps/overworld.png");
    imageLink = ResourceManager::getInstance()->loadImage("data/images/maps/link.png", true);
}

MapOcarina::~MapOcarina() {
    ResourceManager::getInstance()->free(imageMap);
    ResourceManager::getInstance()->free(imageLink);
}

void MapOcarina::handleActions(Action* action) {
    if (teleport || !started) return;
    if (action->isAction(ENTER)) {
        AudioManager::getInstance()->playSound(TS_TELEPORT, 2);
        teleport = true;
        return;
    }
    if (action->isAction(QUIT_GAME)) {
        started = false;
        MainController::getInstance()->getGameController()->hideMap();
        AudioManager::getInstance()->playSound(TS_MENU2);
        return;
    }

    int old = position;
    if (action->isAction(MAP_1)) position = 0;
    if (action->isAction(MAP_2)) position = 1;
    if (action->isAction(MAP_3)) position = 2;
    if (action->isAction(MAP_4)) position = 3;
    if (action->isAction(MAP_5)) position = 4;
    if (action->isAction(MAP_6)) position = 5;
    if (action->isAction(MAP_7)) position = 6;
    if (action->isAction(MAP_8)) position = 7;
    if (action->isAction(MAP_9)) position = 8;

    if (action->isAction(PUSH_UP) || action->isAction(PUSH_LEFT)) {
        if (--position < 0) position = 8;
    }
    if (action->isAction(PUSH_DOWN) || action->isAction(PUSH_RIGHT)) {
        if (++position > 8) position = 0;
    }

    if (old != position) {
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
}

void MapOcarina::loop() {
    if (!started) return;
    if (teleport && !AudioManager::getInstance()->isPlaying(2)) {
        MainController::getInstance()->getGameController()->hideMap();

        switch (position) {
            case 0 : MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(14, 133 * 16 + 8, 50 * 16 + 8, S, true, true); break;
            case 1 : MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(11, 20 * 16 + 8, 71 * 16 + 8, S, true, true); break;
            case 2 : MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(11, 105 * 16 + 8, 39 * 16 + 8, S, true, true); break;
            case 3 : MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(12, 22 * 16 + 8, 15 * 16 + 8, S, true, true); break;
            case 4 : MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(9, 39 * 16 + 8, 36 * 16 + 8, S, true, true); break;
            case 5 : MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(7, 19 * 16, 12 * 16 + 8, S, true, true); break;
            case 6 : MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(8, 58 * 16 + 8, 36 * 16 + 8, S, true, true); break;
            case 7 : MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(3, 20 * 16, 18 * 16 + 8, S, true, true); break;
            case 8 : MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(5, 41 * 16 + 8, 27 * 16, S, true, true); break;
            default : return;
        }
        MainController::getInstance()->getGameController()->getTeleportController()->loop();
        int i = position;
        MainController::getInstance()->getGameController()->setStep(GAME_MAP);
        position = i;
        started = false;
        teleport = false;
    }
}

void MapOcarina::draw() {
    WindowManager::getInstance()->draw(imageMap, 0, 0, 320, 240, 0, 0);

    if (position == 0) {
        WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, 119 - 7, 46 - 8);
    } else {
        TextManager::getInstance()->drawLetter('1', 119 - 4, 46 - 10, 0);
    }

    if (position == 1) {
        WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, 160 - 7, 64 - 8);
    } else {
        TextManager::getInstance()->drawLetter('2', 160 - 4, 64 - 10, 0);
    }

    if (position == 2) {
        WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, 236 - 7, 36 - 8);
    } else {
        TextManager::getInstance()->drawLetter('3', 236 - 4, 36 - 10, 0);
    }

    if (position == 3) {
        WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, 20 - 7, 95 - 8);
    } else {
        TextManager::getInstance()->drawLetter('4', 20 - 4, 95 - 10, 0);
    }

    if (position == 4) {
        WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, 160 - 7, 113 - 8);
    } else {
        TextManager::getInstance()->drawLetter('5', 160 - 4, 113 - 10, 0);
    }

    if (position == 5) {
        WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, 266 - 7, 92 - 8);
    } else {
        TextManager::getInstance()->drawLetter('6', 266 - 4, 92 - 10, 0);
    }

    if (position == 6) {
        WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, 52 - 7, 167 - 8);
    } else {
        TextManager::getInstance()->drawLetter('7', 52 - 4, 167 - 10, 0);
    }

    if (position == 7) {
        WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, 124 - 7, 204 - 8);
    } else {
        TextManager::getInstance()->drawLetter('8', 124 - 4, 204 - 10, 0);
    }

    if (position == 8) {
        WindowManager::getInstance()->draw(imageLink, 0, offsetSkin, 14, 12, 215 - 7, 158 - 8);
    } else {
        TextManager::getInstance()->drawLetter('9', 215 - 4, 158 - 10, 0);
    }
}

void MapOcarina::launch() {
    offsetSkin = ConfigurationManager::getInstance()->getSkin() * 15;
    position = 0;
    started = true;
    teleport = false;
}
