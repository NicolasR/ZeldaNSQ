#include "ProjBoomerang.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../types/Item.h"

ProjBoomerang::ProjBoomerang(int i, int j, Direction dir, int type) : step(0), speed(4), dx(0), dy(0),
    level(0), currentLength(0), item(0) {
    x = i;
    y = j;
    direction = dir;
    vanim = 120;
    animMax = 3;

    chrono.reset();

    // for quadtree operations:
    width = 10;
    height = 10;

    switch(direction) {
        case N : dy = -speed; break;
        case S : dy = speed; break;
        case W : dx = -speed; break;
        case E : dx = speed; break;
    }

    box.setW(width);
    box.setH(height);

    force = type;

    maxLength = 64 + 48 * (type - 1);

    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/boomerang.png", true);

}

ProjBoomerang::~ProjBoomerang() {
    ResourceManager::getInstance()->free(image);
}

void ProjBoomerang::projLoop() {
    if (!alive) {
        return;
    }

    if (item != 0 && !item->isAlive()) {
        item = 0;
    }

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim%2 == 0) AudioManager::getInstance()->playSound(TS_BOOMERANG);
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    BoundingBox bb;
    bb.setX(x + 5);
    bb.setY(y + 5);
    bb.setW(1);
    bb.setH(1);

    if (step == 0) {

        // check level +
        Collision c = (Collision)scene->getMap()->getMur(x + 5, y + 5);
        switch (direction) {
            case N : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level ++; break;
            case S : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level ++; break;
            case W : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level ++; break;
            case E : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level ++; break;
        }

        if (!level) {
            item = scene->getMap()->getItem(getBoundingBox());
            if (item != 0) {
                step = 1;
                return;
            }
        }

        if (scene->checkCollisions(&bb, (Collisionable*)this, false, false, false, level)) {
            x += dx;
            y += dy;
            currentLength += speed;

            if (currentLength >= maxLength || (!level && scene->testDegat(&bb, direction, force, TA_BOOMERANG, TE_NORMAL))) {
                step = 1;
                return;
            }

        } else {
            step = 1;
            return;
        }

        // check level -
        switch (direction) {
            case N : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level --; break;
            case S : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level --; break;
            case W : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level --; break;
            case E : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level --; break;
        }

    } else {
        Link * link = scene->getLink();
        int dstX = link->getX() + 3;
        int dstY = link->getY() + 8;

        int tmpX = dstX - x;
        int ddx = tmpX > 0 ? tmpX : - tmpX;
        int tmpY = dstY - y;
        int ddy = tmpY > 0 ? tmpY : - tmpY;
        if (ddx > ddy) {
            if (tmpX > 0) direction = E;
            else if (tmpX < 0) direction = W;
        } else if (ddx < ddy) {
            if (tmpY > 0) direction = S;
            else if (tmpY < 0) direction = N;
        }

        int oldX = x;
        int oldY = y;

        if (dstX < x && x - dstX >= speed) {
            x -= speed;
            if (item != 0) {
                item->setX(item->getX() - speed);
            }
        }

        if (dstX > x && dstX - x >= speed) {
            x += speed;
            if (item != 0) {
                item->setX(item->getX() + speed);
            }
        }

        if (dstY < y && y - dstY >= speed) {
            y -= speed;
            if (item != 0) {
                item->setY(item->getY() - speed);
            }
        }

        if (dstY > y && dstY - y >= speed) {
            y += speed;
            if (item != 0) {
                item->setY(item->getY() + speed);
            }
        }

        if ((dstX == x && dstY == y) || (oldX == x && oldY == y)) {
            alive = false;
            link->unlockBoomerang();
        } else {
            scene->testDegat(&bb, direction, force, TA_BOOMERANG, TE_NORMAL);
        }

        if (!alive && item != 0) {
            item->action();
        }
    }
}

void ProjBoomerang::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, 10 * anim, 10 * (force - 1), 10, 10, dstX, dstY);
}

BoundingBox* ProjBoomerang::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

/*bool ProjBoomerang::isCollision(Collision c) {

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
