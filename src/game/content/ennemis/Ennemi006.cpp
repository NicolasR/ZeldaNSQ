#include "Ennemi006.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "cstdlib"

Ennemi006::Ennemi006(int i, int j) : anim(0), animMax(9), vanim(80), actif(false) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi6.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 16;
    height = 37;

    box.setX(x);
    box.setY(y + 21);
    box.setW(16);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 8 : 5;
    maxLife = expert ? 8 : 5;
    recul = 24;
    vitesseRecul = 4;
    maxDist = 64;
    maxGel = 128;

    items.addType(TI_BOMBE, 20);
    items.addType(TI_RUBIS_BLEU, 20);
    items.addType(TI_FLECHE, 20);

    forceEnn = expert ? 4 : 2;
}

Ennemi006::~Ennemi006() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi006::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    anim = 0;
    actif = false;
    checkPosition();
}

void Ennemi006::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 8 - dstX) + abs(y + height - dstY);
    if (dist <= maxDist) {
        if (link->getStatus()->isVisible()) {

            if (!actif) {
                actif = true;
                maxDist = 200;
                anim = 0;
                chrono.reset();
            } else {
                pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

                move(dir.first, dir.second);

                if (link->getBoundingBox()->intersect(getBoundingBox())) {
                    testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
                }
            }

        } else {
            idle = true;
            if (actif) testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);
        }
    } else {
        idle = true;
    }

    if (actif && chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Ennemi006::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (actif) {
        WindowManager::getInstance()->draw(image, 16 * anim, gel ? 37 : 0, 16, 37, dstX, dstY);
    } else {
        WindowManager::getInstance()->draw(image, 160, 0, 16, 37, dstX, dstY);
    }
}

int Ennemi006::getX() {
    return x;
}

int Ennemi006::getY() {
    return y;
}

BoundingBox* Ennemi006::getBoundingBox() {
    box.setX(x);
    box.setY(y + 21);
    return &box;
}

bool Ennemi006::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return actif;
}
