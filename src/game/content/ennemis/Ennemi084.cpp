#include "Ennemi084.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi084::Ennemi084(int i, int j, Direction dir) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi84.png", true);
    chrono.reset();

    x = i;
    y = j;
    direction = dir;

    // for quadtree operations:
    width = 16;
    height = 17;

    box.setX(x);
    box.setY(y);
    box.setW(16);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 12 : 6;
    maxLife = expert ? 12 : 6;
    recul = 32;
    vitesseRecul = 4;
    maxDist = 128;
    maxGel = 128;

    items.addType(TI_BOMBE, 20);
    items.addType(TI_RUBIS_BLEU, 10);
    items.addType(TI_FLECHE, 20);

    forceEnn = expert ? 8 : 4;
}

Ennemi084::~Ennemi084() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi084::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    width = 16;
    height = 17;
    checkPosition();
}

void Ennemi084::ennLoop() {

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
                for (int i = 0; i < 2 ; i++) {
                    pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

                    if (dir.first < 0) direction = W;
                    if (dir.first > 0) direction = E;

                    move(dir.first, dir.second);

                    if (link->getBoundingBox()->intersect(getBoundingBox())) {
                        testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
                    }
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

void Ennemi084::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (height == 17) {
        WindowManager::getInstance()->draw(image, (direction % 2) * width + (gel ? width * 2 : 0), 34, width, height, dstX, dstY);
    } else {
        WindowManager::getInstance()->draw(image, 2, 51, 12, 6, dstX, dstY + height - 6); // shadow
        WindowManager::getInstance()->draw(image, (direction % 2) * width + (gel ? width * 2 : 0), anim * 17, 16, 16, dstX, dstY);
    }
}

int Ennemi084::getX() {
    return x;
}

int Ennemi084::getY() {
    return y;
}

BoundingBox* Ennemi084::getBoundingBox() {
    box.setX(x);
    box.setY(y + height - 16);
    return &box;
}

bool Ennemi084::isCollision(Collision c) {
    return true;
}

bool Ennemi084::isToAvoid(Collision c) {
    return false;
}
