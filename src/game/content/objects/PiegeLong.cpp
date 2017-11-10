#include "PiegeLong.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

PiegeLong::PiegeLong(int i, int j, Direction dir) : anim(0), animMax(3), vanim(240), direction(dir) {
    x = i;
    y = j;
    x0 = x;
    y0 = y;
    y1 = y;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/piegeLong.png", true);

    // for quadtree operations:
    width = 128;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    if (direction != N && direction != S) {
        direction = S;
    }

    force = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 4 : 2;

    chrono.reset();
}

PiegeLong::~PiegeLong() {
    ResourceManager::getInstance()->free(image);
}

void PiegeLong::loop() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    BoundingBox* bb = getBoundingBox();
    scene->testDegat(bb, direction, force, TA_PHYSIC, TE_NORMAL);
    scene->testDegatOnLink(bb, direction, force, TA_PHYSIC, TE_NORMAL);

    if (direction == N) {
        moveY(-2);
    } else {
        moveY(2);
    }

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void PiegeLong::draw(int offsetX, int offsetY) {
    WindowManager::getInstance()->draw(image, 0, height * anim, width, height, x - offsetX, y - offsetY);
}

BoundingBox* PiegeLong::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void PiegeLong::moveY(int dy) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + dy);

    if (scene->checkCollisions(bb, this, false)) {
        y += dy;
        y1 = oldY;
    }

    if (y != oldY) {
        checkPosition();
    } else {
        if (direction == N) {
            direction = S;
        } else {
            direction = N;
        }
        if (getBoundingBox()->intersect(scene->getCamera()->getBoundingBox()) && y != y1) {
            AudioManager::getInstance()->playSound(TS_PICS);
        }
    }
}

bool PiegeLong::isResetable() {
    return true;
}

void PiegeLong::reset() {
    x = x0;
    y = y0;
    y1 = y;
    checkPosition();
}
