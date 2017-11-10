#include "Ennemi018.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi018::Ennemi018(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi18.png", true);
    chrono.reset();

    x = i;
    y = j;
    direction = S;

    // for quadtree operations:
    width = 16;
    height = 14;

    box.setX(x);
    box.setY(y);
    box.setW(16);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 3 : 2;
    maxLife = expert ? 3 : 2;
    recul = 32;
    vitesseRecul = 4;
    maxDist = 128;

    items.addType(TI_PETIT_COEUR, 30);
    items.addType(TI_RUBIS_VERT, 20);
    items.addType(TI_RUBIS_BLEU, 10);

    forceEnn = expert ? 2 : 1;
}

Ennemi018::~Ennemi018() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi018::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    width = 16;
    height = 14;
    checkPosition();
}

void Ennemi018::ennLoop() {

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
                pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

                move(dir.first, dir.second);

                if (link->getBoundingBox()->intersect(getBoundingBox())) {
                    testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
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

void Ennemi018::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (height == 14) {
        WindowManager::getInstance()->draw(image, 0, 28, width, height, dstX, dstY);
    } else {
        WindowManager::getInstance()->draw(image, 2, 42, 12, 6, dstX + 2, dstY + height - 6); // shadow
        WindowManager::getInstance()->draw(image, 0, anim * 14, 16, 14, dstX, dstY);
    }
}

int Ennemi018::getX() {
    return x;
}

int Ennemi018::getY() {
    return y;
}

BoundingBox* Ennemi018::getBoundingBox() {
    box.setX(x);
    box.setY(y + height - 16);
    return &box;
}

bool Ennemi018::isCollision(Collision c) {
    return true;
}

bool Ennemi018::isToAvoid(Collision c) {
    return false;
}

void Ennemi018::giveItem(int x, int y) {
    Ennemi::giveItem(x, y);
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Primes* primes = scene->getPrimes();

    if (primes->isRunning() && primes->getTime() > 0 && !primes->isSuccessful() && primes->getCurrent() == 18) {
        scene->getAnimationInGame()->startAnim(19);
    }
}
