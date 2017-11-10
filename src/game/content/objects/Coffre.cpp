/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Coffre.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

Coffre::Coffre(int i, int j, int t, bool op, TypeItem obj, int id) : type(t), opened(op), object(obj), objId(id) {
    x = i;
    y = j;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/coffres.png", true);

    // for quadtree operations:
    if (type == 0) {
        width = 16;
        height = 16;
    } else {
        width = 32;
        height = 24;
    }

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);
}

Coffre::~Coffre() {
    ResourceManager::getInstance()->free(image);
}

void Coffre::draw(int offsetX, int offsetY) {

    int srcX = (type == 0 && opened) ? 16 : 0;
    int srcY = type == 0 ? 0 : opened ? 40 : 16;

    WindowManager::getInstance()->draw(image, srcX, srcY, width, height, x - offsetX, y - offsetY);
}

bool Coffre::action(Direction dir) {
    if (dir == N && !opened) {
        AudioManager::getInstance()->playSound(TS_COFFRE);
        if (object != TI_NO_ITEM) {
            MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink()->trouveObjet(object, objId);
        } else {
            AudioManager::getInstance()->playSound(TS_ERROR);
        }
        // for map 34 and 61 : open all chests
        MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->openChests(x, y);
        opened = true;
        return true;
    }
    return false;
}

bool Coffre::isOpened() {
    return opened;
}

void Coffre::open() {
    opened = true;
}

void Coffre::close(TypeItem obj, int id) {
    objId = id;
    opened = false;
    object = obj;
}

int Coffre::getDown() {return y - 240;}


