#include "Ennemi069.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi069::Ennemi069(int i, int j) : anim(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi69.png", true);

    x = i;
    y = j;
    direction = W;

    // for quadtree operations:
    width = 81;
    height = 63;

    box.setX(x + 57);
    box.setY(y + 3);
    box.setW(24);
    box.setH(37);

    life = 1;
    maxLife = 1;
    recul = 0;
    vitesseRecul = 0;

    forceEnn = expert ? 20 : 8;
}

Ennemi069::~Ennemi069() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi069::setStep(int step) {
    anim = step;
}

void Ennemi069::reset() {
    Ennemi::reset();
    anim = 0;
}

void Ennemi069::endOfLife() {
    alive = false;
}

bool Ennemi069::isResetable() {
    return alive;
}

void Ennemi069::ennLoop() {



    BoundingBox degat;

    switch (anim) {
        case 0 :
            testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
            break;
        case 1 :
            testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
            degat.setX(x + 40);
            degat.setY(y + 19);
            degat.setW(15);
            degat.setH(10);
            testDegatOnLink(&degat, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
            break;
        case 2 :
            degat.setX(x + 61);
            degat.setY(y);
            degat.setW(27);
            degat.setH(40);
            testDegatOnLink(&degat, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
            break;
        case 3 :
            degat.setX(x + 48);
            degat.setY(y + 20);
            degat.setW(32);
            degat.setH(22);
            testDegatOnLink(&degat, direction, forceEnn * 2, TA_PHYSIC, TE_NORMAL);

            degat.setX(x + 16);
            degat.setY(y + 31);
            degat.setW(32);
            degat.setH(18);
            testDegatOnLink(&degat, direction, forceEnn * 2, TA_PHYSIC, TE_NORMAL);

            degat.setX(x);
            degat.setY(y + 47);
            degat.setW(32);
            degat.setH(18);
            testDegatOnLink(&degat, direction, forceEnn * 2, TA_PHYSIC, TE_NORMAL);
            break;
    }
}

void Ennemi069::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    switch (anim) {
        case 0 :
            WindowManager::getInstance()->draw(image, 0, 0, 26, 40, dstX + 57, dstY);
            break;
        case 1 :
            WindowManager::getInstance()->draw(image, 26, 0, 41, 40, dstX + 40, dstY);
            break;
        case 2 :
            WindowManager::getInstance()->draw(image, 67, 0, 27, 40, dstX + 61, dstY);
            break;
        case 3 :
            WindowManager::getInstance()->draw(image, 94, 0, 81, 63, dstX, dstY);
            break;
    }
}

int Ennemi069::getX() {
    return x;
}

int Ennemi069::getY() {
    return y;
}

BoundingBox* Ennemi069::getBoundingBox() {
    return &box;
}

bool Ennemi069::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return false;
}

int Ennemi069::getDown() {
    return y + 31;
}
