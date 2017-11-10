#include "Ennemi007.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi007::Ennemi007(int i, int j, int ih) : anim(0), animMax(1), vanim(180), initHeight(ih), currentHeight(ih) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi7.png", true);
    chrono.reset();

    x = i;
    y = j - initHeight;

    // for quadtree operations:
    width = 16;
    height = 16 + initHeight;

    box.setX(x);
    box.setY(y + initHeight);
    box.setW(16);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 3 : 1;
    maxLife = expert ? 3 : 1;
    recul = 32;
    vitesseRecul = 4;

    items.addType(TI_RUBIS_VERT, 20);
    items.addType(TI_FLECHE, 10);
    items.addType(TI_BOMBE, 10);
    items.addType(TI_RUBIS_BLEU, 10);
}

Ennemi007::~Ennemi007() {
    ResourceManager::getInstance()->free(image);
}

bool Ennemi007::isResetable() {
    if (initHeight != 0) alive = false;
    return initHeight == 0;
}

void Ennemi007::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi007::ennLoop() {

    if (currentHeight) {
        currentHeight-=4;
        y+=4;
        height-=4;
        checkPosition();
        computeMaxSize();
        return;
    }

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + width / 2 - dstX) + abs(y + height - dstY);
    if (dist <= maxDist) {
        if (link->getStatus()->isVisible()) {
            pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

            move(dir.first, dir.second);

            if (link->getBoundingBox()->intersect(getBoundingBox())) {
                testDegatOnLink(&box, direction, 1, TA_PHYSIC, TE_QUASI_MORT);
            }
        } else {
            idle = true;
            testDegatOnLink(getBoundingBox(), direction, 1, TA_PHYSIC, TE_QUASI_MORT);
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

void Ennemi007::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (currentHeight) {
        // shadow
        WindowManager::getInstance()->draw(image, 2, 32, 12, 6, dstX + 2, dstY + currentHeight + 10);
    }
    WindowManager::getInstance()->draw(image, direction * width, anim * height, 16, 16, dstX, dstY);
}

int Ennemi007::getX() {
    return x;
}

int Ennemi007::getY() {
    return y;
}

BoundingBox* Ennemi007::getBoundingBox() {
    box.setX(x);
    box.setY(y + currentHeight);
    return &box;
}

bool Ennemi007::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return currentHeight == 0;
}
