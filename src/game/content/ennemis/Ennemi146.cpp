#include "Ennemi146.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/FumeeBlanche.h"

#include "../helper/ProjectileHelper.h"

#define PI 3.14159265

Ennemi146::Ennemi146(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi146.png", true);
    chrono.reset();

    step = 0;
    cooldown = 0;

    x = i;
    y = j;

    // for quadtree operations:
    width = 50;
    height = 50;

    box.setX(x+9);
    box.setY(y+18);
    box.setW(32);
    box.setH(32);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 30 : 20;
    maxLife = expert ? 30 : 20;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 4 : 2;
}

Ennemi146::~Ennemi146() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi146::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    animMax = 1;
    step = 0;
    cooldown = 0;
    checkPosition();
}

bool Ennemi146::isResetable() {
    return alive;
}

void Ennemi146::ennLoop() {

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
        testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);
    } else if (step == 1) {

        Link* link = getLink();

        int dstX = link->getX() + 8;
        int dstY = link->getY() + 24;

        int srcX = x + 25;
        int srcY = y + 50;

        if (abs(dstX - srcX) > abs(dstY - srcY)) {
            direction = (dstX > srcX) ? E : W;
        } else {
            direction = (dstY > srcY) ? S : N;
        }

        testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);
    } else if (step == 2) {
        testDegatOnLink(getBoundingBox(), S, forceEnn, TA_PHYSIC, TE_NORMAL);
    }


    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        if (step == 0) {
            cooldown++;
            if (cooldown == 20) {
                if (life > maxLife / 2) {
                    step = 1;
                    animMax = 3;
                } else {
                    step = 2;
                    animMax = 19;
                }
                anim = 0;
                cooldown = 0;
            }
        } else {
            if (anim == 0) {
                step = 0;
                animMax = 1;
            } else {
                if (step == 1) {
                    if (anim == 3) {
                        snipe();
                    }
                } else {
                    if (anim == 8) {
                        multi();
                    }
                }
            }
        }
        chrono.reset();
    }
}

void Ennemi146::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    switch (step) {
        case 0 :
            WindowManager::getInstance()->draw(image, direction * width, anim * height, width, height, dstX, dstY);
            break;
        case 1 :
            if (direction == N) {
                WindowManager::getInstance()->draw(image, anim * width, 100, width, 57, dstX, dstY - 7);
                break;
            }
            else if (direction == W) dstX -= 9;
            else if (direction == E) dstX += 9;
            WindowManager::getInstance()->draw(image, anim * width, 107 + direction * height, width, height, dstX, dstY);
            break;
        case 2 :
            WindowManager::getInstance()->draw(image, 56 * (anim % 4), 307 + 62 * (anim / 4), 56, 62, dstX - 3, dstY - 12);
            break;
    }
}

void Ennemi146::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + 8 + dx);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi146::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + 18 + dy);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    if (map->checkCollisions(bb, this, false, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi146::getX() {
    return x;
}

int Ennemi146::getY() {
    return y;
}

BoundingBox* Ennemi146::getBoundingBox() {
    box.setX(x + 8);
    box.setY(y + 18);
    return &box;
}

bool Ennemi146::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return type == TA_MASSIF || type == TA_EXPLO || type == TA_MARTEAU;
}

void Ennemi146::giveItem(int i, int j) {
    Ennemi::giveItem(i, j);
    if (isBoss) {
        AudioManager::getInstance()->playMusic(20);
        Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
        map->addItem(ItemHelper::getInstance()->createItem(TI_COEUR, x + 25, y + 50 - 8, 6));
    }
}

void Ennemi146::snipe() {

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
    switch (direction) {
        case N : origx = x + 39; origy = y; break;
        case S : origx = x + 12; origy = y + 27; break;
        case W : origx = x - 4; origy = y + 14; break;
        case E : origx = x + 54; origy = y + 14; break;
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

    ProjectileHelper::getInstance()->addProjectile(TP_PIERRE, origx, origy, anglx, angly);
    AudioManager::getInstance()->playSound(TS_THROW);
}

void Ennemi146::multi() {

    int angle = (int)((float)rand() / RAND_MAX * (360));
    double radius = 4;
    int nbPierres = 9;

    double fx = 0;
    double fy = 0;
    Direction fd = N;
    double fa = ((double)360) / ((double)nbPierres);
    double tmpa = 0;
    for (int i = 0; i < nbPierres; i++) {

        tmpa = i * fa + angle;

        fx = radius * cos(tmpa * PI / 180.0 );
        fy = -radius * sin(tmpa * PI / 180.0 );

        if (abs(fx) > abs(fy)) {
            fd = fx > 0 ? E : W;
        } else {
            fd = fy > 0 ? S : N;
        }

        ProjectileHelper::getInstance()->addProjectile(TP_PIERRE, x + 25, y + 25, fx, fy);
    }


    AudioManager::getInstance()->playSound(TS_THROW);
}
