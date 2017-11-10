#include "ProjBouleMort.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

ProjBouleMort::ProjBouleMort(int i, int j, double dx, double dy) : dx(dx), dy(dy), force(1), parcouru(0), retour(false) {
    x = i - 3;
    y = j - 3;
    longX = i;
    longY = j;
    for (int i = 0; i < 16; i++) {
        histX[i] = longX;
        histY[i] = longY;
    }
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

    // for quadtree operations:
    width = 7;
    height = 7;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/bouleMort.png", true);
}

ProjBouleMort::~ProjBouleMort() {
    ResourceManager::getInstance()->free(image);
}

void ProjBouleMort::projLoop() {
    if (!alive) {
        return;
    }

    // compute bounding box for collisions
    box.setX(longX - 3 + dx); box.setY(longY - 3 + dy);
    box.setW(7); box.setH(7);

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
        if (minX > (int)histX[2]) minX = (int)histX[2];
        if (maxX < (int)histX[2]) maxX = (int)histX[2];
        if (minY > (int)histY[2]) minY = (int)histY[2];
        if (maxY < (int)histY[2]) maxY = (int)histY[2];
        if (minX > (int)histX[4]) minX = (int)histX[4];
        if (maxX < (int)histX[4]) maxX = (int)histX[4];
        if (minY > (int)histY[4]) minY = (int)histY[4];
        if (maxY < (int)histY[4]) maxY = (int)histY[4];
        x = minX - 3;
        y = minY - 3;
        width = maxX - minX + 4;
        height = maxY - minY + 4;

        if (retour) {
            if (scene->testDegat(getBoundingBox(), direction, force, TA_MAGIC, TE_MORT)) {
                alive = false;
            }
        } else {
            if (scene->testDegatOnLink(getBoundingBox(), direction, force, TA_MAGIC, TE_MORT)) {
                alive = false;
            }
        }
    } else {
        alive = false;
    }
}

void ProjBouleMort::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = longX - 3 - offsetX;
    int dstY = longY - 3 - offsetY;

    WindowManager::getInstance()->draw(image, 0, 0, 7, 7, dstX, dstY);

    if (parcouru >= 2) {
        dstX = histX[2] - 1 - offsetX;
        dstY = histY[2] - 1 - offsetY;
        WindowManager::getInstance()->draw(image, 7, 0, 4, 4, dstX, dstY);
        if (parcouru >= 4) {
            dstX = histX[4] - 1 - offsetX;
            dstY = histY[4] - 1 - offsetY;
            WindowManager::getInstance()->draw(image, 11, 0, 3, 3, dstX, dstY);
        }
    }
}

BoundingBox* ProjBouleMort::getBoundingBox() {
    return &box;
}

int ProjBouleMort::getX() {return x;}
int ProjBouleMort::getY() {return y;}
int ProjBouleMort::getDown() {return y + 240;}

void ProjBouleMort::renvoie(Direction dir) {
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
