#include "Ennemi023.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "cstdlib"

Ennemi023::Ennemi023(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi23.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 26;
    height = 17;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height-1);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 5 : 3;
    maxLife = expert ? 5 : 3;
    recul = 32;
    vitesseRecul = 4;
    maxGel = 128;

    items.addType(TI_PETIT_COEUR, 50);
    items.addType(TI_RUBIS_VERT, 20);

    forceEnn = expert ? 2 : 1;
}

Ennemi023::~Ennemi023() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi023::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi023::ennLoop() {

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

void Ennemi023::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, anim * width,  (gel ? height : 0), width, height, dstX, dstY);
}

int Ennemi023::getX() {
    return x;
}

int Ennemi023::getY() {
    return y;
}

BoundingBox* Ennemi023::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void Ennemi023::giveItem(int x, int y) {
    Ennemi::giveItem(x, y);
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Primes* primes = scene->getPrimes();

    if (primes->isRunning() && primes->getTime() > 0 && !primes->isSuccessful() && primes->getCurrent() == 23) {
        scene->getAnimationInGame()->startAnim(19);
    }
}
