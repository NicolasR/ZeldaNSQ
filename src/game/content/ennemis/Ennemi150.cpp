#include "Ennemi150.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi150::Ennemi150(int i, int j) : anim(0), animMax(1), vanim(180), surPied(true), cooldown(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi150.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 16;
    height = 17;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 3 : 1;
    maxLife = expert ? 3 : 1;
    recul = 32;
    vitesseRecul = 4;

    items.addType(TI_FLECHE, 20);
    items.addType(TI_BOMBE, 20);
    items.addType(TI_RUBIS_ROUGE, 20);

    forceEnn = expert ? 24 : 12;
}

Ennemi150::~Ennemi150() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi150::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
    surPied = true;
    cooldown = 0;
}

void Ennemi150::ennLoop() {

    if (cooldown) {
        cooldown--;
        if (cooldown == 0) {
            surPied = !surPied;
            if (!surPied) {
                cooldown = 64;
            }
        }
    }

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    if (surPied && !cooldown) {

        int dstX = link->getX() + 8;
        int dstY = link->getY() + 24;

        int dist = abs(x + 8 - dstX) + abs(y + 16 - dstY);
        if (dist <= maxDist) {
            if (link->getStatus()->isVisible()) {
                pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

                move(dir.first, dir.second);

                if (link->getBoundingBox()->intersect(getBoundingBox())) {
                    testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
                }
            } else {
                idle = true;
            }
        } else {
            idle = true;
        }
    }
    testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Ennemi150::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, 0, 32, 16, 6, dstX, dstY + 11);

    if (surPied) {
        if (cooldown == 0) {
            WindowManager::getInstance()->draw(image, anim * width, 0, width, 16, dstX, dstY);
        } else if (cooldown >= 16) {
            WindowManager::getInstance()->draw(image, anim * width, 0, width, 16, dstX, dstY - (32 - cooldown));
        } else {
            WindowManager::getInstance()->draw(image, anim * width, 16, width, 16, dstX, dstY - cooldown);
        }
    } else {
        WindowManager::getInstance()->draw(image, anim * width, 16, width, 16, dstX, dstY);
    }

}

int Ennemi150::getX() {
    return x;
}

int Ennemi150::getY() {
    return y;
}

BoundingBox* Ennemi150::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

bool Ennemi150::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    if (surPied && !cooldown && type == TA_MARTEAU) {
        cooldown = 32;
        idle = true;
    }
    return !surPied;
}
