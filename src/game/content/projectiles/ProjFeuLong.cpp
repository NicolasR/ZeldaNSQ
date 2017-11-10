#include "ProjFeuLong.h"


#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

ProjFeuLong::ProjFeuLong(int i, int j, Direction dir, bool bl) : speed(4), dx(0), dy(0), level(0), force(4), byLink(bl) {
    x = i;
    y = j;
    direction = dir;
    vanim = 120;
    animMax = 1;

    // for quadtree operations:
    width = 16;
    height = 16;

    switch(direction) {
        case N : dy = -speed; width = 7; break;
        case S : dy = speed; width = 7; break;
        case W : dx = -speed; height = 7; break;
        case E : dx = speed; height = 7; break;
    }

    box.setW(width);
    box.setH(height);

    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/feu.png", true);

    if (byLink) {
        force = 4;
    } else {
        force = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 24 : 12;
    }

    chrono.reset();
}

ProjFeuLong::~ProjFeuLong() {
    ResourceManager::getInstance()->free(image);
}

void ProjFeuLong::projLoop() {
    if (!alive) {
        return;
    }

    // compute bounding box for collisions
    BoundingBox bb;
    switch (direction) {
        case N : bb.setX(x + dx + 3); bb.setY(y + dy); break;
        case S : bb.setX(x + dx + 3); bb.setY(y + dy + 15); break;
        case W : bb.setX(x + dx); bb.setY(y + dy + 3); break;
        case E : bb.setX(x + dx + 15); bb.setY(y + dy + 3); break;
    }

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    // check level +
    Collision c = (Collision)scene->getMap()->getMur(bb.getX(), bb.getY());
    switch (direction) {
        case N : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level ++; break;
        case S : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level ++; break;
        case W : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level ++; break;
        case E : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level ++; break;
    }



    bb.setX(x + dx);
    bb.setY(y + dy);
    bb.setW(width);
    bb.setH(height);

    if (!level && (
            (byLink && scene->testDegat(&bb, direction, force, TA_MAGIC, TE_FEU))
            || (!byLink && scene->testDegatOnLink(&bb, direction, force, TA_MAGIC, TE_FEU)))) {
        alive = false;
        return;
    }


    bb.setW(1);
    bb.setH(1);
    switch (direction) {
        case N : bb.setX(x + dx + 3); bb.setY(y + dy); break;
        case S : bb.setX(x + dx + 3); bb.setY(y + dy + 15); break;
        case W : bb.setX(x + dx); bb.setY(y + dy + 3); break;
        case E : bb.setX(x + dx + 15); bb.setY(y + dy + 3); break;
    }

    if (scene->checkCollisions(&bb, (Collisionable*)this, false, false, false, level)) {
        x += dx;
        y += dy;
    } else {
        alive = false;
        return;
    }

    // check level -
    switch (direction) {
        case N : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level --; break;
        case S : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level --; break;
        case W : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level --; break;
        case E : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level --; break;
    }

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void ProjFeuLong::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    int srcX = 0;
    int srcY = 0;
    switch (direction) {
        case N : srcX = anim * 7; break;
        case S : srcX = 14 + anim * 7; break;
        case W : srcX = 28; srcY = anim * 7; break;
        case E : srcX = 44; srcY = anim * 7; break;
    }

    WindowManager::getInstance()->draw(image, srcX, srcY, width, height, dstX, dstY);
}

BoundingBox* ProjFeuLong::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}
