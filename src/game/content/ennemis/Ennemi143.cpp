#include "Ennemi143.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi143::Ennemi143(int i, int j) : anim(0), animMax(5), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi143.png", true);
    chrono.reset();

    beforeJump = 0;
    jump = 0;
    beforeShoot = 4;

    x = i;
    y = j;
    direction = E;

    // for quadtree operations:
    width = 46;
    height = 80;

    box.setX(x+12);
    box.setY(y+16);
    box.setW(22);
    box.setH(64);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 100 : 70;
    maxLife = expert ? 100 : 70;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 32 : 16;
}

Ennemi143::~Ennemi143() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi143::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    beforeJump = 0;
    jump = 0;
    beforeShoot = 4;
    checkPosition();
}

bool Ennemi143::isResetable() {
    return alive;
}

void Ennemi143::ennLoop() {

    if (!jump) {

        Link* link = getLink();

        if (link->getBoundingBox()->getX() + 8 < x + 23) {
            direction = W;
        } else {
            direction = E;
        }

        testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);

    } else {

        if (jump == 1) {
            beforeJump-=5;
            image->setAlpha(beforeJump);

            if (beforeJump == 0) {
                if (life <= 0) {
                    alive = false;
                } else {
                    jump = 2;
                    jumpToLink();
                }
            }
        } else if (jump == 2) {
            beforeJump+=5;
            image->setAlpha(beforeJump);

            if (beforeJump == 255) {
                jump = 0;
                anim = 0;
                animMax = 5;
            }
        }
    }

    if (chrono.getElapsedTime() >= vanim) {
        if (!jump) {
            beforeShoot--;
            if (beforeShoot == 0) {
                beforeShoot = 4;
                snipe();
            }
        }
        if (!gel) anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Ennemi143::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (jump != 1) {
        WindowManager::getInstance()->draw(image, anim * 45, (direction % 2) * 80, 45, 80, dstX, dstY);
    } else {
        WindowManager::getInstance()->draw(image, anim * 45, (direction % 2) * 80 + 160, anim == 2 ? 46 : 45, 80, dstX, dstY);
    }
}

int Ennemi143::getX() {
    return x;
}

int Ennemi143::getY() {
    return y;
}

BoundingBox* Ennemi143::getBoundingBox() {
    box.setX(x + 12);
    box.setY(y + 16);
    return &box;
}

bool Ennemi143::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    //TA_SWORD, TA_SWORD_HOLD, TA_ARROW
    Link* link = getLink();
    bool result = jump == 0 && (((type == TA_SWORD || type == TA_SWORD_HOLD) && link->getEpee() >= 2)
        || (type == TA_ARROW && link->getInventory()->hasObject(ARC_FEE)));
    if (result) {
        jump = 1; // = begin to vanish
        anim = 0;
        animMax = 2;
        beforeJump = 255;
        image->setAlpha(beforeJump);
    }
    return result;
}

void Ennemi143::giveItem(int x, int y) {
    alive = true;
    jump = 1; // = begin to vanish
    anim = 0;
    animMax = 2;
    beforeJump = 255;

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->getId() == 63) {
        AudioManager::getInstance()->playMusic(20);
        map->addItem(ItemHelper::getInstance()->createItem(TI_COEUR, x, y, 10));
    } else {
        AudioManager::getInstance()->playMusic(18);
        map->addItem(ItemHelper::getInstance()->createItem(TI_COEUR, x, y, 2));
    }
}

void Ennemi143::jumpToLink() {

    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    x = dstX - 22;
    y = dstY - 80;


    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->getId() == 63) {
        if (x < 320 * 2 + 40 + 32) x = 320 * 2 + 40 + 32;
        if (x > 320 * 2 + 235 - 32) x = 320 * 2 + 235 - 32;
        if (y < 240 + 8) y = 240 + 8;
        if (y > 240 + 136) y = 240 + 136;
    } else {
        if (x < 360 + 32) x = 360 + 32;
        if (x > 555 - 32) x = 555 - 32;
        if (y < 248) y = 248;
        if (y > 376) y = 376;
    }
}

void Ennemi143::snipe() {

    // throw proj and play sound
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    double anglx = 0;
    double angly = 0;
    int origx = direction == W ? x + 17 : x + 27;
    int origy = y + 27;
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

    ProjectileHelper::getInstance()->addProjectile(expert ? TP_BOULE_MORT : TP_BOULE_DEMI, origx, origy, anglx, angly);
    AudioManager::getInstance()->playSound(TS_THROW);
}
