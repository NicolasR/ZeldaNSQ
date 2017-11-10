#include "ProjPierre.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/Debris.h"

ProjPierre::ProjPierre(int i, int j, double dx, double dy) : dx(dx), dy(dy) {
    x = i - 6;
    y = j - 6;
    longX = i;
    longY = j;

    anim = 0;
    vanim = 120;
    animMax = 3;
    chrono.reset();

    if (dx > 0) {
        if (dy > 0) {
            if (dx > dy) direction = E;
            else direction = S;
        } else {
            if (dx > -dy) direction = E;
            else direction = N;
        }
    } else {
        if (dy > 0) {
            if (-dx > dy) direction = W;
            else direction = S;
        } else {
            if (-dx > -dy) direction = W;
            else direction = N;
        }
    }

    // for quadtree operations:
    width = 13;
    height = 13;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/pierre.png", true);

    force = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 4 : 2;
}

ProjPierre::~ProjPierre() {
    ResourceManager::getInstance()->free(image);
}

void ProjPierre::projLoop() {
    if (!alive) {
        return;
    }

    // compute bounding box for collisions
    box.setX(longX - 6 + dx); box.setY(longY - 6 + dy);

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    if (scene->checkCollisions(&box, (Collisionable*)this, false)) {
        longX += dx;
        longY += dy;

        if (scene->testDegatOnLink(getBoundingBox(), direction, force, TA_PHYSIC, TE_NORMAL)) {
            alive = false;
            AudioManager::getInstance()->playSound(TS_BREAK);
            scene->getMap()->addEffect(new Debris(longX, longY, direction, force, true));
        }
    } else {
        alive = false;
        AudioManager::getInstance()->playSound(TS_BREAK);
        scene->getMap()->addEffect(new Debris(longX, longY, direction, force, true));
    }

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void ProjPierre::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = longX - 6 - offsetX;
    int dstY = longY - 6 - offsetY;

    WindowManager::getInstance()->draw(image, anim * 13, 0, 13, 13, dstX, dstY);
}

BoundingBox* ProjPierre::getBoundingBox() {
    return &box;
}

int ProjPierre::getX() {return longX;}
int ProjPierre::getY() {return longY;}
int ProjPierre::getDown() {return longY + 240;}
