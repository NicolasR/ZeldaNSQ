#include "Explosion.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

Explosion::Explosion(int i, int j) : anim(0), animMax(1), vanim(120) {
    image = ResourceManager::getInstance()->loadImage("data/images/objects/bombe.png", true);
    chrono.reset();

    x = i - 16;
    y = j - 16;

    // for quadtree operations:
    width = 32;
    height = 32;
}

Explosion::~Explosion() {
    ResourceManager::getInstance()->free(image);
}

void Explosion::loop() {
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            alive = false;
        } else {
            Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
            BoundingBox b(x, y, 32, 32);
            scene->testDegat(&b, S, 5, TA_EXPLO, TE_NORMAL);
            scene->testDegatOnLink(&b, S, 5, TA_EXPLO, TE_NORMAL);
            AudioManager::getInstance()->playSound(TS_BOOM);
        }
        chrono.reset();
    }
}

void Explosion::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX + (anim == 0 ? 9 : 0);
    int dstY = y - offsetY + (anim == 0 ? 9 : 0);
    int srcX = (anim + 8) * 16 + 1;
    int srcY = (anim == 1) ? 0 : 8;
    int srcW = (anim == 1) ? 32 : 14;
    int srcH = srcW;

    WindowManager::getInstance()->draw(image, srcX, srcY, srcW, srcH, dstX, dstY);
}

int Explosion::getX() {return x;}
int Explosion::getY() {return y;}
