#include "Ennemi118.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"
#include "../effects/Explosion.h"

#include "../../MainController.h"

Ennemi118::Ennemi118(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi118.png", true);
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

    life = expert ? 5 : 3;
    maxLife = expert ? 5 : 3;
    recul = 32;
    vitesseRecul = 4;
    maxGel = 128;

    items.addType(TI_MAGIE_PEU, 20);
    items.addType(TI_RUBIS_ROUGE, 10);
    items.addType(TI_MAGIE_BEAUCOUP, 10);

    forceEnn = expert ? 12 : 8;
}

Ennemi118::~Ennemi118() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi118::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

void Ennemi118::ennLoop() {

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

void Ennemi118::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, (direction%2) * 14, anim * 15 + (gel ? 30 : 0), 14, 15, dstX + 1, dstY + 1);
}

int Ennemi118::getX() {
    return x;
}

int Ennemi118::getY() {
    return y;
}

BoundingBox* Ennemi118::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void Ennemi118::giveItem(int x, int y) {
    AudioManager::getInstance()->playSound(TS_KILLENNEMY);
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (expert) {
        map->addEffect(new Explosion(x-16, y-16));
        map->addEffect(new Explosion(x+16, y-16));
        map->addEffect(new Explosion(x-16, y+16));
        map->addEffect(new Explosion(x+16, y+16));
    } else {
        map->addEffect(new Explosion(x, y));
    }
    TypeItem type = items.selectType();
    if (type != TI_NO_ITEM) {
        map->addItem(ItemHelper::getInstance()->createItem(type, x, y));
    }
}
