#include "Oeil.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../MainController.h"

Oeil::Oeil(int i, int j, Direction dir, Map* mp) : direction(dir), actif(false), map(mp) {
    x = i;
    y = j;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/oeil.png", true);

    // for quadtree operations:
    width = (direction == N || direction == S) ? 24 : 8;
    height = (direction == W || direction == E) ? 24 : 8;

    // no collisions
    box.setX(0);
    box.setY(0);
    box.setW(-1);
    box.setH(-1);
}

Oeil::~Oeil() {
    ResourceManager::getInstance()->free(image);
}

void Oeil::loop() {

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();

    BoundingBox sightBox;

    switch (direction) {
        case N :
            sightBox.setX(x - 4);
            sightBox.setW(32);
            sightBox.setY(y + 20);
            sightBox.setH(320);
            break;
        case S :
            sightBox.setX(x - 4);
            sightBox.setW(32);
            sightBox.setY(y - 12 - 320);
            sightBox.setH(320);
            break;
        case W :
            sightBox.setX(x + 20);
            sightBox.setW(320);
            sightBox.setY(y - 4);
            sightBox.setH(32);
            break;
        case E :
            sightBox.setX(x - 12 - 320);
            sightBox.setW(320);
            sightBox.setY(y - 4);
            sightBox.setH(32);
            break;
    }

    if (link->getStatus()->isVisible() && sightBox.intersect(link->getBoundingBox())) {
        BoundingBox bb1(0,0,16,16);
        BoundingBox bb2(0,0,16,16);
        bool b1 = true;
        bool b2 = true;
        for (int i = 0; i < 20; i++) {
            switch (direction) {
                case N :
                    bb1.setX(sightBox.getX());
                    bb1.setY(sightBox.getY() + 16 * i);
                    bb2.setX(bb1.getX() + 16);
                    bb2.setY(bb1.getY());
                    break;
                case S :
                    bb1.setX(sightBox.getX());
                    bb1.setY(sightBox.getY() + sightBox.getH() - 16 * (i + 1));
                    bb2.setX(bb1.getX() + 16);
                    bb2.setY(bb1.getY());
                    break;
                case W :
                    bb1.setX(sightBox.getX() + 16 * i);
                    bb1.setY(sightBox.getY());
                    bb2.setX(bb1.getX());
                    bb2.setY(bb1.getY() + 16);
                    break;
                case E :
                    bb1.setX(sightBox.getX() + sightBox.getW() - 16 * (i + 1));
                    bb1.setY(sightBox.getY());
                    bb2.setX(bb1.getX());
                    bb2.setY(bb1.getY() + 16);
                    break;
            }

            if (b1) {
                if (bb1.intersect(link->getBoundingBox())) {
                    if (!actif) {
                        actif = true;
                        map->activateOeil(true);
                    }
                    return;
                }
                b1 = scene->checkCollisions(&bb1, (Collisionable*)this, false, false, false, false, true);
            }

            if (b2) {
                if (bb2.intersect(link->getBoundingBox())) {
                    if (!actif) {
                        actif = true;
                        map->activateOeil(true);
                    }
                    return;
                }
                b2 = scene->checkCollisions(&bb2, (Collisionable*)this, false, false, false, false, true);
            }

            if (!b1 && !b2) {
                if (actif) {
                    actif = false;
                    map->activateOeil(false);
                }
                return;
            }
        }
    } else {
        if (actif) {
            actif = false;
            map->activateOeil(false);
            return;
        }
    }
}

void Oeil::draw(int offsetX, int offsetY) {

    int i = x - offsetX;
    int j = y - offsetY;

    int srcX = 0;
    int srcY = 0;
    int srcW = 0;
    int srcH = 0;

    switch (direction) {
        case N :
            srcX = 0;
            srcY = actif ? 8 : 0;
            srcW = 24;
            srcH = actif ? 5 : 8;
            if (actif) j += 3;
            break;
        case S :
            srcX = 24;
            srcY = actif ? 8 : 0;
            srcW = 24;
            srcH = actif ? 5 : 8;
            break;
        case W :
            srcX = actif ? 8 : 0;
            srcY = 13;
            srcW = actif ? 5 : 8;
            srcH = 24;
            if (actif) i += 3;
            break;
        case E :
            srcX = actif ? 24 : 29;
            srcY = 13;
            srcW = actif ? 5 : 8;
            srcH = 24;
            break;
    }

    WindowManager::getInstance()->draw(image, srcX, srcY, srcW, srcH, i, j);
}

bool Oeil::isCollision(Collision c) {
    if ((c == MUR_H || c == MUR_B) && (direction == W || direction == E)) {
        return true;
    }
    if ((c == MUR_G || c == MUR_D) && (direction == N || direction == S)) {
        return true;
    }

    switch (c) {
        case TROU : case HERBE : case HERBE_HAUTE : case TERRE : case EAU :
        case EAU_PROF : case GLACE : case MURRET : case SABLE : case NEIGE :
        case PIERRE : case SOL_BOIS : case TAPIS : case DANGER_BAS : return true;
        default : return false;
    }
}
