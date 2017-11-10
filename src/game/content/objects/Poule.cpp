#include "Poule.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

Poule::Poule(int i, int j) : anim(0), animMax(1), vanim(180), stop(0) {
    x = i;
    y = j;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/poule.png", true);

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    box2.setX(0);
    box2.setY(0);
    box2.setW(-1);
    box2.setH(-1);

    attackable = false;
    safeMode = true;

    chrono.reset();
}

Poule::~Poule() {
    ResourceManager::getInstance()->free(image);
}

void Poule::portLoop() {
    if (!alive) return;

    if (stop > 0) stop--;

    if (!moving && !carried && !stop) {
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
    } else if (carried) {
        direction = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink()->getDirection();
    }



    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Poule::draw(int offsetX, int offsetY) {
    if (alive) {
        // shadow
        if (moving || !carried) {
            WindowManager::getInstance()->draw(image, 2, 79, 12, 6, x - offsetX + 2, y - offsetY + height - 6 +1);
        }
        WindowManager::getInstance()->draw(image, 16 * (direction % 2), 17 * anim, 16, 16, x - offsetX, y - offsetY);
    }
}

void Poule::impact() {
    AudioManager::getInstance()->playSound(TS_POULE);
}

void Poule::onLift() {
    AudioManager::getInstance()->playSound(TS_POULE);
}

void Poule::stopBeforeUp() {
    stop = 64;
}

void Poule::moveX(int dx) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + dx);

    if (scene->checkCollisions(bb, this, false) && scene->checkCollisionsWithLink(bb)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Poule::moveY(int dy) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + dy);

    if (scene->checkCollisions(bb, this, false) && scene->checkCollisionsWithLink(bb)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

bool Poule::isCollision(Collision c) {
    switch (c) {
        case TROU : case HERBE : case HERBE_HAUTE : case TERRE : case EAU :
        case GLACE : case SABLE : case NEIGE :
        case PIERRE : case SOL_BOIS : case TAPIS : case DANGER_BAS : return true;
        case MURRET : return moving;
        default : return false;
    }
}

BoundingBox* Poule::getBoundingBox() {
    box.setX(x);
    box.setY(y + height - 16);
    return &box;
}

BoundingBox* Poule::getSecondBoundingBox() {
    return &box2;
}
