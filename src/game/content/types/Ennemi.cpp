#include "Ennemi.h"

#include "../../MainController.h"

#include "../../../engine/audio/AudioManager.h"

#include "../effects/FumeeBlanche.h"

Ennemi::Ennemi() : life(1), maxLife(1), invul(0), recul(32), vitesseRecul(2), maxDist(200), direction(S),
    startX(0), startY(0), startDir(S), idle(true), gel(0), maxGel(0), isBoss(false), stunnable(true),
    link(0), toucheX(0), toucheY(0), reculRestant(0) {
    expert = MainController::getInstance()->getGameController()->getSave()->isExpert();
}

Ennemi::~Ennemi() {
}

void Ennemi::loop() {

    if (isBoss) {
        MainController::getInstance()->getGameController()->getSceneController()->getScene()->getHud()->setBossLife(life, maxLife);
    }

    if (invul) invul--;
    if (gel) gel--;
    if (toucheX != 0 || toucheY != 0) {
        computeProjection();
        return;
    }

    // test vide
    if (isOnGap() && isToAvoid(TROU)) {
        AudioManager::getInstance()->playSound(TS_FALL, 1);
        if (isBoss) {
            MainController::getInstance()->getGameController()->getSceneController()->getScene()->getHud()->setBossLife(0, maxLife);
        }
        alive = false;
        MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->testAnimRoom();
    }

    if ((!invul || !stunnable) && !gel) ennLoop();
}

bool Ennemi::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return true;
}

void Ennemi::underAttack(Direction dir, int force, TypeAttack type, TypeEffect effect, BoundingBox* attBox) {

    if (invul || !attBox->intersect(getBoundingBoxForDamage()) || !hasEffect(type, effect, dir)) {
        return;
    }

    if (effect == TE_GRAVITE) {
        force = (life + 1) / 2;
    }

    life -= force;


    if (life <= 0) {

        if (isBoss) {
            MainController::getInstance()->getGameController()->getSceneController()->getScene()->getHud()->setBossLife(0, maxLife);
        }

        alive = false;

        BoundingBox* bb = getBoundingBox();
        int cx = bb->getX() + bb->getW() / 2;
        int cy = bb->getY() + bb->getH() / 2;

        giveItem(cx, cy);

        MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->testAnimRoom();
    } else {
        AudioManager::getInstance()->playSound(TS_HITENNEMY);

        if (effect == TE_GLACE) {
            gel = maxGel;
        }

        jumpBack(dir);
    }

}

void Ennemi::jumpBack(Direction dir) {
    invul = 16;

    if (recul > 0) {
        switch (dir) {
            case N : toucheX = 0; toucheY = -1; direction = S; break;
            case S : toucheX = 0; toucheY = 1; direction = N; break;
            case W : toucheX = -1; toucheY = 0; direction = E; break;
            case E : toucheX = 1; toucheY = 0; direction = W; break;
        }
        reculRestant = recul;
    }
}

void Ennemi::giveItem(int x, int y) {
    AudioManager::getInstance()->playSound(TS_KILLENNEMY);
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    map->addEffect(new FumeeBlanche(x, y));
    TypeItem type = items.selectType();
    if (type != TI_NO_ITEM) {
        map->addItem(ItemHelper::getInstance()->createItem(type, x, y));
    }
}

Link* Ennemi::getLink() {
    if (link == 0) {
        link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
    }
    return link;
}

bool Ennemi::testDegatOnLink(BoundingBox* box, Direction dir, int force, TypeAttack type, TypeEffect effect) {
    return MainController::getInstance()->getGameController()->getSceneController()->getScene()->testDegatOnLink(box, dir, force, type, effect);
}

bool Ennemi::isIdle() {
    return idle;
}

void Ennemi::reset() {
    alive = true;
    life = maxLife;
    idle = true;
    gel = 0;
}

bool Ennemi::isResetable() {
    return true;
}

void Ennemi::move(int dx, int dy, bool ignoreIdle) {

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    int oldX = x;
    int oldY = y;

    x += dx;
    if (!scene->checkCollisions(getBoundingBox(), this, true, ignoreIdle)) {
        x = oldX;
    }

    y += dy;
    if (!scene->checkCollisions(getBoundingBox(), this, true, ignoreIdle)) {
        y = oldY;
    }

    if (toucheX == 0 && toucheY == 0) {

        if (x > oldX && (direction == W || y == oldY)) direction = E;
        if (x < oldX && (direction == E || y == oldY)) direction = W;
        if (y > oldY && (direction == N || x == oldX)) direction = S;
        if (y < oldY && (direction == S || x == oldX)) direction = N;

    }

    idle = (x == oldX && y == oldY);
    if (!idle) checkPosition();
}

void Ennemi::computeProjection() {
    if (toucheY != 0) {
        int prevY = y;
        move(0, toucheY * vitesseRecul, false);
        reculRestant -= vitesseRecul;
        if (prevY == y || reculRestant == 0) toucheY = 0;
    }
    if (toucheX != 0) {
        int prevX = x;
        move(toucheX * vitesseRecul, 0, false);
        reculRestant -= vitesseRecul;
        if (prevX == x || reculRestant == 0) toucheX = 0;
    }
}

BoundingBox* Ennemi::getBoundingBoxForDamage() {
    return getBoundingBox();
}
