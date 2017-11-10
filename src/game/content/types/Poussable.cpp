#include "Poussable.h"

#include "../../MainController.h"
#include "../../game/scene/Scene.h"

Poussable::Poussable() : moving(false), dx(0), dy(0), poids(1) {
}

Poussable::~Poussable() {
}

void Poussable::loop() {
}

int Poussable::getPoids() {
    return poids;
}
bool Poussable::isOnWater(BoundingBox* b) {
    int x = b->getX();
    int y = b->getY();
    int w = b->getW();
    int h = b->getH();

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    for (int j = (y / 8) * 8; j < y + h; j += 8) {
        for (int i = (x / 8) * 8; i < x + w; i += 8) {
            if (map->getMur(i, j) != EAU_PROF) return false;
        }
    }
    return true;
}

bool Poussable::isOnGap(BoundingBox* b) {
    int x = b->getX();
    int y = b->getY();
    int w = b->getW();
    int h = b->getH();

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    for (int j = (y / 8) * 8; j < y + h; j += 8) {
        for (int i = (x / 8) * 8; i < x + w; i += 8) {
            if (map->getMur(i, j) != TROU) return false;
        }
    }
    return true;
}
