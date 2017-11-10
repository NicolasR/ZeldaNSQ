#include "Ennemi114.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Ennemi114::Ennemi114(int i, int j, bool cm) : anim(0), animMax(2), vanim(180), canMove(cm) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi114.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 16;
    height = 32;

    box.setX(x);
    box.setY(y+16);
    box.setW(width);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = 1;
    maxLife = 1;
    recul = 0;
    vitesseRecul = 0;

    forceEnn = expert ? 12 : 8;
}

Ennemi114::~Ennemi114() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi114::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    checkPosition();
}

bool Ennemi114::isResetable() {
    return alive;
}

void Ennemi114::ennLoop() {

    if (canMove) {
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

void Ennemi114::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, anim * width, 0, width, height, dstX, dstY);
}

void Ennemi114::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + dx);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi114::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + 16 + dy);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi114::getX() {
    return x;
}

int Ennemi114::getY() {
    return y;
}

BoundingBox* Ennemi114::getBoundingBox() {
    box.setX(x);
    box.setY(y+16);
    return &box;
}

bool Ennemi114::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return false;//effect == TE_GLACE;
}
