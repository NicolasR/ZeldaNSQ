#include "Ennemi095.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi095::Ennemi095(int i, int j) : anim(0), animMax(1), vanim(180), step(0), cooldown(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi95.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 27;
    height = 25;

    box.setX(x+6);
    box.setY(y+9);
    box.setW(16);
    box.setH(16);

    life = 1;
    maxLife = 1;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 20 : 10;
}

Ennemi095::~Ennemi095() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi095::reset() {
    Ennemi::reset();
    chrono.reset();
    anim = 0;
    cooldown = 0;
    step = 0;
}

bool Ennemi095::isResetable() {
    return alive;
}

void Ennemi095::ennLoop() {

    Link* link = getLink();
    direction = (link->getX() + 8 > x + 14) ? E : W;

    testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
            step = 0;
            cooldown++;
        }
        if (cooldown == 8) {
            int randomValue = (int)((float)rand() / RAND_MAX * (10));
            if (randomValue > 3) {
                step = 1;
                snipe();
            } else {
                step = 2;
            }
            cooldown = 0;
        }
        chrono.reset();
    }
}

void Ennemi095::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    switch (step) {
        case 0 :
            WindowManager::getInstance()->draw(image, direction == W ? 0 : 20, anim*22, 20, 22, dstX + 3, dstY + 3);
            break;
        case 1 :
            WindowManager::getInstance()->draw(image, 40, 0, 19, 25, dstX + 4, dstY);
            break;
        case 2 :
            WindowManager::getInstance()->draw(image, anim * 27, 44, 27, 24, dstX, dstY + 1);
            break;
    }
}

int Ennemi095::getX() {
    return x;
}

int Ennemi095::getY() {
    return y;
}

BoundingBox* Ennemi095::getBoundingBox() {
    return &box;
}

void Ennemi095::snipe() {

    // throw proj and play sound
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    double anglx = 0;
    double angly = 0;
    int origx = x + 13;
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

    ProjectileHelper::getInstance()->addProjectile(expert ? TP_BIG_FEU : TP_BOULE_FEU, origx, origy, anglx, angly);
    AudioManager::getInstance()->playSound(TS_THROW);
}
