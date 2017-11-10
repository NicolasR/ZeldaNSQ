#include "PetitCoeur.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

PetitCoeur::PetitCoeur(int i, int j) {

    x = i - 1;
    y = j;
    persistent = false;

    image = ResourceManager::getInstance()->loadImage("data/images/items/items.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 10;
    height = 16;

    box.setX(x + 1);
    box.setY(y + 7);
    box.setW(8);
    box.setH(9);
}

PetitCoeur::~PetitCoeur() {
    ResourceManager::getInstance()->free(image);
}

void PetitCoeur::loop() {
    if (!ready) {
        if (chrono.getElapsedTime() >= vanim) {
            anim++;
            if (anim > animMax) {
                ready = true;
            }
            chrono.reset();
        }
    }
}

void PetitCoeur::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x + 1 - offsetX;
    int dstY = y - offsetY;
    int srcX = 16;

    if (anim <= 20) {
        srcX = (anim < 10) ? 0 : 8;
        dstY += anim / 3;
        dstX = dstX - 1 + ((anim / 6) % 3) + anim / 12;
    } else {
        dstY += 7;
    }

    WindowManager::getInstance()->draw(image, 24, 3, 6, 4, x + 1 - offsetX + 1, y - offsetY + 12);
    WindowManager::getInstance()->draw(image, srcX, 0, 8, 7, dstX, dstY);
}

void PetitCoeur::action() {
    if (!ready) return;


    if (getLink()->getStatus()->getLife() == getLink()->getStatus()->getMaxLife()) {
        AudioManager::getInstance()->playSound(TS_HEART);
    }

    getLink()->getStatus()->updateLife(2);
    alive = false;
}
