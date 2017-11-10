#include "GemmeForce.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

GemmeForce::GemmeForce(int i, int j, int num, bool withAnim) : id(num) {

    x = i;
    y = j - 10;
    persistent = false;

    image = ResourceManager::getInstance()->loadImage("data/images/items/items.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 16;
    height = 26;

    box.setX(x);
    box.setY(y + 10);
    box.setW(16);
    box.setH(16);

    ready = !withAnim;
}

GemmeForce::~GemmeForce() {
    ResourceManager::getInstance()->free(image);
}

void GemmeForce::loop() {
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

void GemmeForce::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;
    int srcX = 16;


    if (ready) {
        dstY += 10;
    } else {
        if (anim <= 10) dstY += anim;
        else if (anim <= 14) dstY += 10 - (anim - 10);
        else if (anim <= 18) dstY += 6 + (anim - 14);
        else dstY += 10;
    }

    WindowManager::getInstance()->draw(image, 24, 3, 6, 4, dstX + 5, y - offsetY + 22);
    WindowManager::getInstance()->draw(image, srcX, 63, 16, 16, dstX, dstY);
}

void GemmeForce::action() {
    if (!ready) return;
    getLink()->trouveObjet(TI_GEMME_FORCE, id);
    alive = false;
}
