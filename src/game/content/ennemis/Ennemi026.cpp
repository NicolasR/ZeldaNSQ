#include "Ennemi026.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi026::Ennemi026(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi26.png", true);
    chrono.reset();

    x = i;
    y = j;
    direction = S;

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(16);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 5 : 3;
    maxLife = expert ? 5 : 3;
    recul = 32;
    vitesseRecul = 4;
    maxDist = 128;
    maxGel = 128;

    items.addType(TI_PETIT_COEUR, 10);
    items.addType(TI_FLECHE, 20);
    items.addType(TI_BOMBE, 20);
    items.addType(TI_RUBIS_BLEU, 10);

    forceEnn = expert ? 12 : 8;
}

Ennemi026::~Ennemi026() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi026::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    width = 16;
    height = 16;
    checkPosition();
}

void Ennemi026::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 8 - dstX) + abs(y + height - dstY);
    if (dist <= maxDist) {
        if (link->getStatus()->isVisible()) {

            if (height < 32) {
                height++;
                y--;
                maxDist = 200;
            } else {
                pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

                move(dir.first, dir.second);

                if (link->getBoundingBox()->intersect(getBoundingBox())) {
                    testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
                }
            }

        } else {
            idle = true;
            testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);
        }
    } else {
        idle = true;
    }

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Ennemi026::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (height == 16) {
        WindowManager::getInstance()->draw(image, 0, gel ? 16 : 0, 14, 16, dstX, dstY);
    } else {
        WindowManager::getInstance()->draw(image, 43, gel ? 16 : 0, 12, 6, dstX + 2, dstY + height - 6); // shadow
        WindowManager::getInstance()->draw(image, anim * 14, gel ? 16 : 0, 14, 16, dstX, dstY);
    }
}

int Ennemi026::getX() {
    return x;
}

int Ennemi026::getY() {
    return y;
}

BoundingBox* Ennemi026::getBoundingBox() {
    box.setX(x);
    box.setY(y + height - 16);
    return &box;
}

bool Ennemi026::isCollision(Collision c) {
    return true;
}

bool Ennemi026::isToAvoid(Collision c) {
    return false;
}
