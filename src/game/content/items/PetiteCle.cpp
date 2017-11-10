#include "PetiteCle.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

PetiteCle::PetiteCle(int i, int j, int id) : id(id) {
    x = i;
    y = j;
    persistent = true;
    alive = true;
    ready = true;

    image = ResourceManager::getInstance()->loadImage("data/images/items/items.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 8;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(8);
    box.setH(16);
}

PetiteCle::~PetiteCle() {
    ResourceManager::getInstance()->free(image);
}

void PetiteCle::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, 24, 3, 6, 4, dstX + 1, dstY + 12);
    WindowManager::getInstance()->draw(image, 100, 48, 8, 16, dstX, dstY);
}

void PetiteCle::action() {
    getLink()->trouveObjet(TI_CLE, id);
    alive = false;
}
