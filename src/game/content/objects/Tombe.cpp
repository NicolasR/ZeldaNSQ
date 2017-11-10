#include "Tombe.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

Tombe::Tombe(int i, int j) : moved(false) {
    x = i;
    y = j;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/tombe.png", true);

    // for quadtree operations:
    width = 32;
    height = 24;

    box.setX(x + 3);
    box.setY(y);
    box.setW(26);
    box.setH(24);
}

Tombe::~Tombe() {
    ResourceManager::getInstance()->free(image);
}

void Tombe::loop() {
    if (moving) {

        if (dy != 0) {
            if (dy < 0) {
                height += dy;
            } else {
                y += dy;
                height -= dy;
            }
            box.setY(y);
            box.setH(height);
        }

        if (dy != 0) {
            // quadtree operations:
            checkPosition();
            computeMaxSize();
        }

        if (height == 24) {
            moving = false;
            dy = 0;
        }
    }
}

void Tombe::draw(int offsetX, int offsetY) {

    int i = x - offsetX;
    int j = y - offsetY;

    if (moving && dy < 0) {
        j += height - 24;
    }

    WindowManager::getInstance()->draw(image, 0, 0, 32, 24, i, j);
}

void Tombe::pousse(Direction d, int v) {
    if (!moving && !moved && d == N) {
        BoundingBox dest(x+3, y-16, 26, 24);
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        if (scene->checkCollisions(&dest, this, true)) {
            AudioManager::getInstance()->playSound(TS_PUSH);
            moving = true;
            moved = true;
            y -= 16;
            height += 16;
            dy = -v;

            box.setY(y);
            box.setH(height);

            // quadtree operations:
            checkPosition();
            computeMaxSize();
        }
    }
}
