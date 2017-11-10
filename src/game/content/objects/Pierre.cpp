#include "Pierre.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../effects/Debris.h"

#include "../../MainController.h"

Pierre::Pierre(int i, int j, int id, Map* mp, bool rst) : type(id), map(mp), resetable(rst) {
    x = i;
    y = j;
    x0 = x;
    y0 = y;

    poids = (((type-1) % 2) + 1) * 2; // should be 2 or 4

    image = ResourceManager::getInstance()->loadImage("data/images/link/objets.png", true);

    // for quadtree operations:
    width = 16;
    height = 16;

    // rock has no collisions
    box.setX(0);
    box.setY(0);
    box.setW(-1);
    box.setH(-1);

    // init map with motif and collisions
    short s = map->getSol(x, y);
    if (type < 3) {
        map->setSol(x, y, s - type, MUR);
    } else {
        switch (s) {
            case 1643 : map->setSol(x, y, 1811, MUR); break;
            case 1645 : map->setSol(x, y, 1812, MUR); break;
        }
    }

}

Pierre::~Pierre() {
    ResourceManager::getInstance()->free(image);
}

void Pierre::portLoop() {
    // nothing to do
}

void Pierre::draw(int offsetX, int offsetY) {
    if (alive && (carried || moving)) {
        // shadow
        if (moving) {
            WindowManager::getInstance()->draw(image, 100, 106, 12, 6, x - offsetX + 2, y - offsetY + height - 6);
        }

        int srcY = 0;
        int srcX = 112;
        switch (type) {
            case 1 : srcY = 32; break;
            case 2 : srcY = 80; break;
            case 3 : srcY = 112; break;
            case 4 : srcX = 96; srcY = 112; break;
            default : srcY = 32; break;
        }

        WindowManager::getInstance()->draw(image, srcX, srcY, 16, 16, x - offsetX, y - offsetY);
    }
}

void Pierre::impact() {
    AudioManager::getInstance()->playSound(TS_BREAK);
    map->addEffect(new Debris(x + 8, y + 8, direction, poids));
    alive = false;
}

void Pierre::onLift() {
    short s = map->getSol(x0, y0);
    if (type < 3) {

        Collision c = HERBE;
        switch (s) {
            case 305 : case 306 : c = SABLE; break;
            default : c = HERBE; break;
        }

        map->setSol(x0, y0, s + type, c);
    } else {
        switch (s) {
            case 1811 : map->setSol(x, y, 1643, PIERRE); break;
            case 1812 : map->setSol(x, y, 1645, PIERRE); break;
        }
    }

    map->testGemmeForce(x / 16, y / 16);
}

bool Pierre::isResetable() {
    return resetable;
}

void Pierre::reset() {
    x = x0;
    y = y0;
    if (map->getMur(x, y) != MUR) {
        short s = map->getSol(x, y);
        if (type < 3) {
            map->setSol(x, y, s - type, MUR);
        } else {
            switch (s) {
                case 1643 : map->setSol(x, y, 1811, MUR); break;
                case 1645 : map->setSol(x, y, 1812, MUR); break;
            }
        }
    }
    alive = true;
    carried = false;
    moving = false;
    carriable = true;
    air = 0;
    checkPosition();
}
