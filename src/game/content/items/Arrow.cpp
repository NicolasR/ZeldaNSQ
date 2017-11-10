#include "Arrow.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

Arrow::Arrow(int i, int j) {

    x = i;
    y = j - 10;
    persistent = false;

    image = ResourceManager::getInstance()->loadImage("data/images/items/items.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 6;
    height = 26;

    box.setX(x);
    box.setY(y + 10);
    box.setW(6);
    box.setH(16);
}

Arrow::~Arrow() {
    ResourceManager::getInstance()->free(image);
}

void Arrow::loop() {
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

void Arrow::draw(int offsetX, int offsetY) {
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

    int srcY = (getLink()->getInventory()->hasObject(ARC_FEE)) ? 22 : 7;

    WindowManager::getInstance()->draw(image, 24, 3, 6, 4, dstX - 1, y - offsetY + 22);
    WindowManager::getInstance()->draw(image, 24, srcY, 5, 15, dstX, dstY);
}

void Arrow::action() {
    if (!ready) return;
    AudioManager::getInstance()->playSound(TS_ITEM);
    getLink()->getStatus()->setArrows(getLink()->getStatus()->getArrows() + 5);
    alive = false;
}
