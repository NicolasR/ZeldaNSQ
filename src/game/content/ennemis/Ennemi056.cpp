#include "Ennemi056.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi056::Ennemi056(int i, int j) : anim(0), animMax(2), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi56.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 18;
    height = 17;

    box.setX(x);
    box.setY(y);
    box.setW(16);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 16 : 9;
    maxLife = expert ? 16 : 9;
    recul = 32;
    vitesseRecul = 4;

    items.addType(TI_MAGIE_PEU, 30);
    items.addType(TI_MAGIE_BEAUCOUP, 10);

    forceEnn = expert ? 8 : 4;
}

Ennemi056::~Ennemi056() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi056::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi056::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 8 / 2 - dstX) + abs(y + 8 - dstY);
    if (dist <= maxDist) {
        if (link->getStatus()->isVisible()) {
            pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

            if (dir.first < 0) direction = W;
            if (dir.first > 0) direction = E;

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
        if (!gel) anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Ennemi056::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, (direction%2) * 18, anim * 17, 18, 17, dstX - 1, dstY);
}

int Ennemi056::getX() {
    return x - 1;
}

int Ennemi056::getY() {
    return y;
}

BoundingBox* Ennemi056::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

bool Ennemi056::isCollision(Collision c) {
    return true;
}

bool Ennemi056::isToAvoid(Collision c) {
    return false;
}

bool Ennemi056::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    Link* link = getLink();
    return ((type == TA_SWORD || type == TA_SWORD_HOLD) && link->getEpee() >= 2)
        || (type == TA_ARROW && link->getInventory()->hasObject(ARC_FEE));
}
