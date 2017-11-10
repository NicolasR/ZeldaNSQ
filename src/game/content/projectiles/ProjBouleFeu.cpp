#include "ProjBouleFeu.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

ProjBouleFeu::ProjBouleFeu(int i, int j, double dx, double dy, BoundingBox* safeB) : dx(dx), dy(dy), parcouru(0), retour(false), safeBox(safeB) {
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

    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/bouleFeu.png", true);

    force = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 4 : 2;
}

ProjBouleFeu::~ProjBouleFeu() {
    ResourceManager::getInstance()->free(image);
}

void ProjBouleFeu::projLoop() {
    if (!alive) {
        return;
    }

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    // compute bounding box for collisions
    box.setX(longX - 3 + dx); box.setY(longY - 3 + dy);
    box.setW(7); box.setH(7);

    longX += dx;
    longY += dy;

    if (retour) {
        if (scene->testDegat(getBoundingBox(), direction, force, TA_MAGIC, TE_FEU)) {
            alive = false;
            return;
        }
    } else {
        if (scene->testDegatOnLink(getBoundingBox(), direction, force, TA_MAGIC, TE_FEU)
             || scene->testDegat(getBoundingBox(), direction, force, TA_MAGIC, TE_FEU, false)) {
            alive = false;
            return;
        }
    }

    if (((safeBox == 0 || retour) && scene->checkCollisions(&box, (Collisionable*)this, false))
        || ((safeBox != 0 && !retour) && scene->checkCollisions(&box, (Collisionable*)this, false, false, false, false, true,
                                                   safeBox->getX(), safeBox->getY(), safeBox->getW(), safeBox->getH()))) {

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

    } else {
        alive = false;
    }
}

void ProjBouleFeu::draw(int offsetX, int offsetY) {
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

BoundingBox* ProjBouleFeu::getBoundingBox() {
    return &box;
}

int ProjBouleFeu::getX() {return x;}
int ProjBouleFeu::getY() {return y;}
int ProjBouleFeu::getDown() {return y + 240;}

void ProjBouleFeu::renvoie(Direction dir) {
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
