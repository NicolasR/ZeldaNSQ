#include "Ennemi014.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi014::Ennemi014(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi14.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 41;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(41);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 20 : 10;
    maxLife = expert ? 20 : 10;
    recul = 32;
    vitesseRecul = 4;
    maxGel = 128;

    items.addType(TI_MAGIE_PEU, 40);
    items.addType(TI_RUBIS_BLEU, 20);
    items.addType(TI_RUBIS_ROUGE, 10);

    forceEnn = expert ? 6 : 3;
}

Ennemi014::~Ennemi014() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi014::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi014::ennLoop() {

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

void Ennemi014::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, anim * width, (gel ? height : 0), width, height, dstX, dstY);
}

int Ennemi014::getX() {
    return x;
}

int Ennemi014::getY() {
    return y;
}

BoundingBox* Ennemi014::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void Ennemi014::giveItem(int x, int y) {
    Ennemi::giveItem(x, y);
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Primes* primes = scene->getPrimes();

    if (primes->isRunning() && primes->getTime() > 0 && !primes->isSuccessful() && primes->getCurrent() == 14) {
        scene->getAnimationInGame()->startAnim(19);
    }
}
