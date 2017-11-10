#include "Ennemi068.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi068::Ennemi068(int i, int j) : anim(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi68.png", true);

    x = i;
    y = j;
    direction = E;

    // for quadtree operations:
    width = 81;
    height = 63;

    box.setX(x + 2);
    box.setY(y + 3);
    box.setW(24);
    box.setH(37);

    life = 1;
    maxLife = 1;
    recul = 0;
    vitesseRecul = 0;

    forceEnn = expert ? 20 : 8;
}

Ennemi068::~Ennemi068() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi068::setStep(int step) {
    anim = step;
}

void Ennemi068::reset() {
    Ennemi::reset();
    anim = 0;
}

void Ennemi068::endOfLife() {
    alive = false;
}

bool Ennemi068::isResetable() {
    return alive;
}

void Ennemi068::ennLoop() {



    BoundingBox degat;

    switch (anim) {
        case 0 :
            testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
            break;
        case 1 :
            testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
            degat.setX(x + 26);
            degat.setY(y + 19);
            degat.setW(15);
            degat.setH(10);
            testDegatOnLink(&degat, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
            break;
        case 2 :
            degat.setX(x - 7);
            degat.setY(y);
            degat.setW(27);
            degat.setH(40);
            testDegatOnLink(&degat, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
            break;
        case 3 :
            degat.setX(x + 1);
            degat.setY(y + 20);
            degat.setW(32);
            degat.setH(22);
            testDegatOnLink(&degat, direction, forceEnn * 2, TA_PHYSIC, TE_NORMAL);

            degat.setX(x + 33);
            degat.setY(y + 31);
            degat.setW(32);
            degat.setH(18);
            testDegatOnLink(&degat, direction, forceEnn * 2, TA_PHYSIC, TE_NORMAL);

            degat.setX(x + 49);
            degat.setY(y + 47);
            degat.setW(32);
            degat.setH(18);
            testDegatOnLink(&degat, direction, forceEnn * 2, TA_PHYSIC, TE_NORMAL);
            break;
    }
}

void Ennemi068::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    switch (anim) {
        case 0 :
            WindowManager::getInstance()->draw(image, 0, 0, 26, 40, dstX, dstY);
            break;
        case 1 :
            WindowManager::getInstance()->draw(image, 26, 0, 41, 40, dstX, dstY);
            break;
        case 2 :
            WindowManager::getInstance()->draw(image, 67, 0, 27, 40, dstX - 7, dstY);
            break;
        case 3 :
            WindowManager::getInstance()->draw(image, 94, 0, 81, 63, dstX, dstY);
            break;
    }
}

int Ennemi068::getX() {
    return x;
}

int Ennemi068::getY() {
    return y;
}

BoundingBox* Ennemi068::getBoundingBox() {
    return &box;
}

bool Ennemi068::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return false;
}

int Ennemi068::getDown() {
    return y + 31;
}
