#include "ObjBomb.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

ObjBomb::ObjBomb(int i, int j) : anim(0), animMax(30), vanim(120) {
    x = i;
    y = j;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/bombe.png", true);
    //chrono.reset();

    // for quadtree operations:
    width = 14;
    height = 16;

    // bomb has no collisions (then we cannot pick them up...)
    box.setX(0);
    box.setY(0);
    box.setW(-1);
    box.setH(-1);

    setPosable(true);

    chrono.reset();
}

ObjBomb::~ObjBomb() {
    ResourceManager::getInstance()->free(image);
}

void ObjBomb::portLoop() {

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim == 30) {
            x -= 9;
            y -= 9;
            height = 32;
            width = 32;
            Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
            BoundingBox* b = getHandableBox();
            scene->testDegat(b, S, 5, TA_EXPLO, TE_NORMAL);
            scene->testDegatOnLink(b, S, 5, TA_EXPLO, TE_NORMAL);
            AudioManager::getInstance()->playSound(TS_BOOM);
        }
        if (anim > animMax) {
            alive = false;
        }
        chrono.reset();
    }

}

void ObjBomb::draw(int offsetX, int offsetY) {

    int dstX = x - offsetX;
    int dstY = y - offsetY;
    int srcX = (anim < 22) ? 1 : (anim - 21) * 16 + 1;
    int srcY = (anim == 30) ? 0 : 8;
    int srcW = (anim == 30) ? 32 : 14;
    int srcH = srcW;

    // draw shadow
    if (!carried && anim < 29) {
        WindowManager::getInstance()->draw(image, 2, 1, 12, 6, dstX + 1, dstY + height - 6);
    }

    WindowManager::getInstance()->draw(image, srcX, srcY, srcW, srcH, dstX, dstY);
}

void ObjBomb::impact() {
    AudioManager::getInstance()->playSound(TS_SOL);
}

void ObjBomb::reset() {
    alive = false;
}

bool ObjBomb::isResetable() {
    return true;
}

