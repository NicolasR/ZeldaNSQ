#include "Ennemi152.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi152::Ennemi152(int i, int j) : anim(0), animMax(13), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi152.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 38;
    height = 35;

    box.setX(x+3);
    box.setY(y+3);
    box.setW(32);
    box.setH(32);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 120 : 60;
    maxLife = expert ? 120 : 60;
    recul = 16;
    vitesseRecul = 2;
}

Ennemi152::~Ennemi152() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi152::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi152::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 19 - dstX) + abs(y + 35 - dstY);
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
}

void Ennemi152::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, anim * width, 0, width, height, dstX, dstY);
}

int Ennemi152::getX() {
    return x;
}

int Ennemi152::getY() {
    return y;
}

BoundingBox* Ennemi152::getBoundingBox() {
    box.setX(x + 3);
    box.setY(y + 3);
    return &box;
}
