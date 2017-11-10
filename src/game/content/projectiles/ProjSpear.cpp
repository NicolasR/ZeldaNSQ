#include "ProjSpear.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

ProjSpear::ProjSpear(int i, int j, Direction dir) : speed(4), dx(0), dy(0), wholeSpear(false) {
    x = i;
    y = j;
    direction = dir;

    // for quadtree operations:
    if (direction == N || direction == S) {
        width = 8;
        height = 16;
    } else {
        width = 16;
        height = 8 + 6;
    }

    switch(direction) {
        case N : dy = -speed; break;
        case S : dy = speed; break;
        case W : dx = -speed; break;
        case E : dx = speed; break;
    }

    box.setW(width);
    box.setH(height);

    force = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 4 : 2;
    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/spear.png", true);
    dist = 0;
}

ProjSpear::~ProjSpear() {
    ResourceManager::getInstance()->free(image);
}

void ProjSpear::projLoop() {
    if (!alive) {
        return;
    }

    int ddx = dx;
    int ddy = dy;

    if (dx != 0 && x % speed != 0) {
        int tmp = x % speed;
        if (dx > 0) {
            ddx = speed - tmp;
        } else {
            ddx = -tmp;
        }
    }

    if (dy != 0 && y % speed != 0) {
        int tmp = y % speed;
        if (dy > 0) {
            ddy = speed - tmp;
        } else {
            ddy = -tmp;
        }
    }

    // compute bounding box for collisions
    BoundingBox bb;
    switch (direction) {
        case N : bb.setX(x + ddx + 3); bb.setY(y + ddy + 8); break;
        case S : bb.setX(x + ddx + 3); bb.setY(y + ddy + 6); break;
        case W : bb.setX(x + ddx + 8); bb.setY(y + ddy + 3); break;
        case E : bb.setX(x + ddx + 6); bb.setY(y + ddy + 3); break;
    }
    bb.setW(1); bb.setH(1);

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    if (scene->testDegat(&bb, direction, force, TA_ARROW, TE_NORMAL)
        || scene->testDegatOnLink(&bb, direction, force, TA_ARROW, TE_NORMAL)) {
        alive = false;
        return;
    }

    if (!wholeSpear || dist <= 16 || scene->checkCollisions(&bb, (Collisionable*)this, false, false, false)) {
        x += ddx;
        y += ddy;
        dist = dist + abs(ddx) + abs(ddy);
        wholeSpear = true;

        if (scene->testDegat(&bb, direction, force, TA_ARROW, TE_NORMAL)
        || scene->testDegatOnLink(&bb, direction, force, TA_ARROW, TE_NORMAL)) {
            alive = false;
        }

    } else {
        alive = false;
    }
}

void ProjSpear::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int srcX = 16 * direction;
    int srcY = 0;
    int srcW = 16;
    int srcH = 16;

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (direction == N || direction == S) {
        dstX-=4;
        if (!wholeSpear) {
            srcH = 8;
            if (direction == S) {
                srcY += 8;
                dstY += 8;
            }
        }
    } else {
        dstY-=4;
        if (!wholeSpear) {
            srcW = 8;
            if (direction == E) {
                srcX += 8;
                dstX += 8;
            }
        }
    }

    WindowManager::getInstance()->draw(image, srcX, srcY, srcW, srcH, dstX, dstY);
}

BoundingBox* ProjSpear::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}
