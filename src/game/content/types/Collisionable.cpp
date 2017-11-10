#include "Collisionable.h"

#include "../scene/Map.h"

#include "../../MainController.h"

Collisionable::Collisionable() {
}

Collisionable::~Collisionable() {
}

bool Collisionable::isCollision(Collision c) {

    switch (c) {
        case TROU : case HERBE : case HERBE_HAUTE : case TERRE : case EAU :
        case EAU_PROF : case GLACE : case SABLE : case NEIGE : case PIERRE :
        case SOL_BOIS : case TAPIS : case DANGER_BAS : return true;
        case MURRET : case MUR : return false;
        default : return false;
    }
}

bool Collisionable::isToAvoid(Collision c) {
    switch (c) {
        case TROU : case EAU : case EAU_PROF : return true;
        default : return false;
    }
}

bool Collisionable::isOnIce() {
    BoundingBox* box = getBoundingBox();
    int x = box->getX();
    int y = box->getY();
    int w = box->getW();
    int h = box->getH();

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    for (int j = (y / 8) * 8; j < y + h; j += 8) {
        for (int i = (x / 8) * 8; i < x + w; i += 8) {
            if (map->getMur(i, j) == GLACE) return true;
        }
    }
    return false;
}

bool Collisionable::isOnWater() {
    BoundingBox* box = getBoundingBox();
    int x = box->getX();
    int y = box->getY();
    int w = box->getW();
    int h = box->getH();

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    for (int j = (y / 8) * 8; j < y + h; j += 8) {
        for (int i = (x / 8) * 8; i < x + w; i += 8) {
            if (map->getMur(i, j) != EAU_PROF) return false;
        }
    }
    return true;
}

bool Collisionable::isOnGap() {
    BoundingBox* box = getBoundingBox();
    int x = box->getX();
    int y = box->getY();
    int w = box->getW();
    int h = box->getH();

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    for (int j = (y / 8) * 8; j < y + h; j += 8) {
        for (int i = (x / 8) * 8; i < x + w; i += 8) {
            if (map->getMur(i, j) != TROU) return false;
        }
    }
    return true;
}

bool Collisionable::isIdle() {
    return false;
}
