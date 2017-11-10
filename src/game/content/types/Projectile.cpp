#include "Projectile.h"

Projectile::Projectile() : anim(0), animMax(20), vanim(30) {
}

Projectile::~Projectile() {
}


void Projectile::loop() {
    int oldX = x;
    int oldY = y;
    int oldW = width;
    int oldH = height;

    projLoop();

    bool idle = (x == oldX && y == oldY);
    if (!idle) checkPosition();
    if (oldW != width || oldH != height) {
        computeMaxSize();
    }
}

int Projectile::getX() {return x;}
int Projectile::getY() {return y;}
BoundingBox* Projectile::getBoundingBox() {return &box;}

bool Projectile::isCollision(Collision c) {
    if ((c == MUR_H || c == MUR_B) && (direction == W || direction == E)) {
        return true;
    }
    if ((c == MUR_G || c == MUR_D) && (direction == N || direction == S)) {
        return true;
    }

    switch (c) {
        case TROU : case HERBE : case HERBE_HAUTE : case TERRE : case EAU :
        case EAU_PROF : case GLACE : case MURRET : case SABLE : case NEIGE :
        case PIERRE : case SOL_BOIS : case TAPIS : case DANGER_BAS : return true;
        default : return false;
    }
}

void Projectile::renvoie(Direction dir) {}
