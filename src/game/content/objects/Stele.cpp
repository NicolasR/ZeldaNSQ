/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Stele.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

#include "../effects/Debris.h"

Stele::Stele(int i, int j, int t, bool b) : texte(t), broken(b) {
    x = i;
    y = j;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/stele.png", true);

    width = 32;
    height = 32;

    box.setX(broken ? -1 : (x + 4));
    box.setY(broken ? -1 : (y + 16));
    box.setW(broken ? -1 : (width - 8));
    box.setH(broken ? -1 : (height - 16));
}

Stele::~Stele() {
    ResourceManager::getInstance()->free(image);
}

void Stele::draw(int offsetX, int offsetY) {
    if (broken) {
        WindowManager::getInstance()->draw(image, 0, 32, 32, 16, x - offsetX, y - offsetY + 16);
    } else {
        WindowManager::getInstance()->draw(image, 0, 0, width, height, x - offsetX, y - offsetY);
    }
}

bool Stele::action(Direction dir) {
    if (dir == N && !broken) {
        MainController::getInstance()->getGameController()->displayText(getDynamicText());
        return true;
    }
    return false;
}

int Stele::getDynamicText() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    switch (texte) {
        case 198 :
            if (link->getInventory()->hasObject(OCARINA)) {
                return 209;
            }
            break;
        default : break;
    }
    return texte;
}

void Stele::explose() {
    if (broken) return;
    broken = true;
    box.setX(-1);
    box.setY(-1);
    box.setW(-1);
    box.setH(-1);
    AudioManager::getInstance()->playSound(TS_BREAK);
    MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->addEffect(new Debris(x + 16, y + 16, S, 1));
}

int Stele::getDown() {
    return broken ? -1 : y + height;
}
