#include "Ennemi144.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/FumeeBlanche.h"

#include "../helper/ProjectileHelper.h"

#define PI 3.14159265

Ennemi144::Ennemi144(int i, int j, Map* mp) : anim(0), animMax(2), vanim(180), cooldown(0), angle(0), radius(40), map(mp) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi144.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 25;
    height = 58;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(58);

    life = expert ? 20 : 10;
    maxLife = expert ? 20 : 10;
    recul = 0;
    vitesseRecul = 0;

    isBoss = map->getId() == 58;
    stunnable = false;

    for (int i = 0; i < 7; i++) flammes[i] = 0;

    forceEnn = expert ? 4 : 2;
}

Ennemi144::~Ennemi144() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi144::reset() {
    Ennemi::reset();
    chrono.reset();
    anim = 0;
    cooldown = 0;
    angle = 0;
    for (int i = 0; i < 7; i++) flammes[i] = 0;
}

bool Ennemi144::isResetable() {
    return !isBoss || alive;
}

void Ennemi144::setFlammes() {
    int fx = 0;
    int fy = 0;
    Direction fd = N;
    double fa = ((double)360) / ((double)7);
    double tmpa = 0;
    for (int i = 0; i < 7; i++) {

        tmpa = i * fa + angle;

        fx = ((double)radius) * cos(tmpa * PI / 180.0 );
        fy = -((double)radius) * sin(tmpa * PI / 180.0 );

        if (abs(fx) > abs(fy)) {
            fd = fx > 0 ? E : W;
        } else {
            fd = fy > 0 ? S : N;
        }

        if (flammes[i] == 0) {
            flammes[i] = new FlammeCirculaire(x + 9 + fx, y + 23 + fy, fd);
            map->addEffect(flammes[i]);
        } else {
            flammes[i]->move(x + 9 + fx, y + 23 + fy, fd);
        }
    }
}

void Ennemi144::ennLoop() {

    angle++;
    if (angle >= 360) angle = 0;
    setFlammes();

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
        if (cooldown == 20) {
            snipe();
            cooldown = 0;
        }
        chrono.reset();
    }
}

void Ennemi144::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, 0, 48, 17, 8, dstX + 5, dstY + 46+4);
    WindowManager::getInstance()->draw(image, anim * width, 0, width, 48, dstX, dstY);
}

void Ennemi144::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + dx);

    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi144::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + dy);

    if (map->checkCollisions(bb, this, false, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi144::getX() {
    return x;
}

int Ennemi144::getY() {
    return y;
}

BoundingBox* Ennemi144::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void Ennemi144::giveItem(int i, int j) {
    Ennemi::giveItem(i, j);
    for (int i = 0; i < 7; i++) if (flammes[i] != 0) flammes[i]->end();
    if (isBoss) {
        AudioManager::getInstance()->playMusic(20);
        map->addItem(ItemHelper::getInstance()->createItem(TI_COEUR, x + 13, y + 15, 5));
    }
}

void Ennemi144::snipe() {

    // throw proj and play sound
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    double anglx = 0;
    double angly = 0;
    int origx = x + 13;
    int origy = y + 15;
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
