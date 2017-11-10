#include "Ennemi030.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi030::Ennemi030(int i, int j) : cooldown(64) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi30.png", true);

    x = i;
    y = j;

    // for quadtree operations:
    width = 16;
    height = 24;

    box.setX(x);
    box.setY(y+8);
    box.setW(16);
    box.setH(16);

    startDir = direction;

    life = expert ? 10 : 1;
    maxLife = expert ? 10 : 1;
    recul = 0;
    vitesseRecul = 0;
    idle = true;

    items.addType(TI_PETIT_COEUR, 20);
    items.addType(TI_MAGIE_PEU, 20);
    items.addType(TI_MAGIE_BEAUCOUP, 10);

    forceEnn = expert ? 8 : 4;
}

Ennemi030::~Ennemi030() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi030::reset() {
    Ennemi::reset();
    direction = startDir;
    cooldown = 64;
    checkPosition();
}

void Ennemi030::ennLoop() {

    if (cooldown) cooldown--;

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + width / 2 - dstX) + abs(y + height - dstY);
    if (dist <= maxDist) {
        if (link->getStatus()->isVisible()) {

            int srcX = x + 8;
            int srcY = y + 24;

            if (cooldown <= 64) {
                if (abs(dstX - srcX) > abs(dstY - srcY)) {
                    direction = (dstX > srcX) ? E : W;
                } else {
                    direction = (dstY > srcY) ? S : N;
                }
            }

            if (!cooldown) {
                switch(direction) {
                    case N : ProjectileHelper::getInstance()->addProjectile(TP_ARC_MAGIQUE, x, y, N); break;
                    case S : ProjectileHelper::getInstance()->addProjectile(TP_ARC_MAGIQUE, x, y + 16, S); break;
                    case W : ProjectileHelper::getInstance()->addProjectile(TP_ARC_MAGIQUE, x - 8, y + 8, W); break;
                    case E : ProjectileHelper::getInstance()->addProjectile(TP_ARC_MAGIQUE, x + 16, y + 8, E); break;
                }
                AudioManager::getInstance()->playSound(TS_MAGIC);
                cooldown = 128;
            }
        }
        testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);
    }
}

void Ennemi030::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, direction * width, (cooldown <= 64) ? 0 : 24, width, height, dstX, dstY);
}

int Ennemi030::getX() {
    return x;
}

int Ennemi030::getY() {
    return y;
}

BoundingBox* Ennemi030::getBoundingBox() {
    return &box;
}

bool Ennemi030::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return type == TA_MAGIC && effect == TE_NORMAL;
}
