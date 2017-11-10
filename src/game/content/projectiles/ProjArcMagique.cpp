#include "ProjArcMagique.h"


#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

ProjArcMagique::ProjArcMagique(int i, int j, Direction dir) : speed(4), dx(0), dy(0), level(0), retour(false) {
    x = i;
    y = j;
    direction = dir;
    vanim = 120;
    animMax = 1;

    force = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 8 : 4;

    // for quadtree operations:
    width = 16;
    height = 16;

    switch(direction) {
        case N : dy = -speed; height = 8; break;
        case S : dy = speed; height = 8; break;
        case W : dx = -speed; width = 8; break;
        case E : dx = speed; width = 8; break;
    }

    box.setW(width);
    box.setH(height);

    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/arcmagique.png", true);

    chrono.reset();
}

ProjArcMagique::~ProjArcMagique() {
    ResourceManager::getInstance()->free(image);
}

void ProjArcMagique::projLoop() {
    if (!alive) {
        return;
    }

    // compute bounding box for collisions
    int xc = x + 8;
    int yc = y + 8;
    switch (direction) {
        case N : yc = y; break;
        case S : break;
        case W : xc = x; break;
        case E : break;
    }

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    // check level +
    Collision c = (Collision)scene->getMap()->getMur(xc, yc);
    switch (direction) {
        case N : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level ++; break;
        case S : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level ++; break;
        case W : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level ++; break;
        case E : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level ++; break;
    }



    if (scene->checkCollisions(getBoundingBox(), (Collisionable*)this, false, false, false, level)) {
        x += dx;
        y += dy;

        if (!level && (
                (retour && scene->testDegat(getBoundingBox(), direction, force, TA_MAGIC, TE_NORMAL))
                || (!retour && scene->testDegatOnLink(getBoundingBox(), direction, force, TA_MAGIC, TE_NORMAL))
                       )) {
            alive = false;
            return;
        }

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

void ProjArcMagique::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    int srcX = 0;
    int srcY = 0;
    switch (direction) {
        case N : srcY = anim * 8; break;
        case S : srcX = 16; srcY = anim * 8; break;
        case W : srcX = 32 + anim * 8; break;
        case E : srcX = 48 + anim * 8; break;
    }

    WindowManager::getInstance()->draw(image, srcX, srcY, width, height, dstX, dstY);
}

BoundingBox* ProjArcMagique::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void ProjArcMagique::renvoie(Direction dir) {
    if (!retour) {

        switch (dir) {
            case N :
                if (dy > 0) {dy = -dy; direction = N;}
                break;
            case S :
                if (dy < 0) {dy = -dy; direction = S;}
                break;
            case W :
                if (dx > 0) {dx = -dx; direction = W;}
                break;
            case E :
                if (dx < 0) {dx = -dx; direction = E;}
                break;
        }

        AudioManager::getInstance()->playSound(TS_HITENNEMY);
        retour = true;
    }
}
