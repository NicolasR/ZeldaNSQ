#include "ProjBaguetteGlace.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

ProjBaguetteGlace::ProjBaguetteGlace(int i, int j, Direction dir) : speed(8), dx(0), dy(0), level(0),
force(3), parcouru(0) {
    x = i;
    y = j;
    x0 = 0;
    y0 = 0;
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

ProjBaguetteGlace::~ProjBaguetteGlace() {
    ResourceManager::getInstance()->free(image);
}

void ProjBaguetteGlace::projLoop() {
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

        int tmp = parcouru;

        parcouru += ddx > 0 ? ddx : -ddx;
        parcouru += ddy > 0 ? ddy : -ddy;

        if ((tmp <= 17 && parcouru > 18 ) || (tmp <= 34 && parcouru > 34)) {
            switch (direction) {
                case N : height += 17; break;
                case S : height += 17; y0 -= 17; break;
                case W : width += 17; break;
                case E : width += 17; x0 -= 17; break;
            }
            box.setW(width);
            box.setH(height);
        }

        if (!level && scene->testDegat(getBoundingBox(), direction, force, TA_MAGIC, TE_GLACE)) {
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
}

void ProjBaguetteGlace::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, 34, 0, 17, 17, dstX, dstY);

    if (parcouru > 17) {
        WindowManager::getInstance()->draw(image, 17, 0, 17, 17, dstX - (17 * (dx / speed)), dstY - (17 * (dy / speed)));
        if (parcouru > 34) {
            WindowManager::getInstance()->draw(image, 0, 0, 17, 17, dstX - (34 * (dx / speed)), dstY - (34 * (dy / speed)));
        }
    }
}

BoundingBox* ProjBaguetteGlace::getBoundingBox() {
    box.setX(x + x0);
    box.setY(y + y0);
    return &box;
}

int ProjBaguetteGlace::getX() {return x + x0;}
int ProjBaguetteGlace::getY() {return y + y0;}
