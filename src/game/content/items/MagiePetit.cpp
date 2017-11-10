#include "MagiePetit.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

MagiePetit::MagiePetit(int i, int j) {

    x = i;
    y = j - 3;
    persistent = false;

    image = ResourceManager::getInstance()->loadImage("data/images/items/items.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 8;
    height = 19;

    box.setX(x);
    box.setY(y + 3 + 7);
    box.setW(8);
    box.setH(9);
}

MagiePetit::~MagiePetit() {
    ResourceManager::getInstance()->free(image);
}

void MagiePetit::loop() {
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

void MagiePetit::draw(int offsetX, int offsetY) {
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

    WindowManager::getInstance()->draw(image, 24, 3, 6, 4, x - offsetX + 1, y - offsetY + 15);
    WindowManager::getInstance()->draw(image, 0, 70, 8, 9, dstX, dstY);
}

void MagiePetit::action() {
    if (!ready) return;
    AudioManager::getInstance()->playSound(TS_ITEM);
    getLink()->getStatus()->updateMagic(getLink()->getStatus()->getMaxMagic() / 8);
    alive = false;
}
