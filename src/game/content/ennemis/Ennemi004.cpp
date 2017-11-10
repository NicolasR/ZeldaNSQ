#include "Ennemi004.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi004::Ennemi004(int i, int j) : anim(0), animMax(2), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi4.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 16;
    height = 17;

    box.setX(x);
    box.setY(y+1);
    box.setW(width);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 5 : 3;
    maxLife = expert ? 5 : 3;
    recul = 32;
    vitesseRecul = 4;
    maxGel = 128;

    items.addType(TI_PETIT_COEUR, 20);
    items.addType(TI_RUBIS_VERT, 20);

    forceEnn = expert ? 2 : 1;
}

Ennemi004::~Ennemi004() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi004::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi004::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 8 - dstX) + abs(y + 17 - dstY);
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

void Ennemi004::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, anim * width, (gel ? height : 0), width, height, dstX, dstY);
}

int Ennemi004::getX() {
    return x;
}

int Ennemi004::getY() {
    return y;
}

BoundingBox* Ennemi004::getBoundingBox() {
    box.setX(x);
    box.setY(y+1);
    return &box;
}
