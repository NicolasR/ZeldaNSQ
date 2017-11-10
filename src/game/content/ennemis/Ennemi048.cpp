#include "Ennemi048.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi048::Ennemi048(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi48.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 22 + 26; // + 26 for weapon
    height = 27 + 19;

    box.setX(x+3);
    box.setY(y+11);
    box.setW(16);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 80 : 40;
    maxLife = expert ? 80 : 40;
    recul = 32;
    vitesseRecul = 4;
    maxGel = 128;

    items.addType(TI_RUBIS_ROUGE, 70);

    forceEnn = expert ? 80 : 40;
}

Ennemi048::~Ennemi048() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi048::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi048::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 22 / 2 - dstX) + abs(y + 27 - dstY);
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
                weapon.setX(x + 16);
                weapon.setY(y - 5 + 4 * anim);
                weapon.setW(5);
                weapon.setH(17);
                break;
            case S :
                weapon.setX(x + anim);
                weapon.setY(y + 24 - 3 * anim);
                weapon.setW(5);
                weapon.setH(17);
                break;
            case W :
                weapon.setX(x - 13 + 4 * anim);
                weapon.setY(y + 16 + 3 * anim);
                weapon.setW(17);
                weapon.setH(5);
                break;
            case E :
                weapon.setX(x + 18 - 4 * anim);
                weapon.setY(y + 16 + 3 * anim);
                weapon.setW(17);
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

void Ennemi048::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (direction == N) {
        WindowManager::getInstance()->draw(image, 88, 10 + (gel ? 54 : 0), 5, 17, dstX + 16, dstY - 5 + 4 * anim);
    }

    WindowManager::getInstance()->draw(image, direction * 22, anim * 27 + (gel ? 54 : 0), 22, 27, dstX, dstY);

    switch (direction) {
        case N : break; // already done
        case S :
            WindowManager::getInstance()->draw(image, 93, 10 + (gel ? 54 : 0), 5, 17, dstX + anim, dstY + 24 - anim*3);
            break;
        case W :
            WindowManager::getInstance()->draw(image, 88, 0 + (gel ? 54 : 0), 17, 5, dstX - 13 + 4 * anim, dstY + 16 + 3 * anim);
            break;
        case E :
            WindowManager::getInstance()->draw(image, 88, 5 + (gel ? 54 : 0), 17, 5, dstX + 18 - 4 * anim, dstY + 16 + 3 * anim);
            break;
    }
}

int Ennemi048::getX() {
    return x - 13;
}

int Ennemi048::getY() {
    return y - 5;
}

BoundingBox* Ennemi048::getBoundingBox() {
    box.setX(x + 3);
    box.setY(y + 11);
    return &box;
}
