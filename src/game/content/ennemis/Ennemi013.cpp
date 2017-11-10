#include "Ennemi013.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi013::Ennemi013(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi13.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 32;
    height = 15;

    box.setX(x + 1);
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
    maxDist = 160;
    maxGel = 128;

    items.addType(TI_PETIT_COEUR, 10);
    items.addType(TI_RUBIS_VERT, 20);
    items.addType(TI_RUBIS_BLEU, 10);
    items.addType(TI_FLECHE, 10);
    items.addType(TI_BOMBE, 10);

    forceEnn = expert ? 4 : 2;
}

Ennemi013::~Ennemi013() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi013::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    width = 32;
    height = 15;
    checkPosition();
}

void Ennemi013::ennLoop() {

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

void Ennemi013::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (height == 15) {
        WindowManager::getInstance()->draw(image, 64,  (gel ? 21 : 0), 32, 15, dstX - 8, dstY);
    } else {
        WindowManager::getInstance()->draw(image, 10, 15, 12, 6, dstX + 2, dstY + height - 6); // shadow
        WindowManager::getInstance()->draw(image, anim * 32, (gel ? 21 : 0), 32, 15, dstX - 8, dstY);
    }
}

int Ennemi013::getX() {
    return x - 8;
}

int Ennemi013::getY() {
    return y;
}

BoundingBox* Ennemi013::getBoundingBox() {
    box.setX(x);
    box.setY(y + height - 16);
    return &box;
}

bool Ennemi013::isCollision(Collision c) {
    return true;
}

bool Ennemi013::isToAvoid(Collision c) {
    return false;
}
