#include "Medaillon.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

Medaillon::Medaillon(int i, int j, int id) : id(id) {
    x = i;
    y = j;
    persistent = true;
    alive = true;
    ready = true;

    image = ResourceManager::getInstance()->loadImage("data/images/status/medaillons.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(16);
    box.setH(16);
}

Medaillon::~Medaillon() {
    ResourceManager::getInstance()->free(image);
}

void Medaillon::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, 16 * id, 0, 16, 16, dstX, dstY);
}

void Medaillon::action() {
    switch (id) {
        case 0 : getLink()->trouveObjet(TI_MEDAILLON_1, id); break;
        case 1 : getLink()->trouveObjet(TI_MEDAILLON_2, id); break;
        case 2 : getLink()->trouveObjet(TI_MEDAILLON_3, id); break;
        case 3 : getLink()->trouveObjet(TI_MEDAILLON_4, id); break;
        case 4 : getLink()->trouveObjet(TI_MEDAILLON_5, id); break;
        case 5 : getLink()->trouveObjet(TI_MEDAILLON_6, id); break;
        case 6 : getLink()->trouveObjet(TI_MEDAILLON_7, id); break;
    }
    alive = false;
}

bool Medaillon::isPickable() {return false;}
