#include "NaviMagic.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

NaviMagic::NaviMagic(int x, int y) : x(x - 17), y(y - 17), anim(0), animMax(4), vanim(120) {
    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/magie.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 34;
    height = 34;
}

NaviMagic::~NaviMagic() {
    ResourceManager::getInstance()->free(image);
}

void NaviMagic::loop() {
    if (!alive) {
        return;
    }
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            alive = false;
        }
        chrono.reset();
    }
}

void NaviMagic::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    switch (anim) {
        case 0 :
            WindowManager::getInstance()->draw(image, 0, 0, 17, 17, dstX, dstY);
            break;
        case 1 :
            WindowManager::getInstance()->draw(image, 17, 0, 17, 17, dstX, dstY);
            WindowManager::getInstance()->draw(image, 0, 0, 17, 17, dstX + 17, dstY + 8);
            break;
        case 2 :
            WindowManager::getInstance()->draw(image, 34, 0, 17, 17, dstX, dstY);
            WindowManager::getInstance()->draw(image, 17, 0, 17, 17, dstX + 17, dstY + 8);
            WindowManager::getInstance()->draw(image, 0, 0, 17, 17, dstX + 8, dstY + 17);
            break;
        case 3 :
            WindowManager::getInstance()->draw(image, 34, 0, 17, 17, dstX + 17, dstY + 8);
            WindowManager::getInstance()->draw(image, 17, 0, 17, 17, dstX + 8, dstY + 17);
            break;
        case 4 :
            WindowManager::getInstance()->draw(image, 34, 0, 17, 17, dstX + 8, dstY + 17);
            break;
    }
}

int NaviMagic::getX() {
    return x;
}

int NaviMagic::getY() {
    return y;
}
