#include "Ennemi151.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi151::Ennemi151(int i, int j) : anim(0), animMax(3), vanim(180), anim2(0), animMax2(3), vanim2(640) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi151.png", true);
    chrono.reset();
    chrono2.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 60 : 30;
    maxLife = expert ? 60 : 30;
    recul = 32;
    vitesseRecul = 4;
    maxGel = 128;
}

Ennemi151::~Ennemi151() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi151::reset() {
    Ennemi::reset();
    chrono.reset();
    chrono2.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    anim2 = 0;
    checkPosition();
}

void Ennemi151::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 8 - dstX) + abs(y + 16 - dstY);
    if (dist <= maxDist) {
        if (link->getStatus()->isVisible()) {
            pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

            move(dir.first, dir.second);

            if (link->getBoundingBox()->intersect(getBoundingBox())) {
                testDegatOnLink(&box, direction, life, TA_PHYSIC, TE_NORMAL);
            }
        } else {
            idle = true;
            testDegatOnLink(getBoundingBox(), direction, life, TA_PHYSIC, TE_NORMAL);
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

    if (chrono2.getElapsedTime() >= vanim2) {
        anim2++;
        if (anim2 > animMax2) {
            anim2 = 0;
        }
        chrono2.reset();
    }
}

void Ennemi151::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, anim * width, gel ? 64 : anim2 * height, width, height, dstX, dstY);
}

int Ennemi151::getX() {
    return x;
}

int Ennemi151::getY() {
    return y;
}

BoundingBox* Ennemi151::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}
