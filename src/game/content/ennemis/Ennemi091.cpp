#include "Ennemi091.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi091::Ennemi091(int i, int j, Map* mp) : anim(0), animMax(1), vanim(180), animDispel(0), cooldown(0), map(mp) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi91.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 22;
    height = 33;

    box.setX(x + 4);
    box.setY(y);
    box.setW(16);
    box.setH(33);

    life = expert ? 20 : 7;
    maxLife = expert ? 20 : 7;
    recul = 0;
    vitesseRecul = 0;

    isBoss = map->getId() == 58;
    stunnable = false;

    forceEnn = expert ? 2 : 1;
}

Ennemi091::~Ennemi091() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi091::reset() {
    Ennemi::reset();
    chrono.reset();
    anim = 0;
    cooldown = 0;
    animDispel = 0;
}

bool Ennemi091::isResetable() {
    return !isBoss || alive;
}

void Ennemi091::ennLoop() {

    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 11 - dstX) + abs(y + 25 - dstY);
    if (dist <= 64) {
        if (animDispel < 15) animDispel++;
        cooldown = 0;
    } else {
        if (animDispel > 0) animDispel--;
    }

    // move
    int randomValue = (int)((float)rand() / RAND_MAX * (100));
    switch (randomValue) {
        case 1 : moveX(-1); direction=W; break;
        case 2 : moveX(1); direction=E; break;
        case 3 : moveY(-1); direction=N; break;
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

    if (animDispel == 0) testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        if (animDispel == 0)  {
            cooldown++;
            if (cooldown == 20) {
                snipe();
                cooldown = 0;
            }
        }
        chrono.reset();
    }
}

void Ennemi091::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (animDispel) {
        WindowManager::getInstance()->draw(image, ((animDispel/5) + 4) * width, 0, width, height, dstX, dstY);
    } else {
        Link* link = getLink();
        int lX = link->getX() + 8;
        WindowManager::getInstance()->draw(image, (anim + ((lX > x + 11) ? 2 : 0)) * width, 0, width, height, dstX, dstY);
    }
}

void Ennemi091::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + 4 + dx);

    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi091::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + dy);

    if (map->checkCollisions(bb, this, false, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi091::getX() {
    return x;
}

int Ennemi091::getY() {
    return y;
}

BoundingBox* Ennemi091::getBoundingBox() {
    box.setX(x + 4);
    box.setY(y);
    return &box;
}

void Ennemi091::snipe() {

    // throw proj and play sound
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    double anglx = 0;
    double angly = 0;
    int origx = x + 8 + 3 * direction % 2;
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

bool Ennemi091::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return animDispel == 0;
}
