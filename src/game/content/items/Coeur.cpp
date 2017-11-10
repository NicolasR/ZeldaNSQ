#include "Coeur.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

Coeur::Coeur(int i, int j, int num, bool withAnim) : id(num) {

    x = i;
    y = j - 10;
    persistent = false;

    image = ResourceManager::getInstance()->loadImage("data/images/items/items.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 14;
    height = 23;

    box.setX(x);
    box.setY(y + 10);
    box.setW(14);
    box.setH(13);

    ready = !withAnim;
}

Coeur::~Coeur() {
    ResourceManager::getInstance()->free(image);
}

void Coeur::loop() {
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

void Coeur::draw(int offsetX, int offsetY) {
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

    WindowManager::getInstance()->draw(image, 24, 3, 6, 4, dstX + 4, y - offsetY + 21);
    WindowManager::getInstance()->draw(image, 30, 0, 14, 13, dstX, dstY);
}

void Coeur::action() {
    if (!ready) return;
    getLink()->trouveObjet(TI_COEUR, id);
    alive = false;
}

bool Coeur::isPickable() {
    return false;
}
