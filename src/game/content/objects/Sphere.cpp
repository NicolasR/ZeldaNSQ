#include "Sphere.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

Sphere::Sphere(int i, int j) : blue(true), invul(0) {
    x = i;
    y = j;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/sphere.png", true);

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    attackable = true;
}

Sphere::~Sphere() {
    ResourceManager::getInstance()->free(image);
}

void Sphere::loop() {
    if (invul) {
        invul--;
        if (!invul) attackable = true;
    }
}

void Sphere::draw(int offsetX, int offsetY) {
    WindowManager::getInstance()->draw(image, blue ? 0 : 16, 0, 16, 16, x - offsetX, y - offsetY);
}

void Sphere::underAttack(Direction dir, int force, TypeAttack type, TypeEffect effect) {
    AudioManager::getInstance()->playSound(TS_HITENNEMY);
    attackable = false;
    invul = 24;
    bool succes = MainController::getInstance()->getGameController()->getSceneController()->getScene()->permuteBlocs(blue ? 0 : 1);
    if (succes) blue = !blue;
}
