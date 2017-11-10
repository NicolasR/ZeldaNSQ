#include "FumeeViolette.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

FumeeViolette::FumeeViolette(int x, int y) : x(x - 14), y(y - 14), anim(0), animMax(2), vanim(60) {
    image = ResourceManager::getInstance()->loadImage("data/images/effects/exploOni.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 28;
    height = 28;
}

FumeeViolette::~FumeeViolette() {
    ResourceManager::getInstance()->free(image);
}

void FumeeViolette::loop() {
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            alive = false;
        }
        chrono.reset();
    }
}

void FumeeViolette::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, width * anim, 0, width, height, dstX, dstY);
}

int FumeeViolette::getX() {return x;}
int FumeeViolette::getY() {return y;}
