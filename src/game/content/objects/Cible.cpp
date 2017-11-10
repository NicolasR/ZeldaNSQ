#include "Cible.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

#include "../effects/Debris.h"

Cible::Cible(int t, int d) : type(t), delay(d), delayOnBegin(d) {

    width = 16;
    height = 16;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/cible.png", true);

    init();

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    alive = false;
}

Cible::~Cible() {
    ResourceManager::getInstance()->free(image);
}

void Cible::ennLoop() {
    if (delay > 0) {
        delay--;
        return;
    }

    switch (type) {
        case 0 :
            x++;
            if (x > 17 * 16 + 8) {
                x = 24;
                nbTours--;
            }
            checkPosition();
            break;
        case 1 :
        case 2 :
            x--;
            if (x < 24) {
                x = 17 * 16 + 8;
                nbTours--;
            }
            checkPosition();
            break;
        case 3 :
            x-=2;
            if (x < 24) {
                x = 17 * 16 + 8;
                nbTours--;
            }
            checkPosition();
            break;
    }

    if (!nbTours) {
        alive = false;
    }
}

void Cible::draw(int offsetX, int offsetY) {
    WindowManager::getInstance()->draw(image, type * 16, 0, width, height, x - offsetX, y - offsetY);
}

void Cible::underAttack(Direction dir, int force, TypeAttack t, TypeEffect effect, BoundingBox* box) {
    if (t == TA_ARROW && type && (type != 2 || force == 5)) {
        Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
        AudioManager::getInstance()->playSound(TS_BREAK);
        map->addEffect(new Debris(x + 8, y + 8, direction, 1));
        alive = false;

        int score = 0;
        switch (type) {
            case 1 : score = 1; break;
            case 2 : score = 2; break;
            case 3 : score = 10; break;
        }
        map->updateScore(score);
    }
}

void Cible::start() {
    init();
    alive = true;
    delay = delayOnBegin;
}

void Cible::init() {
    switch (type) {
        case 0 :
            // caisse
            x = 24;
            y = 7 * 16;
            nbTours = 3;
            break;
        case 1 :
        case 2 :
            // cibles standarts
            x = 17 * 16 + 8;
            y = 6 * 16;
            nbTours = 1;
            break;
        case 3 :
            // cible rapide
            x = 17 * 16 + 8;
            y = 5 * 16;
            nbTours = 1;
            break;
    }
}

BoundingBox* Cible::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void Cible::reset() {
    start();
}

int Cible::getX() {
    return x;
}

int Cible::getY() {
    return y;
}
