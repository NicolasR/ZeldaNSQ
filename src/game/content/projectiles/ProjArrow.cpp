#include "ProjArrow.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

ProjArrow::ProjArrow(int i, int j, Direction dir, int type) : moving(true), speed(8), dx(0), dy(0), level(0) {
    x = i;
    y = j;
    direction = dir;
    vanim = 120;
    animMax = 5;

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

    switch (type) {
        default :
        case 1 :
            force = 2;
            image = ResourceManager::getInstance()->loadImage("data/images/projectiles/arrow.png", true);
            break;
        case 2 :
            force = 5;
            image = ResourceManager::getInstance()->loadImage("data/images/projectiles/arrow2.png", true);
            break;
    }


}

ProjArrow::~ProjArrow() {
    ResourceManager::getInstance()->free(image);
}

void ProjArrow::projLoop() {
    if (!alive) {
        return;
    }

    if (moving) {

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

        // check level +
        Collision c = (Collision)scene->getMap()->getMur(bb.getX(), bb.getY());
        switch (direction) {
            case N : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level ++; break;
            case S : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level ++; break;
            case W : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level ++; break;
            case E : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level ++; break;
        }

        if (!level && scene->testDegat(&bb, direction, force, TA_ARROW, TE_NORMAL)) {
            alive = false;
            return;
        }
        if (scene->checkCollisions(&bb, (Collisionable*)this, false, false, false, level)) {
            x += ddx;
            y += ddy;

            if (!level && scene->testDegat(&bb, direction, force, TA_ARROW, TE_NORMAL)) {
                alive = false;
                return;
            }

        } else {
            moving = false;
            chrono.reset();
            anim = 1;
        }

        // check level -
        switch (direction) {
            case N : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level --; break;
            case S : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level --; break;
            case W : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level --; break;
            case E : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level --; break;
        }

    } else {

        if (chrono.getElapsedTime() >= vanim) {
            anim++;
            if (anim > animMax) {
                alive = false;
            }
            chrono.reset();
        }

    }
}

void ProjArrow::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (direction == N || direction == S) {
        dstX-=4;
    } else {
        dstY-=4;
    }

    int srcY;
    switch (anim) {
        case 1 : case 3 : case 5 : srcY = 16; break;
        case 2 : srcY = 32; break;
        case 4 : srcY = 48; break;
        default : srcY = 0; break;
    }

    WindowManager::getInstance()->draw(image, 16 * direction, srcY, 16, 16, dstX, dstY);
}

BoundingBox* ProjArrow::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

/*bool ProjArrow::isCollision(Collision c) {

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
