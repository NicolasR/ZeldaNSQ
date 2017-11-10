#include "TransitionRooms.h"

#include "../../MainController.h"

TransitionRooms::TransitionRooms() : camera(0), dstX(0), dstY(0), moveLink(0) {
}

TransitionRooms::~TransitionRooms() {
}

void TransitionRooms::loop() {

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();

    BoundingBox* box = camera->getBoundingBox();

    switch (direction) {
        case N :
            if (box->getY() > dstY) {
                box->setY(box->getY() - 8);
                if (box->getY() < dstY) box->setY(dstY);
                if (moveLink > 0) {
                    link->setY(link->getY() - 4);
                    moveLink -= 4;
                }
            }
            break;
        case S :
            if (box->getY() < dstY) {
                box->setY(box->getY() + 8);
                if (box->getY() > dstY) box->setY(dstY);
                if (moveLink > 0) {
                    link->setY(link->getY() + 4);
                    moveLink -= 4;
                }
            }
            break;
        case W :
            if (box->getX() > dstX) {
                box->setX(box->getX() - 8);
                if (box->getX() < dstX) box->setX(dstX);
                if (moveLink > 0) {
                    link->setX(link->getX() - 4);
                    moveLink -= 4;
                }
            }
            break;
        case E :
            if (box->getX() < dstX) {
                box->setX(box->getX() + 8);
                if (box->getX() > dstX) box->setX(dstX);
                if (moveLink > 0) {
                    link->setX(link->getX() + 4);
                    moveLink -= 4;
                }
            }
            break;
    }

    if ((box->getX() == dstX && (direction == W || direction == E))
        || (box->getY() == dstY && (direction == N || direction == S))) {

        scene->getMap()->resetRoom(); // reset previous room
        scene->getMap()->launchRoom();
        scene->getAnimationInGame()->testAnim();
        scene->fixCameraBounds();
        link->startNewRoom();
        MainController::getInstance()->getGameController()->setStep(GAME_MAIN);
    }
}

void TransitionRooms::init() {

    moveLink = 48;

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Map* map = scene->getMap();

    camera = scene->getCamera();

    BoundingBox* bounds = scene->getMap()->getBounds();

    // direction
    Link* link = scene->getLink();
    int x = link->getX();
    int y = link->getY();

    if (y <= bounds->getY() && bounds->getY() > 0) {
        dstX = (x/320)*320;
        dstY = bounds->getY() - 240;
        direction = N;
    } else if (y >= bounds->getY() + bounds->getH() - 32 && bounds->getY() + bounds->getH() < map->getH()) {
        dstX = (x/320)*320;
        dstY = bounds->getY() + bounds->getH();
        direction = S;
    } else if (x <= bounds->getX() + 8 && bounds->getX() > 0) {
        dstX = bounds->getX() - 320;
        dstY = (y/240)*240;
        direction = W;
    } else if (x >= bounds->getX() + bounds->getW() - 24 && bounds->getX() + bounds->getW() < map->getW()) {
        dstX = bounds->getX() + bounds->getW();
        dstY = (y/240)*240;
        direction = E;
    }
}
