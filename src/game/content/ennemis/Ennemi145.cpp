#include "Ennemi145.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi145::Ennemi145(int i, int j) : step(1), cooldown(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi145.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 26;
    height = 23;

    box.setX(x);
    box.setY(y);
    box.setW(26);
    box.setH(23);

    life = expert ? 12 : 7;
    maxLife = expert ? 12 : 7;
    recul = 0;
    vitesseRecul = 0;

    stunnable = false;
    isBoss = true;
}

Ennemi145::~Ennemi145() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi145::reset() {
    Ennemi::reset();
    chrono.reset();
    step = 1;
    cooldown = 0;
}

bool Ennemi145::isResetable() {
    return alive;
}

void Ennemi145::ennLoop() {

    switch (step) {
        case 0 : // wait
            cooldown++;
            if (cooldown >= 64) {//126, 63
                int dest = (int)((float)rand() / RAND_MAX * (9));
                x = 126 * 16 + 3 + 48 * (dest % 3);
                y = 63 * 16 + 48 * (dest / 3);
                step = ((float)rand() / RAND_MAX * (100) > 25) ? 1 : 3;
                if (step == 1) {
                    box.setX(x+6);
                    box.setW(14);
                } else {
                    box.setX(x);
                    box.setW(26);
                }
                box.setY(y+23);
                box.setH(0);
                checkPosition();
                cooldown = 0;
            }
            break;
        case 1 : // up
            cooldown++;
            box.setY(y+23-cooldown);
            box.setH(cooldown);
            checkPosition();
            if (cooldown == 11) {
                step = 6;
                cooldown = 0;
            }
            break;
        case 2 : // down
            cooldown--;
            box.setY(y+23-cooldown);
            box.setH(cooldown);
            checkPosition();
            if (cooldown == 0) {
                step = 0;
                box.setH(-1);
            }
            break;
        case 3 : // jump
            cooldown+=2;
            box.setY(y+23-cooldown);
            box.setH(cooldown);
            checkPosition();
            if (cooldown == 32) {
                step = 4;
                cooldown = 0;
            }
            break;
        case 4 : // fire
            cooldown++;
            if (cooldown == 8) {
                snipe();
                step = 5;
                cooldown = 32;
            }
            break;
        case 5 : // fall
            cooldown--;
            box.setY(y+23-cooldown);
            box.setH(cooldown);
            if (cooldown == 0) {
                step = 0;
                box.setH(-1);
            }
            break;
        case 6 :
            cooldown++;
            if (cooldown == 16) {
                step = 2;
                cooldown = 11;
            }
            break;
    }
}

void Ennemi145::draw(int offsetX, int offsetY) {
    if (!alive || !step) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    switch (step) {
        case 1 : case 2 : // head only
            WindowManager::getInstance()->draw(image, 26, 0, 14, cooldown, dstX + 6, dstY + 23 - cooldown); // max h = 11
            break;
        case 3 : case 5 : // jump or fall
            WindowManager::getInstance()->draw(image, 0, 0, 26, cooldown > 23 ? 23 : cooldown, dstX, dstY + 23 - cooldown); // max h = 23 + 9
            break;
        case 4 :
            WindowManager::getInstance()->draw(image, 0, 0, 26, 23, dstX, dstY - 9); // max h = 23 + 9
            break;
        case 6 : // head only
            WindowManager::getInstance()->draw(image, 26, 0, 14, 11, dstX + 6, dstY + 23 - 11); // max h = 11
            break;
    }
}

int Ennemi145::getX() {
    return x;
}

int Ennemi145::getY() {
    return y;
}

BoundingBox* Ennemi145::getBoundingBox() {
    return &box;
}

void Ennemi145::snipe() {

    // throw proj and play sound
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    double anglx = 0;
    double angly = 0;
    int origx = x + 13;
    int origy = y + 9;
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

    ProjectileHelper::getInstance()->addProjectile(TP_BOULE_FEU, origx, origy, anglx, angly);
    AudioManager::getInstance()->playSound(TS_THROW);
}

bool Ennemi145::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return step;
}
