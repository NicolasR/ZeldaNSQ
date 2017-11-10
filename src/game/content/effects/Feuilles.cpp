#include "Feuilles.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

Feuilles::Feuilles(int i, int j, int t)  : x(i), y(j), type(t), anim(0), animMax(6), vanim(120) {
    image = ResourceManager::getInstance()->loadImage("data/images/effects/buisson.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 27;
    height = 43;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);
}

Feuilles::~Feuilles() {
    ResourceManager::getInstance()->free(image);
}

void Feuilles::loop() {

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            alive = false;
        }
        chrono.reset();
    }
}

void Feuilles::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    int srcX = anim * width;
    int srcY = type * height;

    WindowManager::getInstance()->draw(image, srcX, srcY, width, height, dstX, dstY);
}

int Feuilles::getX() {
    return x;
}

int Feuilles::getY() {
    return y;
}
