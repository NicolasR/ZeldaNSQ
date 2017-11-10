#include "Ennemi142.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/FumeeBlanche.h"

Ennemi142::Ennemi142(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi142.png", true);
    chrono.reset();

    beforeJump = 30;
    jump = 0;
    dstJumpX = 0;
    dstJumpY = 0;
    dx = 0;
    dy = 0;
    trueX = 0;
    trueY = 0;

    x = i;
    y = j;

    // for quadtree operations:
    width = 39;
    height = 51;

    box.setX(x+4);
    box.setY(y+35);
    box.setW(32);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 50 : 25;
    maxLife = expert ? 50 : 25;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 24 : 16;
}

Ennemi142::~Ennemi142() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi142::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    beforeJump = 30;
    jump = 0;
    dstJumpX = 0;
    dstJumpY = 0;
    dx = 0;
    dy = 0;
    trueX = 0;
    trueY = 0;
    checkPosition();
}

bool Ennemi142::isResetable() {
    return alive;
}

void Ennemi142::ennLoop() {

    if (!jump) {

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

    } else {

        if (jump > 2) {

            trueX += dx;
            trueY += dy;

            x = (int)trueX;
            y = (int)trueY;

            if (dy < 0 && y < dstJumpY) {
                y = dstJumpY;
                dy = 0;
            } else if (dy > 0 && y > dstJumpY) {
                y = dstJumpY;
                dy = 0;
            }

            if (dx < 0 && x < dstJumpX) {
                x = dstJumpX;
                dx = 0;
            } else if (dx > 0 && x > dstJumpX) {
                x = dstJumpX;
                dx = 0;
            }

            if (x == dstJumpX && y == dstJumpY) {
                jump = 0;
                beforeJump = 30;
            }
        }

        testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
    }

    if (chrono.getElapsedTime() >= vanim) {
        if (!gel) anim++;
        if (anim > animMax) {
            anim = 0;
        }
        if (jump > 0 && jump < 3) {
            jump++;
            if (jump == 3) {
                jumpToLink();
            }
        }
        if (beforeJump > 0) {
            beforeJump--;
            if (beforeJump == 0) {
                jump=1;
            }
        }
        chrono.reset();
    }
}

void Ennemi142::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    switch (jump) {
        case 0 :
            WindowManager::getInstance()->draw(image, direction * width, anim * height, width, height, dstX, dstY);
            break;
        case 1 :
            WindowManager::getInstance()->draw(image, 0, 102, width, height, dstX, dstY);
            break;
        case 2 :
            WindowManager::getInstance()->draw(image, 39, 102, width, height, dstX, dstY);
            break;
        case 3 :
            WindowManager::getInstance()->draw(image, 78, 102, width, height + 2, dstX, dstY - 2);
            break;
    }
}

int Ennemi142::getX() {
    return x;
}

int Ennemi142::getY() {
    return y;
}

BoundingBox* Ennemi142::getBoundingBox() {
    box.setX(x + 4);
    box.setY(y + 35);
    return &box;
}

bool Ennemi142::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return type == TA_EXPLO;
}

void Ennemi142::giveItem(int x, int y) {
    AudioManager::getInstance()->playSound(TS_KILLENNEMY);
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    map->addEffect(new FumeeBlanche(x, y));
    map->addItem(ItemHelper::getInstance()->createItem(TI_COEUR, x, y, 1));
}

void Ennemi142::jumpToLink() {
    AudioManager::getInstance()->playSound(TS_THROW);

    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    dstJumpX = dstX - 19;
    dstJumpY = dstY - 51;

    if (dstJumpX < 24+32) dstJumpX = 24+32;
    if (dstJumpX > 256-32) dstJumpX = 256-32;
    if (dstJumpY < 209) dstJumpY = 209;
    if (dstJumpY > 405) dstJumpY = 405;

    dx = 0;
    dy = 0;

    if (dstJumpX-x != 0 || dstJumpY-y != 0) {

        double coef1 = 0;
        double coef2 = 0;

        if ((dstJumpX-x) == 0) {dx=0; dy=4;}
            else if ((dstJumpY-y) == 0) {dx=4; dy=0;}
            else {
                coef1=((double)(dstJumpY-y))/((double)(dstJumpX-x));
                coef2=((double)(dstJumpX-x))/((double)(dstJumpY-y));
                dx=(sqrt(12/(1+(coef1*coef1))));
                dy=(sqrt(12/(1+(coef2*coef2))));
            }
            if (dstJumpX - x < 0) dx = -dx;
            if (dstJumpY - y < 0) dy = -dy;

        if (dx>4) dx=4;
        if (dy>4) dy=4;
        if (dx<-4) dx=-4;
        if (dy<-4) dy=-4;
    }

    trueX = x;
    trueY = y;
}
