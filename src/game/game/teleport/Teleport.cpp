#include "Teleport.h"

#include "../../MainController.h"

Teleport::Teleport() : dstMap(0), dstX(0), dstY(0), direction(N), instant(false), reload(true), etage(false) {
}

Teleport::~Teleport() {
}

void Teleport::loop() {
    if (!instant) {
        transition.loop();
        if (transition.isWaiting()) {
            Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
            BoundingBox* box = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getCamera()->getBoundingBox();
            mid();
            transition.restart(link->getX() + 8 - box->getX(), link->getY() + 16 - box->getY());
        } else if (transition.isAchieved()) {
            end();
        }
    } else {
        mid();
        end();
    }
}


void Teleport::mid() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    Navi* navi = scene->getNavi();

    link->setX(dstX);
    link->setY(dstY);
    link->setDirection(direction);
    navi->setX(link->getX() + 16);
    navi->setY(link->getY());
    navi->setDirection(direction);

    if (!reload) {
        scene->getMap()->resetRoom(); // reset previous room
        scene->getMap()->launchRoom();
        link->startNewRoom();
        if (etage) {
            scene->getHud()->displayEtage();
        }
    } else {
        Map* mp = new Map(dstMap, scene->getMetronome());
        mp->load();
        scene->setMap(mp);
        mp->launch();

        link->startNewMap();
    }
    link->setAnimation(IDLE);
    scene->fixCameraBounds();

    // open door if needed
    if (scene->getMap()->getId() < 15) {
        int x = link->getX();
        int y = link->getY();
        if (scene->getMap()->getSol(x, y + 8 - 1) == 458) {
            scene->getMap()->ouvrePorteMaison(x, y);
        }
    }
}

void Teleport::end() {
    transition.reset();
    MainController::getInstance()->getGameController()->setStep(GAME_MAIN);
    MainController::getInstance()->getGameController()->getSceneController()->getScene()->getAnimationInGame()->testAnim();

    if (dstMap > 57 && !etage) {
        MainController::getInstance()->getGameController()->displayText(612 + dstMap - 58);
    }
}

void Teleport::draw() {
    if (!instant) {
        transition.draw();
    }
}

void Teleport::init() {
    if (!instant) {
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        Link* link = scene->getLink();
        BoundingBox* box = scene->getCamera()->getBoundingBox();
        transition.start(link->getX() + 8 - box->getX(), link->getY() + 16 - box->getY());
    }
}

void Teleport::setTeleport(int m, int x, int y, Direction dir, bool inst, bool reld, bool etg) {
    dstMap = m;
    dstX = x;
    dstY = y;
    direction = dir;
    instant = inst;
    reload = reld;
    etage = etg;

    if (dstMap != MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->getId()) {
        reload = true;
    }
}

