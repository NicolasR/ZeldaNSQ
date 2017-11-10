#include "Ennemi111.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../algo/AStar.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/FumeeBlanche.h"

#include "../helper/ProjectileHelper.h"

#define PI 3.14159265

Ennemi111::Ennemi111(int i, int j) : anim(0), animMax(1), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi111.png", true);
    chrono.reset();

    step = 0;
    cooldown = 0;

    x = i;
    y = j;

    // for quadtree operations:
    width = 63;
    height = 48;

    box.setX(x+16);
    box.setY(y+16);
    box.setW(32);
    box.setH(32);

    startX = x;
    startY = y;
    startDir = direction;

    life = expert ? 160 : 120;
    maxLife = expert ? 160 : 120;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 40 : 20;
}

Ennemi111::~Ennemi111() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi111::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    animMax = 1;
    step = 0;
    cooldown = 0;
    checkPosition();
}

bool Ennemi111::isResetable() {
    return alive;
}

void Ennemi111::ennLoop() {

    if (step == 0) {
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
    } else if (step == 1) {
        testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);
    }


    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        if (step == 0) {
            if ((maxLife - life) >= 20) cooldown++;
            if (cooldown == 16) {
                step = 1;
                anim = 0;
                cooldown = 0;
            }
        } else {
            if (anim == 1) {
                cooldown++;
                multi();
            } else if (cooldown > (maxLife - life) / 30) {
                step = 0;
                cooldown = 0;
            }
        }
        chrono.reset();
    }
}

void Ennemi111::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    switch (step) {
        case 0 :
            WindowManager::getInstance()->draw(image, direction * width, anim * height, width, height, dstX, dstY);
            break;
        case 1 :
            WindowManager::getInstance()->draw(image, direction * width, (((anim + 1) % 2) + 2) * height, width, height, dstX, dstY);
            break;
    }
}

void Ennemi111::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + 16 + dx);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi111::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + 16 + dy);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    if (map->checkCollisions(bb, this, false, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi111::getX() {
    return x;
}

int Ennemi111::getY() {
    return y;
}

BoundingBox* Ennemi111::getBoundingBox() {
    box.setX(x + 16);
    box.setY(y + 16);
    return &box;
}

void Ennemi111::giveItem(int i, int j) {
    Ennemi::giveItem(i, j);
    if (isBoss) {
        AudioManager::getInstance()->playMusic(20);
        Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
        map->addItem(ItemHelper::getInstance()->createItem(TI_COEUR, x + 32, y + 48 - 8, 11));
    }
}

void Ennemi111::multi() {

    int angle = (int)((float)rand() / RAND_MAX * (360));
    double radius = 4;
    int nbPierres = 12;

    double fx = 0;
    double fy = 0;
    Direction fd = N;
    double fa = ((double)360) / ((double)nbPierres);
    double tmpa = 0;
    for (int i = 0; i < nbPierres; i++) {

        tmpa = i * fa + angle;

        fx = radius * cos(tmpa * PI / 180.0 );
        fy = -radius * sin(tmpa * PI / 180.0 );

        if (abs(fx) > abs(fy)) {
            fd = fx > 0 ? E : W;
        } else {
            fd = fy > 0 ? S : N;
        }

        ProjectileHelper::getInstance()->addProjectile(TP_BIG_GLACE, x + 25, y + 25, fx, fy);
    }


    AudioManager::getInstance()->playSound(TS_THROW);
}
