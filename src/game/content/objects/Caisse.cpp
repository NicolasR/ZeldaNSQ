#include "Caisse.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/Plouf.h"

Caisse::Caisse(int i, int j, int id) {
    x = i;
    y = j;
    x0 = x;
    y0 = y;
    type = id;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/caisse.png", true);

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(16);
    box.setH(16);
}

Caisse::~Caisse() {
    ResourceManager::getInstance()->free(image);
}

void Caisse::loop() {
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
            box.setY(y);
            box.setH(height);
        }

        if (dx != 0 || dy != 0) {
            // quadtree operations:
            checkPosition();
            computeMaxSize();
        }

        if (height == 16 && width == 16) {
            moving = false;
            dx = 0;
            dy = 0;

            Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
            if (isOnWater(&box)) {
                AudioManager::getInstance()->playSound(TS_PLOUF);
                scene->getMap()->addEffect(new Plouf(x, y));
                alive = false;
            } else if (isOnGap(&box)) {
                AudioManager::getInstance()->playSound(TS_TOMBE);
                alive = false;
            }
        }
    }
}

void Caisse::draw(int offsetX, int offsetY) {

    int i = x - offsetX;
    int j = y - offsetY;

    if (moving) {
        if (dx < 0) {
            i += width - 16;
        } else if (dy < 0) {
            j += height - 16;
        }
    }

    WindowManager::getInstance()->draw(image, 16 * (type % 3), 16 * (type / 3), 16, 16, i, j);
}

void Caisse::pousse(Direction d, int v) {
    if (!moving) {
        BoundingBox dest;
        dest.setX(x);
        dest.setY(y);
        dest.setW(16);
        dest.setH(16);
        switch (d) {
            case N : dest.setY(y - 16); break;
            case S : dest.setY(y + 16); break;
            case W : dest.setX(x - 16); break;
            case E : dest.setX(x + 16); break;
        }
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        BoundingBox* bounds = scene->getMap()->getBounds();

        if (dest.getX() < bounds->getX() + 32 || dest.getX() + dest.getW() > bounds->getX() + bounds->getW() - 32 ||
            dest.getY() < bounds->getY() + 32 || dest.getY() + dest.getH() > bounds->getY() + bounds->getH() - 32) {
            return;
        }

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
            box.setY(y);
            box.setW(width);
            box.setH(height);

            // quadtree operations:
            checkPosition();
            computeMaxSize();
        }
    }
}

int Caisse::getDown() {
    return -1; // ^^
}

bool Caisse::isResetable() {
    return true;
}

void Caisse::reset() {
    moving = false;
    dx = 0;
    dy = 0;
    x = x0;
    y = y0;
    width = 16;
    height = 16;
    box.setX(x);
    box.setY(y);
    box.setW(16);
    box.setH(16);
    checkPosition();
}
