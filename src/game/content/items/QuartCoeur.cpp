#include "QuartCoeur.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

QuartCoeur::QuartCoeur(int i, int j, int id) : id(id) {
    x = i;
    y = j;
    persistent = true;
    alive = true;
    ready = true;

    image = ResourceManager::getInstance()->loadImage("data/images/items/items.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(16);
    box.setH(14);
}

QuartCoeur::~QuartCoeur() {
    ResourceManager::getInstance()->free(image);
}

void QuartCoeur::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, 13, 57, 12, 6, dstX + 2, dstY + 10);
    WindowManager::getInstance()->draw(image, 45, 0, 16, 14, dstX, dstY);
}

void QuartCoeur::action() {
    getLink()->trouveObjet(TI_QUART_COEUR, id);
    alive = false;
}

bool QuartCoeur::isPickable() {return false;}
