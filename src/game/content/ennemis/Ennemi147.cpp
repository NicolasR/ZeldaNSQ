#include "Ennemi147.h"

#include "Ennemi007.h"
#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi147::Ennemi147(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi147.png", true);
    chrono.reset();

    step = 0;
    cooldown = 0;
    snakes = false;

    x = i;
    y = j;

    // for quadtree operations:
    width = 31;
    height = 48;

    box.setX(x);
    box.setY(y+16);
    box.setW(32);
    box.setH(32);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 30 : 14;
    maxLife = expert ? 30 : 14;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 8 : 4;
}

Ennemi147::~Ennemi147() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi147::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    animMax = 1;
    step = 0;
    cooldown = 0;
    snakes = false;
    checkPosition();
}

bool Ennemi147::isResetable() {
    return alive;
}

void Ennemi147::ennLoop() {

    if (step == 0) {
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
        if (step == 0) {
            cooldown++;
            if (cooldown == 20) {
                if (!snakes) {
                    step = 1;
                    animMax = ((maxLife - life) / 2) + 1;
                    vanim = 360;
                } else {
                    step = 2;
                    animMax = 2;
                    vanim = 360;
                }
                anim = 0;
                cooldown = 0;
            }
        } else {
            if (anim == 0) {
                step = 0;
                animMax = 1;
                vanim = 180;
            } else {
                if (step == 1) {
                    snipe();
                } else if (anim == 1) {
                    snakes = false;
                    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

                    BoundingBox bb1(144*16,19*16,16,16);
                    if (map->checkCollisions(&bb1, (Collisionable*)getLink(), true, false, false, false)) {
                        map->addEnnemi(new Ennemi007(144*16,19*16,240));
                    }
                    BoundingBox bb2(155*16,19*16,16,16);
                    if (map->checkCollisions(&bb2, (Collisionable*)getLink(), true, false, false, false)) {
                        map->addEnnemi(new Ennemi007(155*16,19*16,240));
                    }
                    BoundingBox bb3(144*16,25*16,16,16);
                    if (map->checkCollisions(&bb3, (Collisionable*)getLink(), true, false, false, false)) {
                        map->addEnnemi(new Ennemi007(144*16,25*16,240));
                    }
                    BoundingBox bb4(155*16,25*16,16,16);
                    if (map->checkCollisions(&bb4, (Collisionable*)getLink(), true, false, false, false)) {
                        map->addEnnemi(new Ennemi007(155*16,25*16,240));
                    }
                }
            }
        }
        chrono.reset();
    }
}

void Ennemi147::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    Direction dir = (getLink()->getY() + 24 >= y + 48) ? S : N;

    switch (step) {
        case 0 :
            WindowManager::getInstance()->draw(image, anim * width, dir * height, width, height, dstX, dstY);
            break;
        case 1 :
            WindowManager::getInstance()->draw(image, ((anim%2) + 2) * width, dir * height, width, height, dstX, dstY);
            break;
        case 2 :
            WindowManager::getInstance()->draw(image, anim * width, 96, width, height, dstX, dstY);
            break;
    }
}

void Ennemi147::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + dx);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi147::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + 16 + dy);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi147::getX() {
    return x;
}

int Ennemi147::getY() {
    return y;
}

BoundingBox* Ennemi147::getBoundingBox() {
    box.setX(x);
    box.setY(y + 16);
    return &box;
}

bool Ennemi147::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    bool result = type == TA_MAGIC && effect == TE_FEU;
    if (result) snakes = true;
    return result;
}

void Ennemi147::snipe() {

    // throw proj and play sound
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    double anglx = 0;
    double angly = 0;
    int destx = dstX;
    int desty = dstY - 8;

    int origx = 0;
    int origy = 0;
    Direction dir = (getLink()->getY() + 24 >= y + 48) ? S : N;
    switch (dir) {
        case N : origx = x + 6 + (anim % 2) * 18; origy = y + 18; break;
        case S : origx = x + 24 - (anim % 2) * 18; origy = y + 26; break;
        default : break;
    }

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

    ProjectileHelper::getInstance()->addProjectile(TP_BOULE_FEU, origx, origy, anglx, angly);
    AudioManager::getInstance()->playSound(TS_THROW);
}
