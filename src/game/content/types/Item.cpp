#include "Item.h"

#include "../../MainController.h"

Item::Item() : anim(0), animMax(20), vanim(30), ready(false), link(0) {
}

Item::~Item() {
}

void Item::loop() {
}

Link* Item::getLink() {
    if (link == 0) {
        link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
    }
    return link;
}

int Item::getX() {return x;}
int Item::getY() {return y;}

void Item::setX(int i) {
    if (i != x) {
        box.setX(box.getX() + (i - x));
        x = i;
        checkPosition();
    }
}

void Item::setY(int j) {
    if (j != y) {
        box.setY(box.getY() + (j - y));
        y = j;
        checkPosition();
    }
}

BoundingBox* Item::getBoundingBox() {return &box;}
bool Item::isReady() {return ready;}
bool Item::isPersistent() {return persistent;}
bool Item::isPickable() {return true;}

void Item::reset() {
}

bool Item::isResetable() {
    return isPersistent();
}
