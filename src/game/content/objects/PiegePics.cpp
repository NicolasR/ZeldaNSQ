#include "PiegePics.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

PiegePics::PiegePics(int i, int j, Direction dir) : direction(dir) {
    x = i;
    y = j;
    x0 = x;
    y0 = y;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/piegePics.png");

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    force = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 4 : 2;
}

PiegePics::~PiegePics() {
    ResourceManager::getInstance()->free(image);
}

void PiegePics::loop() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    BoundingBox* bb = getBoundingBox();
    scene->testDegat(bb, direction, force, TA_PHYSIC, TE_NORMAL);
    scene->testDegatOnLink(bb, direction, force, TA_PHYSIC, TE_NORMAL);

    switch (direction) {
        case N : moveY(-2); break;
        case S : moveY(2); break;
        case W : moveX(-2); break;
        case E : moveX(2); break;
    }
}

void PiegePics::draw(int offsetX, int offsetY) {
    WindowManager::getInstance()->draw(image, 0, 0, width, height, x - offsetX, y - offsetY);
}

BoundingBox* PiegePics::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void PiegePics::moveX(int dx) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + dx);

    if (scene->checkCollisions(bb, this, false)) {
        x += dx;
    }

    if (x != oldX) {
        checkPosition();
    } else {
        switch (direction) {
            case N : direction = S; break;
            case S : direction = N; break;
            case W : direction = E; break;
            case E : direction = W; break;
        }
        if (getBoundingBox()->intersect(scene->getCamera()->getBoundingBox())) {
            AudioManager::getInstance()->playSound(TS_PICS);
        }
    }
}

void PiegePics::moveY(int dy) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + dy);

    if (scene->checkCollisions(bb, this, false)) {
        y += dy;
    }

    if (y != oldY) {
        checkPosition();
    } else {
        switch (direction) {
            case N : direction = S; break;
            case S : direction = N; break;
            case W : direction = E; break;
            case E : direction = W; break;
        }
        if (getBoundingBox()->intersect(scene->getCamera()->getBoundingBox())) {
            AudioManager::getInstance()->playSound(TS_PICS);
        }
    }
}

bool PiegePics::isResetable() {
    return true;
}

void PiegePics::reset() {
    x = x0;
    y = y0;
    checkPosition();
}
