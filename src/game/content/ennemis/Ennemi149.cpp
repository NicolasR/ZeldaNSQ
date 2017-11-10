#include "Ennemi149.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#define PI 3.14159265

Ennemi149::Ennemi149(int i, int j) : anim(0), animMax(1), vanim(360), cooldown(64), angle(0), radius(0), step(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi149.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 31;
    height = 34;

    box.setX(x+8);
    box.setY(y+15);
    box.setW(16);
    box.setH(16);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 10 : 5;
    maxLife = expert ? 10 : 5;
    recul = 0;
    vitesseRecul = 0;
    maxGel = 128;

    items.addType(TI_RUBIS_VERT, 20);
    items.addType(TI_RUBIS_BLEU, 10);
    items.addType(TI_FLECHE, 20);

    forceEnn = expert ? 8 : 4;
}

Ennemi149::~Ennemi149() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi149::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    animMax = 1;
    cooldown = 64;
    angle = 0;
    radius = 0;
    step = 0;
    checkPosition();
}

void Ennemi149::ennLoop() {
    if (step == 0 && cooldown) cooldown--;

    // retrieve target position ( = link ^^)
    Link* link = getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    int dist = abs(x + 16 - dstX) + abs(y + 31 - dstY);
    if (dist <= maxDist) {
        if (link->getStatus()->isVisible() && step == 0 && cooldown) {
            pair<int, int> dir = AStar::getInstance()->resolvePath(this, dstX, dstY, direction);

            move(dir.first, dir.second);

        } else {


            idle = true;

            if (step == 0 && !cooldown) {
                direction = (getLink()->getX() + 8 >= x + 15) ? E : W;
                step = 1;
                anim = 0;
                animMax = 0;
                chrono.reset();
            }
        }
    } else {
        idle = true;
    }
    testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);

    if (step == 2) {
        radius++;
        angle+=8;
        placeProj();
        if (radius == 80) {
            step = 3;
        }
    } else if (step == 3) {
        radius--;
        angle+=8;
        placeProj();
        if (radius == 0) {
            step = 0;
            cooldown = 64;
            vanim = 360;
            animMax = 1;
            anim = 0;
            chrono.reset();
        }
    }
    if (step > 1) {
        BoundingBox bb(projX - 8, projY - 8, 16, 16);
        testDegatOnLink(&bb, projDir, forceEnn, TA_PHYSIC, TE_NORMAL);
    }

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        if (step == 1 && anim == 0) {
            step = 2;
            animMax = 3;
            vanim = 180;
            angle = 0;
            radius = 0;

            if (direction == W) {
                projX = x + 10;
            } else {
                projX = x + 13;
            }
            projY = y + 28;
        }
        chrono.reset();
    }
}

void Ennemi149::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (invul || gel) {
        WindowManager::getInstance()->draw(image, (direction == W || direction == N ? 55 : 0), 50 + (gel ? 32 : 0), 31, 31, dstX, dstY);
    } else {

        Direction dir = direction;
        if (step == 0) dir = (getLink()->getX() + 8 >= x + 15) ? E : W;


        switch (step) {
            case 0 : WindowManager::getInstance()->draw(image, anim * 23 + (dir == W ? 55 : 0), 0, 23, 23, dstX+4, dstY+7); break;
            case 1 : WindowManager::getInstance()->draw(image, (dir == W ? 55 : 0), 23, 28, 24, dstX+1, dstY+6); break;
            case 2 : case 3 : WindowManager::getInstance()->draw(image, 28 +(dir == W ? 55 : 0), 23, 25, 27, dstX+3+(dir == W ? -2 : 2), dstY+8); break;
        }
    }
    if (step > 1) {
        switch (anim) {
            case 0 : WindowManager::getInstance()->draw(image, 31, 50 + (gel ? 32 : 0), 8, 16, projX - 4 - offsetX, projY - 8 - offsetY); break;
            case 1 : WindowManager::getInstance()->draw(image, 39, 50 + (gel ? 32 : 0), 16, 8, projX - 8 - offsetX, projY - 4 - offsetY); break;
            case 2 : WindowManager::getInstance()->draw(image, 31, 66 + (gel ? 32 : 0), 8, 16, projX - 4 - offsetX, projY - 8 - offsetY); break;
            case 3 : WindowManager::getInstance()->draw(image, 39, 58 + (gel ? 32 : 0), 16, 8, projX - 8 - offsetX, projY - 4 - offsetY); break;
        }
    }
}

int Ennemi149::getX() {
    return x;
}

int Ennemi149::getY() {
    return y;
}

BoundingBox* Ennemi149::getBoundingBox() {
    box.setX(x+8);
    box.setY(y+15);
    return &box;
}

void Ennemi149::placeProj() {
    int fx = 0;
    int fy = 0;

    fx = ((double)radius) * cos(angle * PI / 180.0 );
    fy = -((double)radius) * sin(angle * PI / 180.0 );

    if (abs(fx) > abs(fy)) {
        projDir = fx > 0 && direction == E ? E : W;
    } else {
        projDir = fy > 0 ? S : N;
    }

    if (direction == W) {
        projX = ((double)x + 10) - fx;
    } else {
        projX = ((double)x + 13) + fx;
    }
    projY = ((double)y + 28) + fy;
}
