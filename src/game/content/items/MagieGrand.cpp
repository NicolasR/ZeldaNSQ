#include "MagieGrand.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

MagieGrand::MagieGrand(int i, int j) {

    x = i;
    y = j - 10;
    persistent = false;

    image = ResourceManager::getInstance()->loadImage("data/images/items/items.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 8;
    height = 26;

    box.setX(x);
    box.setY(y + 10);
    box.setW(8);
    box.setH(16);
}

MagieGrand::~MagieGrand() {
    ResourceManager::getInstance()->free(image);
}

void MagieGrand::loop() {
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

void MagieGrand::draw(int offsetX, int offsetY) {
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

    WindowManager::getInstance()->draw(image, 24, 3, 6, 4, x - offsetX + 1, y - offsetY + 22);
    WindowManager::getInstance()->draw(image, 8, 63, 8, 16, dstX, dstY);
}

void MagieGrand::action() {
    if (!ready) return;

    if (getLink()->getStatus()->getMagic() == getLink()->getStatus()->getMaxMagic()) {
        AudioManager::getInstance()->playSound(TS_ITEM);
    } else {
        AudioManager::getInstance()->playSound(TS_MAGICCHARGE);
    }

    getLink()->getStatus()->updateMagic(getLink()->getStatus()->getMaxMagic() / 4);
    alive = false;
}
