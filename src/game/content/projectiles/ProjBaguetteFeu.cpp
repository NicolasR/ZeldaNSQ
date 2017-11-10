#include "ProjBaguetteFeu.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/Flamme.h"

ProjBaguetteFeu::ProjBaguetteFeu(int i, int j, Direction dir) : speed(4), dx(0), dy(0), level(0), force(4) {
    x = i;
    y = j;
    direction = dir;

    // for quadtree operations:
    width = 17;
    height = 17;

    switch(direction) {
        case N : dy = -speed; break;
        case S : dy = speed; break;
        case W : dx = -speed; break;
        case E : dx = speed; break;
    }

    box.setW(width);
    box.setH(height);

    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/magie.png", true);
}

ProjBaguetteFeu::~ProjBaguetteFeu() {
    ResourceManager::getInstance()->free(image);
}

void ProjBaguetteFeu::projLoop() {

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
    bb.setX(x + 8); bb.setY(y + 8);
    bb.setW(1); bb.setH(1);

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    // check level +
    Collision c = (Collision)scene->getMap()->getMur(bb.getX(), bb.getY());
    switch (direction) {
        case N : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level ++; break;
        case S : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level ++; break;
        case W : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level ++; break;
        case E : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level ++; break;
    }

    if (scene->checkCollisions(&bb, (Collisionable*)this, false, false, false, level)) {
        x += ddx;
        y += ddy;

        if (!level && scene->testDegat(getBoundingBox(), direction, force, TA_MAGIC, TE_FEU)) {
            alive = false;
        }

    } else {
        alive = false;
    }

    if (!alive) {
        AudioManager::getInstance()->playSound(TS_BURN);
        scene->getMap()->addEffect(new Flamme(x, y, direction));
        return;
    }

    // check level -
    switch (direction) {
        case N : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level --; break;
        case S : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level --; break;
        case W : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level --; break;
        case E : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level --; break;
    }

}

void ProjBaguetteFeu::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, 51 + 17 * direction, 0, 17, 17, dstX, dstY);
}

BoundingBox* ProjBaguetteFeu::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

/*bool ProjBaguetteFeu::isCollision(Collision c) {

    if ((c == MUR_H || c == MUR_B) && (direction == W || direction == E)) {
        return true;
    }
    if ((c == MUR_G || c == MUR_D) && (direction == N || direction == S)) {
        return true;
    }

    switch (c) {
        case TROU : case HERBE : case HERBE_HAUTE : case TERRE : case EAU :
        case EAU_PROF : case GLACE : case MURRET : return true;
        default : return false;
    }
}*/
