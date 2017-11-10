#include "Ennemi005.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "cstdlib"

Ennemi005::Ennemi005(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi5.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 16 + 18; // + 18 for weapon
    height = 26 + 10;

    box.setX(x);
    box.setY(y+10);
    box.setW(16);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 5 : 3;
    maxLife = expert ? 5 : 3;
    recul = 32;
    vitesseRecul = 4;
    maxGel = 128;

    items.addType(TI_MAGIE_PEU, 20);
    items.addType(TI_FLECHE, 20);
    items.addType(TI_PETIT_COEUR, 30);
    items.addType(TI_RUBIS_BLEU, 10);

    forceEnn = expert ? 4 : 2;
}

Ennemi005::~Ennemi005() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi005::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi005::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 16 / 2 - dstX) + abs(y + 26 - dstY);
    if (dist <= maxDist) {
        if (link->getStatus()->isVisible()) {
            pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

            move(dir.first, dir.second);

            if (link->getBoundingBox()->intersect(getBoundingBox())) {
                testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
            }
        } else {
            idle = true;
            testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);
        }
        BoundingBox weapon;

        switch (direction) {
            case N :
                weapon.setX(x + 13);
                weapon.setY(anim == 0 ? y + 4 : y + 1);
                weapon.setW(5);
                weapon.setH(13);
                break;
            case S :
                weapon.setX(x);
                weapon.setY(anim == 0 ? y + 15 : y + 19);
                weapon.setW(5);
                weapon.setH(13);
                break;
            case W :
                weapon.setX(anim == 0 ? x - 4 : x - 9);
                weapon.setY(anim == 0 ? y + 19 : y + 17);
                weapon.setW(13);
                weapon.setH(5);
                break;
            case E :
                weapon.setX(anim == 0 ? x + 7 : x + 12);
                weapon.setY(anim == 0 ? y + 19 : y + 17);
                weapon.setW(13);
                weapon.setH(5);
                break;
        }
        testDegatOnLink(&weapon, direction, forceEnn, TA_PHYSIC, TE_NORMAL);


    } else {
        idle = true;
    }

    if (chrono.getElapsedTime() >= vanim) {
        if (!gel) anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Ennemi005::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (direction == N) {
        WindowManager::getInstance()->draw(image, 64, 10 + (gel ? 52 : 0), 5, 13, dstX + 13, anim == 0 ? dstY + 4 : dstY + 1);
    }

    WindowManager::getInstance()->draw(image, direction * 16, anim * 26 + (gel ? 52 : 0), 16, 26, dstX, dstY);

    switch (direction) {
        case N : break; // already done
        case S :
            WindowManager::getInstance()->draw(image, 69, 10 + (gel ? 52 : 0), 5, 13, dstX, anim == 0 ? dstY + 15 : dstY + 19);
            break;
        case W :
            WindowManager::getInstance()->draw(image, 64, (gel ? 52 : 0), 13, 5, anim == 0 ? dstX - 4 : dstX - 9, anim == 0 ? dstY + 19 : dstY + 17);
            break;
        case E :
            WindowManager::getInstance()->draw(image, 64, 5 + (gel ? 52 : 0), 13, 5, anim == 0 ? dstX + 7 : dstX + 12, anim == 0 ? dstY + 19 : dstY + 17);
            break;
    }
}

int Ennemi005::getX() {
    return x - 9;
}

int Ennemi005::getY() {
    return y - 2;
}

BoundingBox* Ennemi005::getBoundingBox() {
    box.setX(x);
    box.setY(y + 10);
    return &box;
}
