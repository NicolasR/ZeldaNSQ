#include "Debris.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Debris::Debris(int x, int y, Direction direction, int force, bool ol, bool d, bool t) : x(x), y(y), anim(0), animMax(2), vanim(120),
        direction(direction), force(force), onLink(ol), degat(d), top(t) {
    image = ResourceManager::getInstance()->loadImage("data/images/link/objets.png", true);
    chrono.reset();

    // for quadtree operations:
    width = 32;
    height = 32;
}

Debris::~Debris() {
    ResourceManager::getInstance()->free(image);
}

void Debris::loop() {
    if (!alive) {
        return;
    }
    BoundingBox b(x - anim * 4 - 8, y - anim * 4 - 8, 16 + anim * 8, 16 + anim * 8);
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    if (degat) {
        if (onLink) scene->testDegatOnLink(&b, direction, force, TA_PHYSIC, TE_NORMAL);
        else scene->testDegat(&b, direction, force, TA_PHYSIC, TE_NORMAL);
    }
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            alive = false;
        }
        chrono.reset();
    }
}

void Debris::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    int srcX = 0;
    int srcY = 0;

    switch (anim) {
        case 0 : srcX = 64; srcY = 74; break;
        case 1 : srcX = 56; srcY = 90; break;
        case 2 : srcX = 80; srcY = 74; break;
    }

    WindowManager::getInstance()->draw(image, srcX, srcY, 16 + anim * 8, 16 + anim * 8, dstX - anim * 4 - 8, dstY - anim * 4 - 8);
}

int Debris::getX() {
    return x - 16;
}

int Debris::getY() {
    return y - 16;
}

int Debris::getDown() {
    return top ? y + 240 : y + height;
}
