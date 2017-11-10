#include "Ennemi015.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi015::Ennemi015(int i, int j) : anim(0), animMax(1), vanim(180), cooldown(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi15.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 24;
    height = 31;

    box.setX(x);
    box.setY(y+15);
    box.setW(24);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 20 : 12;
    maxLife = expert ? 20 : 12;
    recul = 32;
    vitesseRecul = 4;
    maxGel = 128;

    items.addType(TI_MAGIE_PEU, 20);
    items.addType(TI_RUBIS_BLEU, 20);
    items.addType(TI_RUBIS_ROUGE, 10);

    forceEnn = expert ? 24 : 16;
}

Ennemi015::~Ennemi015() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi015::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    cooldown = 0;
    checkPosition();
}

void Ennemi015::ennLoop() {

    if (cooldown && !gel) cooldown--;

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + width / 2 - dstX) + abs(y + height - dstY);
    if (dist <= maxDist) {
        if (link->getStatus()->isVisible()) {

            bool fire = false;
            if (!cooldown) {

                int dx = x + width / 2 - dstX;
                int dy = y + height - dstY;

                switch(direction) {
                    case N : fire = (dy > 0 && abs(dx) <= 8); break;
                    case S : fire = (dy < 0 && abs(dx) <= 8); break;
                    case W : fire = (dx > 0 && abs(dy) <= 8); break;
                    case E : fire = (dx < 0 && abs(dy) <= 8); break;
                }
            }

            if (fire) {
                idle = true;
                testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);

                switch(direction) {
                    case N : ProjectileHelper::getInstance()->addProjectile(TP_FEU_LONG_ENN, x + 8, y - 4, N); break;
                    case S : ProjectileHelper::getInstance()->addProjectile(TP_FEU_LONG_ENN, x + 8, y + 20, S); break;
                    case W : ProjectileHelper::getInstance()->addProjectile(TP_FEU_LONG_ENN, x - 4, y + 15, W); break;
                    case E : ProjectileHelper::getInstance()->addProjectile(TP_FEU_LONG_ENN, x + 20, y + 15, E); break;
                }
                AudioManager::getInstance()->playSound(TS_BURN);
                cooldown = 128;

            } else {
                if (cooldown < 64) {
                    pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);
                    move(dir.first, dir.second);
                }
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
        if (!gel) anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Ennemi015::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, direction * width,
                                       ((cooldown == 0 || cooldown < 64) ? anim * height : height * 2) + (gel ? height * 3 : 0),
                                       width, height, dstX, dstY);
}

int Ennemi015::getX() {
    return x;
}

int Ennemi015::getY() {
    return y;
}

BoundingBox* Ennemi015::getBoundingBox() {
    box.setX(x);
    box.setY(y + 15);
    return &box;
}

void Ennemi015::giveItem(int x, int y) {
    Ennemi::giveItem(x, y);
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Primes* primes = scene->getPrimes();

    if (primes->isRunning() && primes->getTime() > 0 && !primes->isSuccessful() && primes->getCurrent() == 15) {
        scene->getAnimationInGame()->startAnim(19);
    }
}
