#include "Ennemi153.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi153::Ennemi153(int i, int j) : anim(0), animMax(3), vanim(180), anim2(0), animMax2(5), vanim2(320), step(0), cooldown(0), debuffUlti(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi153.png", true);
    chrono.reset();
    chrono2.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 78;
    height = 86;

    box.setX(x+7);
    box.setY(y+12);
    box.setW(64);
    box.setH(64);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 1000 : 500;
    maxLife = expert ? 1000 : 500;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 120 : 60;
}

Ennemi153::~Ennemi153() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi153::reset() {
    Ennemi::reset();
    chrono.reset();
    chrono2.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    anim2 = 0;
    step = 0;
    cooldown = 0;
    debuffUlti = 0;
    checkPosition();
}

void Ennemi153::ennLoop() {

    if (step == 0 && life <= (expert ? 400 : 200)) step = 1;
    if (step == 1 && life <= (expert ? 200 : 100)) {
        step = 2;
        getLink()->getInventory()->setObject(NAVI_ULT, 1);
    }
    if (step == 4) {
        if (debuffUlti) {
            debuffUlti--;
            life--;
            if (life <= 0) {
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->getHud()->setBossLife(0, maxLife);
                alive = false;
                giveItem(x + width / 2, y + height / 2);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->testAnimRoom();
                return;
            }
        }
        if (debuffUlti == 0) {
            step = 2;
        }
    }

    if (step != 3) {
        // move
        int randomValue = (int)((float)rand() / RAND_MAX * (100));
        switch (randomValue) {
            case 1 : moveX(-1);direction=W; break;
            case 2 : moveX(1); direction=E; break;
            case 3 : moveY(-1);direction=N; break;
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
        testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);
    }

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
        if (step < 3) {
            cooldown++;
            if (cooldown >= (expert ? 20 : 30) && getLink()->getStatus()->getVirtualLife() > 0) {
                ProjectileHelper::getInstance()->addProjectile(TP_BOULE_ULTIME, x + 40, y + 29, getLink());
                AudioManager::getInstance()->playSound(TS_THROW);
                cooldown = 0;
            }
        }
    }

    if (chrono2.getElapsedTime() >= vanim2) {
        anim2++;
        if (anim2 > animMax2) {
            anim2 = 0;
        }
        chrono2.reset();
    }
}

void Ennemi153::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + 7 + dx);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi153::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + 12 + dy);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, false, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

void Ennemi153::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, step == 4 ? 468 : anim2 * width, anim * height + 344 * ((step == 0 || step == 4) ? 0 : 1), width, height, dstX, dstY);
}

int Ennemi153::getX() {
    return x;
}

int Ennemi153::getY() {
    return y;
}

BoundingBox* Ennemi153::getBoundingBox() {
    box.setX(x+7);
    box.setY(y+12);
    return &box;
}

void Ennemi153::giveItem(int i, int j) {
    Ennemi::giveItem(i, j);
    AudioManager::getInstance()->stopMusic();
    MainController::getInstance()->getGameController()->getSceneController()->getScene()->setCoffre(9, 28, 1);
    getLink()->setOniLink(false);
}

bool Ennemi153::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return  step == 0 || (step == 1 && getLink()->getStatus()->isOniLink());
}

bool Ennemi153::finalForm() {
    return (step > 0);
}

void Ennemi153::stop() {
    step = 3;
}

void Ennemi153::ulti() {
    step = 4;
    debuffUlti = 100;
}
