#include "Bomb.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

Bomb::Bomb(int i, int j) {

    x = i + 0;
    y = j - 10;
    persistent = false;

    image = ResourceManager::getInstance()->loadImage("data/images/items/items.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 13;
    height = 26;

    box.setX(x);
    box.setY(y + 10);
    box.setW(13);
    box.setH(16);
}

Bomb::~Bomb() {
    ResourceManager::getInstance()->free(image);
}

void Bomb::loop() {
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

void Bomb::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (ready) {
        dstY += 10;
    } else {
        if (anim <= 10) dstY += anim;
        else if (anim <= 14) dstY += 10 - (anim - 10);
        else if (anim <= 18) dstY += 6 + (anim - 14);
        else dstY += 10;
    }

    WindowManager::getInstance()->draw(image, 13, 57, 12, 6, dstX, y - offsetY + 20);
    WindowManager::getInstance()->draw(image, 0, 49, 13, 14, dstX, dstY);
}

void Bomb::action() {
    if (!ready) return;
    AudioManager::getInstance()->playSound(TS_ITEM);
    getLink()->getStatus()->setBombs(getLink()->getStatus()->getBombs() + 1);
    alive = false;
}
