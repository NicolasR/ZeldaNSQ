#include "Ennemi076.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/FumeeBlanche.h"

#include "../helper/ProjectileHelper.h"

Ennemi076::Ennemi076(int i, int j) : anim(0), animMax(5), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi76.png", true);
    chrono.reset();

    cooldown = 0;

    x = i;
    y = j;

    // for quadtree operations:
    width = 60;
    height = 51;

    box.setX(x+6);
    box.setY(y+11);
    box.setW(48);
    box.setH(32);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 300 : 150;
    maxLife = expert ? 300 : 150;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 50 : 25;
}

Ennemi076::~Ennemi076() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi076::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    cooldown = 0;
    checkPosition();
}

bool Ennemi076::isResetable() {
    return alive;
}

void Ennemi076::ennLoop() {

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    if (link->getStatus()->isVisible()) {
        pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

        move(dir.first, dir.second);

        if (link->getBoundingBox()->intersect(getBoundingBox())) {
            testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
        }
    } else {
        idle = true;
        testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);
    }


    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        cooldown++;
        if (cooldown == 16) {
            snipe();
            cooldown = 0;
        }
        chrono.reset();
    }
}

void Ennemi076::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, (direction % 2) * width, anim * height, width, height, dstX, dstY);
}

void Ennemi076::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + 6 + dx);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi076::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + 11 + dy);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    if (map->checkCollisions(bb, this, false, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi076::getX() {
    return x;
}

int Ennemi076::getY() {
    return y;
}

BoundingBox* Ennemi076::getBoundingBox() {
    box.setX(x + 6);
    box.setY(y + 11);
    return &box;
}

void Ennemi076::giveItem(int i, int j) {
    Ennemi::giveItem(i, j);
    if (isBoss) {
        AudioManager::getInstance()->playMusic(20);
        Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
        map->addItem(ItemHelper::getInstance()->createItem(TI_COEUR, x + 30, y + 51 - 8, 12));
    }
}

void Ennemi076::snipe() {
    if (expert) {
        ProjectileHelper::getInstance()->addProjectile(TP_BOULE_ULTIME, x + ((direction % 2) ? 24 : 36), y + 42, getLink());
        AudioManager::getInstance()->playSound(TS_THROW);
        return;
    }

    // throw proj and play sound
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    double anglx = 0;
    double angly = 0;
    int origx = x + ((direction % 2) ? 24 : 36);
    int origy = y + 42;
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

    ProjectileHelper::getInstance()->addProjectile(TP_BIG_GLACE, origx, origy, anglx, angly);
    AudioManager::getInstance()->playSound(TS_THROW);
}
