#include "Ennemi102.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi102::Ennemi102(int i, int j) : anim(0), animMax(1), vanim(180), cooldown(0), nbTorches(0), visible(false) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi102.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 24;
    height = 27;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    life = expert ? 75 : 50;
    maxLife = expert ? 75 : 50;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 32 : 16;
}

Ennemi102::~Ennemi102() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi102::reset() {
    Ennemi::reset();
    chrono.reset();
    anim = 0;
    cooldown = 0;
    nbTorches = 0;
    visible = false;
}

bool Ennemi102::isResetable() {
    return alive;
}

void Ennemi102::ennLoop() {

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

    testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        cooldown++;
        if (cooldown == 20 || (cooldown >= 10 && visible)) {
            if (visible) {
                eteint();
            } else {
                snipe();
            }
            cooldown = 0;
        }
        chrono.reset();
    }
}

void Ennemi102::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    Link* link = getLink();
    int dir = (link->getX() + 8 > x + 12) ? 1 : 0;

    int dstX = x - offsetX;
    int dstY = y - offsetY;


    image->setAlpha(255);
    WindowManager::getInstance()->draw(image, 0, 42, 24, 6, dstX, dstY + 21);

    if (nbTorches > 0) {
        int alpha = 64 * nbTorches;
        if (alpha < 255) image->setAlpha(alpha);
        WindowManager::getInstance()->draw(image, dir * 24, 21 * anim, 24, 21, dstX, dstY);
    }
}

void Ennemi102::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + dx);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi102::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + dy);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, false, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi102::getX() {
    return x;
}

int Ennemi102::getY() {
    return y;
}

BoundingBox* Ennemi102::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void Ennemi102::snipe() {

    // throw proj and play sound
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    double anglx = 0;
    double angly = 0;
    int origx = (link->getX() + 8 > x + 12) ? x + 16 : x + 8;
    int origy = y + 11;
    int destx = dstX;
    int desty = dstY - 8;

    double coef1 = 0;
    double coef2 = 0;

    if ((destx-origx) == 0) {anglx=0; angly=12;}
    else if ((desty-origy) == 0) {anglx=12; angly=0;}
    else {
        coef1=((double)(desty-origy))/((double)(destx-origx));
        coef2=((double)(destx-origx))/((double)(desty-origy));
        anglx=(sqrt(12/(1+(coef1*coef1))));
        angly=(sqrt(12/(1+(coef2*coef2))));
    }
    if (destx - origx < 0) anglx = -anglx;
    if (desty - origy < 0) angly = -angly;

    if (anglx>4) anglx=4;
    if (angly>4) angly=4;
    if (anglx<-4) anglx=-4;
    if (angly<-4) angly=-4;

    ProjectileHelper::getInstance()->addProjectile(TP_BIG_FEU, origx, origy, anglx, angly);
    AudioManager::getInstance()->playSound(TS_THROW);
}

void Ennemi102::allume() {
    nbTorches++;
    if (nbTorches == 4) {
        visible = true;
        cooldown = 0;
    }
}

void Ennemi102::eteint() {
    nbTorches--;
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    switch (nbTorches) {
        case 0 :
            map->setSol(77 * 16, 102 * 16, 971);
            visible = false;
            break;
        case 1 :
            map->setSol(62 * 16, 102 * 16, 971);
            break;
        case 2 :
            map->setSol(77 * 16, 92 * 16, 971);
            break;
        case 3 :
            map->setSol(62 * 16, 92 * 16, 971);
            break;
    }
}

bool Ennemi102::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    Link* link = getLink();
    bool result = visible && (((type == TA_SWORD || type == TA_SWORD_HOLD) && link->getEpee() >= 2)
        || (type == TA_ARROW && link->getInventory()->hasObject(ARC_FEE)));
    return result;
}
