#include "Ennemi044.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi044::Ennemi044(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi44.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 22 + 23; // + 23 for weapon
    height = 26 + 16;

    box.setX(x+3);
    box.setY(y+10);
    box.setW(16);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 30 : 16;
    maxLife = expert ? 30 : 16;
    recul = 32;
    vitesseRecul = 4;
    maxGel = 128;

    items.addType(TI_RUBIS_VERT, 30);
    items.addType(TI_RUBIS_BLEU, 30);
    items.addType(TI_RUBIS_ROUGE, 30);

    forceEnn = expert ? 40 : 20;
}

Ennemi044::~Ennemi044() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi044::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi044::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 22 / 2 - dstX) + abs(y + 26 - dstY);
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
                weapon.setX(x + 15);
                weapon.setY(y - 6 + 7 * anim);
                weapon.setW(7);
                weapon.setH(15);
                break;
            case S :
                weapon.setX(x);
                weapon.setY(y + 17 + 3 * anim);
                weapon.setW(7);
                weapon.setH(15);
                break;
            case W :
                weapon.setX(x - 12 + 5 * anim);
                weapon.setY(y + 14 + 2 * anim);
                weapon.setW(15);
                weapon.setH(7);
                break;
            case E :
                weapon.setX(x + 19 - 5 * anim);
                weapon.setY(y + 14 + 2 * anim);
                weapon.setW(15);
                weapon.setH(7);
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

void Ennemi044::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (direction == N) {
        WindowManager::getInstance()->draw(image, 88, 14 + (gel ? 52 : 0), 7, 15, dstX + 15, dstY - 6 + 7 * anim);
    }

    WindowManager::getInstance()->draw(image, direction * 22, anim * 26 + (gel ? 52 : 0), 22, 26, dstX, dstY);

    switch (direction) {
        case N : break; // already done
        case S :
            WindowManager::getInstance()->draw(image, 95, 14 + (gel ? 52 : 0), 7, 15, dstX, dstY + 17 + anim * 3);
            break;
        case W :
            WindowManager::getInstance()->draw(image, 88, 0 + (gel ? 52 : 0), 15, 7, dstX - 12 + 5 * anim, dstY + 14 + 2 * anim);
            break;
        case E :
            WindowManager::getInstance()->draw(image, 88, 7 + (gel ? 52 : 0), 15, 7, dstX + 19 - 5 * anim, dstY + 14 + 2 * anim);
            break;
    }
}

int Ennemi044::getX() {
    return x - 12;
}

int Ennemi044::getY() {
    return y - 6;
}

BoundingBox* Ennemi044::getBoundingBox() {
    box.setX(x + 3);
    box.setY(y + 10);
    return &box;
}
