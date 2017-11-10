#include "Ennemi092.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi092::Ennemi092(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi92.png", true);
    chrono.reset();

    cooldown = 0;

    x = i;
    y = j;

    // for quadtree operations:
    width = 121;
    height = 52;

    box.setX(x + 12);
    box.setY(y);
    box.setW(121 - 24);
    box.setH(32);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 60 : 40;
    maxLife = expert ? 60 : 40;
    recul = 0;
    vitesseRecul = 0;
    step = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 8 : 4;
}

Ennemi092::~Ennemi092() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi092::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    animMax = 1;
    cooldown = 0;
    step = 0;
    checkPosition();
}

bool Ennemi092::isResetable() {
    return alive;
}

void Ennemi092::ennLoop() {

    if (step == 0) {
        // move
        int randomValue = (int)((float)rand() / RAND_MAX * (50));
        switch (randomValue) {
            case 1 : case 3 : moveX(-1);direction=W; break;
            case 2 : case 4 : moveX(1); direction=E; break;
            default :
                if (randomValue < 10) break;
                switch (direction) {
                    case N : case W : moveX(-2); break;
                    case S : case E : moveX(2); break;
                    default : break;
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
            if (cooldown >= 10 - ((maxLife - life)/5)) {
                snipe();
                cooldown = 0;
            }
        }
        chrono.reset();
    }
}

void Ennemi092::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, anim * width, 0, width, height, dstX, dstY);
}

void Ennemi092::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + 12 + dx);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

int Ennemi092::getX() {
    return x;
}

int Ennemi092::getY() {
    return y;
}

BoundingBox* Ennemi092::getBoundingBox() {
    box.setX(x + 12);
    box.setY(y);
    return &box;
}

void Ennemi092::giveItem(int i, int j) {
    Ennemi::giveItem(i, j);
    AudioManager::getInstance()->playMusic(20);
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    map->addItem(ItemHelper::getInstance()->createItem(TI_COEUR, 50 * 16, 64 * 16 - 8, 7));
}

void Ennemi092::snipe() {

    // throw proj and play sound
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    double anglx = 0;
    double angly = 0;
    int destx = dstX;
    int desty = dstY - 8;

    int origx = x + 60;
    int origy = y + 31;

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

    ProjectileHelper::getInstance()->addProjectile(expert ? TP_BOULE_DEMI : TP_BOULE_FEU, origx, origy, anglx, angly);
    AudioManager::getInstance()->playSound(TS_THROW);
}
