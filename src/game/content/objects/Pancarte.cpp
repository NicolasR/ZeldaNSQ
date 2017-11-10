#include "Pancarte.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../effects/Debris.h"

#include "../../MainController.h"

Pancarte::Pancarte(int i, int j, int t, Map* mp) : texte(t), map(mp) {
    x = i;
    y = j;
    x0 = x;
    y0 = y;

    image = ResourceManager::getInstance()->loadImage("data/images/link/objets.png", true);

    // for quadtree operations:
    width = 16;
    height = 16;

    // pancarte has no collisions
    box.setX(0);
    box.setY(0);
    box.setW(-1);
    box.setH(-1);

    // init map with motif and collisions
    short s = map->getSol(x, y);
    map->setSol(x, y, s - 1, BOIS);
}

Pancarte::~Pancarte() {
    ResourceManager::getInstance()->free(image);
}

void Pancarte::portLoop() {
    // nothing to do
}

void Pancarte::draw(int offsetX, int offsetY) {
    if (alive && (carried || moving)) {
        // shadow
        if (moving) {
            WindowManager::getInstance()->draw(image, 100, 106, 12, 6, x - offsetX + 2, y - offsetY + height - 6);
        }
        WindowManager::getInstance()->draw(image, 112, 48, 16, 16, x - offsetX, y - offsetY);
    }
}

void Pancarte::impact() {
    AudioManager::getInstance()->playSound(TS_BREAK);
    map->addEffect(new Debris(x + 8, y + 8, direction, 1));
    alive = false;
}

bool Pancarte::action(Direction dir) {
    if (alive && dir == N && !carried && !moving) {
        MainController::getInstance()->getGameController()->displayText(texte);
    }
    return false;
}

void Pancarte::onLift() {
    short s = map->getSol(x0, y0);

    Collision c = HERBE;
    switch (s) {
        case 308 : c = SABLE; break;
        case 967 : c = NEIGE; break;
        case 969 : c = TERRE; break;
        default : c = HERBE; break;
    }

    map->setSol(x0, y0, s + 1, c);
}

bool Pancarte::isResetable() {
    return true;
}

void Pancarte::reset() {
    x = x0;
    y = y0;
    if (map->getMur(x, y) != BOIS) {
        map->setSol(x, y, map->getSol(x, y) - 1, BOIS);
    }
    alive = true;
    carried = false;
    moving = false;
    carriable = true;
    air = 0;
    checkPosition();
}
