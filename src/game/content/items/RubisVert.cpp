#include "RubisVert.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

RubisVert::RubisVert(int i, int j) {

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

RubisVert::~RubisVert() {
    ResourceManager::getInstance()->free(image);
}

void RubisVert::loop() {
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            if (!ready) {
                ready = true;
                vanim = 120;
                animMax = 7;
            }
            anim = 0;
        }
        chrono.reset();
    }
}

void RubisVert::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;
    int srcX = 0;


    if (ready) {
        dstY += 10;
        if (anim < 3) srcX = anim * 8;
    } else {
        if (anim <= 10) dstY += anim;
        else if (anim <= 14) dstY += 10 - (anim - 10);
        else if (anim <= 18) dstY += 6 + (anim - 14);
        else dstY += 10;
    }

    WindowManager::getInstance()->draw(image, 24, 3, 6, 4, dstX + 1, y - offsetY + 22);
    WindowManager::getInstance()->draw(image, srcX, 7, 8, 14, dstX, dstY);
}

void RubisVert::action() {
    if (!ready) return;
    AudioManager::getInstance()->playSound(TS_RUPEE);
    getLink()->getStatus()->updateRupees(1);
    alive = false;
}
