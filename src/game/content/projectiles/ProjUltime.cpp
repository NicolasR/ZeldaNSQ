#include "ProjUltime.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

ProjUltime::ProjUltime(int i, int j, Link* lk) : dx(0), dy(0), parcouru(0), retour(false), link(lk) {
    x = i - 8;
    y = j - 8;
    longX = i;
    longY = j;
    for (int i = 0; i < 16; i++) {
        histX[i] = longX;
        histY[i] = longY;
    }

    computeDxDy();
    computeDir();

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    vanim = 120;
    anim = 0;
    animMax = 1;

    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/ultime.png", true);

    force = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 40 : 20;
}

ProjUltime::~ProjUltime() {
    ResourceManager::getInstance()->free(image);
}

void ProjUltime::projLoop() {
    if (!alive) {
        return;
    }

    if (!retour && link->getStatus()->getVirtualLife() > 0) {
        computeDxDy();
        computeDir();
    }

    // compute bounding box for collisions
    box.setX(longX - 8 + dx); box.setY(longY - 8 + dy);
    box.setW(16); box.setH(16);

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    if (scene->checkCollisions(&box, (Collisionable*)this, false)) {
        longX += dx;
        longY += dy;
        parcouru++;
        // update hist
        for (int i = 15; i > 0; i--) {
            histX[i] = histX[i - 1];
            histY[i] = histY[i - 1];
        }
        histX[0] = longX;
        histY[0] = longY;

        // update height + width
        int minX = (int)longX;
        int maxX = minX;
        int minY = (int)longY;
        int maxY = minY;
        if (minX > (int)histX[4]) minX = (int)histX[4];
        if (maxX < (int)histX[4]) maxX = (int)histX[4];
        if (minY > (int)histY[4]) minY = (int)histY[4];
        if (maxY < (int)histY[4]) maxY = (int)histY[4];
        if (minX > (int)histX[8]) minX = (int)histX[8];
        if (maxX < (int)histX[8]) maxX = (int)histX[8];
        if (minY > (int)histY[8]) minY = (int)histY[8];
        if (maxY < (int)histY[8]) maxY = (int)histY[8];
        x = minX - 8;
        y = minY - 8;
        width = maxX - minX + 8;
        height = maxY - minY + 8;

        if (retour) {
            if (scene->testDegat(getBoundingBox(), direction, force, TA_MAGIC, TE_NORMAL)) {
                alive = false;
            }
        } else {
            if (scene->testDegatOnLink(getBoundingBox(), direction, force, TA_MAGIC, TE_NORMAL)) {
                alive = false;
            }
        }
    } else {
        alive = false;
    }

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void ProjUltime::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = longX - 8 - offsetX;
    int dstY = longY - 8 - offsetY;

    WindowManager::getInstance()->draw(image, 0, anim * 16, 16, 16, dstX, dstY);

    if (parcouru >= 4) {
        dstX = histX[4] - 6 - offsetX;
        dstY = histY[4] - 6 - offsetY;
        WindowManager::getInstance()->draw(image, 16, anim * 16, 12, 12, dstX, dstY);
        if (parcouru >= 8) {
            dstX = histX[8] - 4 - offsetX;
            dstY = histY[8] - 4 - offsetY;
            WindowManager::getInstance()->draw(image, 28, anim * 16, 8, 8, dstX, dstY);
        }
    }
}

BoundingBox* ProjUltime::getBoundingBox() {
    return &box;
}

int ProjUltime::getX() {return x;}
int ProjUltime::getY() {return y;}
int ProjUltime::getDown() {return y + 240;}

void ProjUltime::renvoie(Direction dir) {
    if (!retour) {

        switch (dir) {
            case N :
                if (dy > 0) dy = -dy;
                break;
            case S :
                if (dy < 0) dy = -dy;
                break;
            case W :
                if (dx > 0) dx = -dx;
                break;
            case E :
                if (dx < 0) dx = -dx;
                break;
        }

        AudioManager::getInstance()->playSound(TS_HITENNEMY);
        retour = true;
    }
}

void ProjUltime::computeDxDy() {
    dx = 0;
    dy = 0;
    int destx = link->getX() + 8;
    int desty = link->getY() + 16;

    double coef1 = 0;
    double coef2 = 0;

    if ((destx-x) == 0) {dx=0; dy=12;}
    else if ((desty-y) == 0) {dx=12; dy=0;}
    else {
        coef1=((double)(desty-y))/((double)(destx-x));
        coef2=((double)(destx-x))/((double)(desty-y));
        dx=(sqrt(12/(1+(coef1*coef1))));
        dy=(sqrt(12/(1+(coef2*coef2))));
    }
    if (destx - x < 0) dx = -dx;
    if (desty - y < 0) dy = -dy;

    if (dx>4) dx=4;
    if (dy>4) dy=4;
    if (dx<-4) dx=-4;
    if (dy<-4) dy=-4;
}

void ProjUltime::computeDir() {
    if (dx > 0) {
        if (dy > 0) {
            if (dx > dy) direction = E;
            else direction = S;
        } else {
            if (dx > -dy) direction = E;
            else direction = N;
        }
    } else {
        if (dy > 0) {
            if (-dx > dy) direction = W;
            else direction = S;
        } else {
            if (-dx > -dy) direction = W;
            else direction = N;
        }
    }
}
