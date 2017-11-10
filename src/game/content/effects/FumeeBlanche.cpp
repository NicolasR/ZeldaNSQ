#include "FumeeBlanche.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

FumeeBlanche::FumeeBlanche(int x, int y, int step) : x(x - 14), y(y - 14), anim(step), animMax(6), vanim(60) {
    image = ResourceManager::getInstance()->loadImage("data/images/effects/explo.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 28;
    height = 28;
}

FumeeBlanche::~FumeeBlanche() {
    ResourceManager::getInstance()->free(image);
}

void FumeeBlanche::loop() {
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            alive = false;
        }
        chrono.reset();
    }
}

void FumeeBlanche::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, width * anim, 0, width, height, dstX, dstY);
}

int FumeeBlanche::getX() {return x;}
int FumeeBlanche::getY() {return y;}
