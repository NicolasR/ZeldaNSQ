#include "Ennemi113.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi113::Ennemi113(int i, int j) : anim(0), animMax(31), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi113.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 28;
    height = 32;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 60 : 40;
    maxLife = expert ? 60 : 40;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 20 : 10;
}

Ennemi113::~Ennemi113() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi113::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

bool Ennemi113::isResetable() {
    return alive;
}

void Ennemi113::ennLoop() {

    if (anim > 16) {
        // move
        int randomValue = (int)((float)rand() / RAND_MAX * (100));
        switch (randomValue) {
            case 1 : moveX(-1);direction=W; break;
            case 2 : moveX(1); direction=E; break;
            case 3 : moveY(-1);direction=N; break;
            case 4 : moveY(1); direction=S; break;
            default :
                if (randomValue < 10) break;
                switch (direction) {
                    case N : moveY(-1); break;
                    case S : moveY(1); break;
                    case W : moveX(-1); break;
                    case E : moveX(1); break;
                }
                break;
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

void Ennemi113::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    int anim2 = 0;
    if (anim > 14) anim2++;
    if (anim > 15) anim2++;
    if (anim > 16) anim2++;


    WindowManager::getInstance()->draw(image, 0, 29, 24, 6, dstX+2, dstY+29-3);

    WindowManager::getInstance()->draw(image, anim2 * width, 0, width, 29, dstX, dstY);
}

void Ennemi113::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + dx);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi113::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + dy);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi113::getX() {
    return x;
}

int Ennemi113::getY() {
    return y;
}

BoundingBox* Ennemi113::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

bool Ennemi113::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return anim > 16;
}

void Ennemi113::giveItem(int i, int j) {
    Ennemi::giveItem(i, j);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    BoundingBox b(0, 240*6, 320, 240);
    map->killEnnemis(&b);
}
