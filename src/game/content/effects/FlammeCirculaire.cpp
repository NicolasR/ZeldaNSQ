#include "FlammeCirculaire.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../MainController.h"

FlammeCirculaire::FlammeCirculaire(int i, int j, Direction dir)  : x(i), y(j), direction(dir), anim(0), animMax(3), vanim(120) {
    image = ResourceManager::getInstance()->loadImage("data/images/effects/feucirculaire.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 8;
    height = 8;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    force = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 2 : 1;
}

FlammeCirculaire::~FlammeCirculaire() {
    ResourceManager::getInstance()->free(image);
}

void FlammeCirculaire::loop() {

    MainController::getInstance()->getGameController()->getSceneController()->
        getScene()->testDegatOnLink(&box, direction, force, TA_MAGIC, TE_FEU);

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void FlammeCirculaire::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, anim * width, 0, width, height, dstX, dstY);
}

int FlammeCirculaire::getX() {
    return x;
}

int FlammeCirculaire::getY() {
    return y;
}

void FlammeCirculaire::move(int i, int j, Direction dir) {
    x = i;
    y = j;
    box.setX(x);
    box.setY(y);
    direction = dir;
    checkPosition();
}

void FlammeCirculaire::end() {
    alive = false;
}
