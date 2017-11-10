#include "Flamme.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../MainController.h"

Flamme::Flamme(int i, int j, Direction dir)  : x(i), y(j), direction(dir), anim(0), animMax(4), vanim(240) {
    image = ResourceManager::getInstance()->loadImage("data/images/effects/feu.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);
}

Flamme::~Flamme() {
    ResourceManager::getInstance()->free(image);
}

void Flamme::loop() {

    MainController::getInstance()->getGameController()->getSceneController()->
        getScene()->testDegat(&box, direction, 1, TA_MAGIC, TE_FEU);

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            alive = false;
        }
        chrono.reset();
    }
}

void Flamme::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    int srcX = anim == 4 ? anim * width : (anim % 2) * width;

    WindowManager::getInstance()->draw(image, srcX, 0, width, height, dstX, dstY);
}

int Flamme::getX() {
    return x;
}

int Flamme::getY() {
    return y;
}

void Flamme::slide(int dx, int dy) {
    x += dx;
    y += dy;
}
