#include "Statue.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/Plouf.h"

Statue::Statue(int i, int j) {
    x = i;
    y = j;
    x0 = x;
    y0 = y;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/statue.png", true);

    // for quadtree operations:
    width = 16;
    height = 25;

    box.setX(x);
    box.setY(y+9);
    box.setW(16);
    box.setH(16);
}

Statue::~Statue() {
    ResourceManager::getInstance()->free(image);
}

void Statue::loop() {
    if (moving) {

        if (dx != 0) {
            if (dx < 0) {
                width += dx;
            } else {
                x += dx;
                width -= dx;
            }
            box.setX(x);
            box.setW(width);
        } else if (dy != 0) {
            if (dy < 0) {
                height += dy;
            } else {
                y += dy;
                height -= dy;
            }
            box.setY(y + 9);
            box.setH(height - 9);
        }

        if (dx != 0 || dy != 0) {
            // quadtree operations:
            checkPosition();
            computeMaxSize();
        }

        if (height == 25 && width == 16) {
            moving = false;
            dx = 0;
            dy = 0;

            Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
            if (isOnWater(&box)) {
                AudioManager::getInstance()->playSound(TS_PLOUF);
                scene->getMap()->addEffect(new Plouf(x, y + 9));
                alive = false;
            } else if (isOnGap(&box)) {
                AudioManager::getInstance()->playSound(TS_TOMBE);
                alive = false;
            }
        }
    }
}

void Statue::draw(int offsetX, int offsetY) {

    int i = x - offsetX;
    int j = y - offsetY;

    if (moving) {
        if (dx < 0) {
            i += width - 16;
        } else if (dy < 0) {
            j += height - 25;
        }
    }

    WindowManager::getInstance()->draw(image, 0, 0, 16, 25, i, j);
}

void Statue::pousse(Direction d, int v) {
    if (!moving) {
        BoundingBox dest;
        dest.setX(x);
        dest.setY(y + 9);
        dest.setW(16);
        dest.setH(16);
        switch (d) {
            case N : dest.setY(y + 9 - 16); break;
            case S : dest.setY(y + 9 + 16); break;
            case W : dest.setX(x - 16); break;
            case E : dest.setX(x + 16); break;
        }
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        if (scene->checkCollisions(&dest, this, true)) {
            AudioManager::getInstance()->playSound(TS_PUSH);
            moving = true;
            switch (d) {
                case N :
                    y -= 16;
                    height += 16;
                    dy = -v;
                    break;
                case S :
                    height += 16;
                    dy = v;
                    break;
                case W :
                    x -= 16;
                    width += 16;
                    dx = -v;
                    break;
                case E :
                    width += 16;
                    dx = v;
                    break;
            }
            box.setX(x);
            box.setY(y + 9);
            box.setW(width);
            box.setH(height - 9);

            // quadtree operations:
            checkPosition();
            computeMaxSize();
        }
    }
}

bool Statue::isResetable() {
    return true;
}

void Statue::reset() {
    moving = false;
    dx = 0;
    dy = 0;
    x = x0;
    y = y0;
    width = 16;
    height = 25;
    box.setX(x);
    box.setY(y+9);
    box.setW(16);
    box.setH(16);
    checkPosition();
}
