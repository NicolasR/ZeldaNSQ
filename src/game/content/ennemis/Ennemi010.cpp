#include "Ennemi010.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi010::Ennemi010(int i, int j) : cooldown(63) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi10.png", true);

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

    life = expert ? 3 : 1;
    maxLife = expert ? 3 : 1;
    recul = 0;
    vitesseRecul = 0;
    idle = true;

    items.addType(TI_PETIT_COEUR, 20);
    items.addType(TI_FLECHE, 20);
    items.addType(TI_RUBIS_BLEU, 20);

    forceEnn = expert ? 4 : 2;
}

Ennemi010::~Ennemi010() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi010::reset() {
    Ennemi::reset();
    x = startX;
    y = startY;
    direction = startDir;
    cooldown = 63;
    checkPosition();
}

void Ennemi010::ennLoop() {

    if (cooldown) cooldown--;

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + width / 2 - dstX) + abs(y + height - dstY);
    if (dist <= maxDist) {
        if (link->getStatus()->isVisible() && !cooldown) {
            // throw proj and play sound
            double anglx = 0;
            double angly = 0;
            int origx = x + 8;
            int origy = y + 8;
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

            ProjectileHelper::getInstance()->addProjectile(TP_BOULE_FEU, x + 8, y + 8, anglx, angly);
            AudioManager::getInstance()->playSound(TS_THROW);
            cooldown = 128;
        }
        testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);
    }
}

void Ennemi010::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, (cooldown == 0 || cooldown < 64) ? 0 : 16, 0, width, height, dstX, dstY);
}

int Ennemi010::getX() {
    return x;
}

int Ennemi010::getY() {
    return y;
}

BoundingBox* Ennemi010::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}
