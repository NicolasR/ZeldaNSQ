#include "Ennemi053.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"
#include "../scene/Primes.h"

#include "../../MainController.h"

Ennemi053::Ennemi053(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi53.png", true);
    chrono.reset();

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

    life = expert ? 3 : 1;
    maxLife = expert ? 3 : 1;
    recul = 32;
    vitesseRecul = 4;

    items.addType(TI_MAGIE_PEU, 20);
    items.addType(TI_RUBIS_VERT, 30);
    items.addType(TI_MAGIE_BEAUCOUP, 10);

    forceEnn = expert ? 2 : 1;
}

Ennemi053::~Ennemi053() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi053::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi053::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + width / 2 - dstX) + abs(y + height - dstY);
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

void Ennemi053::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, (direction%2) * 14, anim * 15, 14, 15, dstX + 1, dstY + 1);
}

int Ennemi053::getX() {
    return x;
}

int Ennemi053::getY() {
    return y;
}

BoundingBox* Ennemi053::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void Ennemi053::giveItem(int x, int y) {
    Ennemi::giveItem(x, y);
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Primes* primes = scene->getPrimes();

    if (primes->isRunning() && primes->getTime() > 0 && !primes->isSuccessful() && primes->getCurrent() == 53) {
        scene->getAnimationInGame()->startAnim(19);
    }
}
