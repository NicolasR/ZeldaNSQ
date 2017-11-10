#include "Interrupteur.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../MainController.h"

Interrupteur::Interrupteur(int i, int j, int id, bool down, Map* mp) : type(id), used(down), usedTmp(false), map(mp) {
    x = i;
    y = j;

    // for quadtree operations:
    width = 16;
    height = 16;

    // no collisions
    box.setX(0);
    box.setY(0);
    box.setW(-1);
    box.setH(-1);

    // init map with motif and collisions
    switch (type) {
        case 0 : map->setSol(x, y, used ? 1236 : 1235, PIERRE); break;
        case 1 : map->setSol(x, y, used ? 1601 : 1600, PIERRE); break;
        case 2 : map->setSol(x, y, used ? 2040 : 2039, PIERRE); break;
        default : map->setSol(x, y, used ? 1236 : 1235, PIERRE); break;
    }

}

Interrupteur::~Interrupteur() {
}

void Interrupteur::loop() {
    if (used && type != 1) {
        return;
    }

    BoundingBox bb(x + 4, y + 4, 8, 8);
    Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
    if (used && type == 1) {
        if (map->checkCollisions(&bb, this, false, false, false, false, true)
            && !bb.intersect(link->getBoundingBox())) {
            if (map->desactivateInter(x, y)) {
                used = false;
                map->setSol(x, y, 1600, PIERRE);
            }
        }
    } else if (usedTmp && type == 2) {
        if (map->checkCollisions(&bb, this, false, false, false, false, true)
            && !bb.intersect(link->getBoundingBox())) {
            usedTmp = false;
            map->desactivateInter(x, y);
            map->setSol(x, y, 2039, PIERRE);
        }
    } else {
        if (!map->checkCollisions(&bb, this, false, false, false, false, true)
            || bb.intersect(link->getBoundingBox())) {
            if (type != 2) used = true; else usedTmp = true;
            map->activateInter(x, y);
            switch (type) {
                case 0 : map->setSol(x, y, 1236, PIERRE); break;
                case 1 : map->setSol(x, y, 1601, PIERRE); break;
                case 2 : map->setSol(x, y, 2040, PIERRE); break;
                default : map->setSol(x, y, 1236, PIERRE); break;
            }
        }
    }
}

void Interrupteur::draw(int offsetX, int offsetY) {
}

bool Interrupteur::isResetable() {
    return type == 1 || type == 2;
}

void Interrupteur::reset() {
    if (used && type == 1) {
        used = false;
        map->desactivateInter(x, y, false);
        map->setSol(x, y, 1600, PIERRE);
    } else if (!used && usedTmp && type == 2) {
        usedTmp = false;
        map->desactivateInter(x, y, false);
        map->setSol(x, y, 2039, PIERRE);
    }
}

void Interrupteur::activate() {
    if (type == 2 && usedTmp) {
        used = true;
    }
}

void Interrupteur::setUsed() {
    used = true;
    map->setSol(x, y, 1236, PIERRE);
}
