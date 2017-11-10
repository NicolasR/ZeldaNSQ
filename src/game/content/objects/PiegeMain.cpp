#include "PiegeMain.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

PiegeMain::PiegeMain(int i, int j) : cooldown(0), step(0) {
    x = i;
    y = j;
    x0 = x;
    y0 = y;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/piegeMain.png", true);

    // for quadtree operations:
    width = 24;
    height = 24;

    box.setX(-1);
    box.setY(-1);
    box.setW(-1);
    box.setH(-1);
}

PiegeMain::~PiegeMain() {
    ResourceManager::getInstance()->free(image);
}

void PiegeMain::loop() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();

    cooldown++;
    if (step == 0) {
        if (cooldown == 240) {
            AudioManager::getInstance()->playSound(TS_TOMBE);
            step = 1;
            cooldown = 0;
            x = link->getX() - 4;
            y = link->getY();
        }
    } else if (step == 1) {
        if (cooldown == 120) {
            step = 2;
            cooldown = 0;
            BoundingBox b(x, y, 24, 24);
            if (b.intersect(link->getBoundingBox())) {
                AudioManager::getInstance()->playSound(TS_TELEPORT);
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(63, 9*16+8, 222 * 16, N, false, false, true);
                MainController::getInstance()->getGameController()->setStep(GAME_TELEPORT);
            }
        }
    } else if (step == 2) {
        if (cooldown == 120) {
            step = 0;
            cooldown = 0;
        }
    }
}

void PiegeMain::draw(int offsetX, int offsetY) {
    if (step == 1) {
        WindowManager::getInstance()->draw(image, 0, 22, 24, 6, x - offsetX, y - offsetY + 18);
        WindowManager::getInstance()->draw(image, 0, 0, 24, 22, x - offsetX, y - offsetY - 240 + cooldown*2);
    } else if (step == 2) {
        WindowManager::getInstance()->draw(image, 0, 22, 24, 6, x - offsetX, y - offsetY + 18);
        WindowManager::getInstance()->draw(image, 28, 0, 20, 21, x - offsetX + 2, y - offsetY - cooldown*2);
    }
}

BoundingBox* PiegeMain::getBoundingBox() {
    return &box;
}

bool PiegeMain::isResetable() {
    return true;
}

void PiegeMain::reset() {
    x = x0;
    y = y0;
    checkPosition();
    cooldown = 0;
    step = 0;
}

int PiegeMain::getDown() {
    return y + 20;
}
