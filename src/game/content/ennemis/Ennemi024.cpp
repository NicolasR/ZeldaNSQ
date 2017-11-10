#include "Ennemi024.h"

#include "Ennemi022.h"
#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/FumeeBlanche.h"

Ennemi024::Ennemi024(int i, int j, bool water) : anim(0), animMax(1), vanim(180), cooldown(30), inWater(water) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi24.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 32;
    height = water ? 38 : 48;

    box.setX(x);
    box.setY(water ? y : y + 16);
    box.setW(32);
    box.setH(32);

    life = expert ? 80 : 40;
    maxLife = expert ? 80 : 40;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    vitesse = water ? 2 : 1;

    forceEnn = expert ? 8 : 4;
}

Ennemi024::~Ennemi024() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi024::reset() {
    Ennemi::reset();
    chrono.reset();
    anim = 0;
    cooldown = 30;
}

bool Ennemi024::isResetable() {
    return alive;
}

void Ennemi024::ennLoop() {

    // move
    int randomValue = (int)((float)rand() / RAND_MAX * (100));
    switch (randomValue) {
        case 1 : moveX(-vitesse);direction=W; break;
        case 2 : moveX(vitesse); direction=E; break;
        case 3 : moveY(-vitesse);direction=N; break;
        case 4 : moveY(vitesse); direction=S; break;
        default :
            if (randomValue < 10) break;
            switch (direction) {
                case N : moveY(-vitesse); break;
                case S : moveY(vitesse); break;
                case W : moveX(-vitesse); break;
                case E : moveX(vitesse); break;
            }
            break;
    }

    testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        cooldown++;
        if (cooldown == 60) {

            Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

            if (x < 140*16) {
                BoundingBox bb1(63*16,17*16,16,16);
                if (map->checkCollisions(&bb1, (Collisionable*)getLink(), true, false, false, false)) {
                    map->addEnnemi(new Ennemi022(63*16,17*16-7,240));
                }
                BoundingBox bb2(76*16,17*16,16,16);
                if (map->checkCollisions(&bb2, (Collisionable*)getLink(), true, false, false, false)) {
                    map->addEnnemi(new Ennemi022(76*16,17*16-7,240));
                }
                BoundingBox bb3(63*16,27*16,16,16);
                if (map->checkCollisions(&bb3, (Collisionable*)getLink(), true, false, false, false)) {
                    map->addEnnemi(new Ennemi022(63*16,27*16-7,240));
                }
                BoundingBox bb4(76*16,27*16,16,16);
                if (map->checkCollisions(&bb4, (Collisionable*)getLink(), true, false, false, false)) {
                    map->addEnnemi(new Ennemi022(76*16,27*16-7,240));
                }
            } else {
                BoundingBox bb1(203*16,17*16,16,16);
                if (map->checkCollisions(&bb1, (Collisionable*)getLink(), true, false, false, false)) {
                    map->addEnnemi(new Ennemi022(203*16,17*16-7,240));
                }
                BoundingBox bb2(216*16,17*16,16,16);
                if (map->checkCollisions(&bb2, (Collisionable*)getLink(), true, false, false, false)) {
                    map->addEnnemi(new Ennemi022(216*16,17*16-7,240));
                }
                BoundingBox bb3(203*16,27*16,16,16);
                if (map->checkCollisions(&bb3, (Collisionable*)getLink(), true, false, false, false)) {
                    map->addEnnemi(new Ennemi022(203*16,27*16-7,240));
                }
                BoundingBox bb4(216*16,27*16,16,16);
                if (map->checkCollisions(&bb4, (Collisionable*)getLink(), true, false, false, false)) {
                    map->addEnnemi(new Ennemi022(216*16,27*16-7,240));
                }
            }


            cooldown = 0;
        }
        chrono.reset();
    }
}

void Ennemi024::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    if (inWater) {
        WindowManager::getInstance()->draw(image, 64 + anim * width, 0, 32, 31, dstX, dstY);
        WindowManager::getInstance()->draw(image, 64 + anim * 36, 32, 36, 19, dstX-2, dstY+18);
    } else {
        WindowManager::getInstance()->draw(image, anim * width, 0, 32, 48, dstX, dstY);
    }

}

void Ennemi024::moveX(int dx) {
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + dx);

    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi024::moveY(int dy) {
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY((inWater ? y : y + 16) + dy);

    if (map->checkCollisions(bb, this, false, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi024::getX() {
    return x;
}

int Ennemi024::getY() {
    return y;
}

BoundingBox* Ennemi024::getBoundingBox() {
    box.setX(x);
    box.setY(inWater ? y : y + 16);
    return &box;
}

void Ennemi024::giveItem(int i, int j) {
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    Ennemi::giveItem(i, j);
    AudioManager::getInstance()->playMusic(20);
    map->addItem(ItemHelper::getInstance()->createItem(TI_COEUR, x + 16, y + 16, 8));
    // todo: kill the other boss
    if (x < 140*16) {
        BoundingBox b(200*16,15*16,320,240);
        map->killEnnemis(&b);
    } else {
        BoundingBox b(60*16,15*16,320,240);
        map->killEnnemis(&b);
    }
}

bool Ennemi024::isToAvoid(Collision c) {
    if (inWater) return c != EAU && c != EAU_PROF;
    else return Collisionable::isToAvoid(c);
}
