#include "Switch.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

Switch::Switch(int i, int j) : left(true), invul(0) {
    x = i;
    y = j;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/switch.png", true);

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    attackable = true;
}

Switch::~Switch() {
    ResourceManager::getInstance()->free(image);
}

void Switch::loop() {
    if (invul) {
        invul--;
        if (!invul) attackable = true;
    }
}

void Switch::draw(int offsetX, int offsetY) {
    WindowManager::getInstance()->draw(image, left ? 0 : 16, 0, 16, 16, x - offsetX, y - offsetY);
}

void Switch::underAttack(Direction dir, int force, TypeAttack type, TypeEffect effect) {
    if (type == TA_SWORD || type == TA_SWORD_HOLD) {
        AudioManager::getInstance()->playSound(TS_HITENNEMY);
        attackable = false;
        invul = 24;
        MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->activateInter(x, y);
        left = !left;
    }
}

void Switch::setLeft(bool l) {
    left = l;
    attackable = false;
    invul = 32;
}
