#include "Navi.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

#include "../effects/NaviMagic.h"

#include "Link.h"

Navi::Navi(Save* save, int x, int y, Direction direction) : x(x), y(y), magic(0), maxMagic(20),
    lvlHeal(0), lvlAtt(0), lvlDef(0), bonusForce(0), tmpForce(0), maxTmpForce(0), bonusDef(0), tmpDef(0), maxTmpDef(0), regen(0),
    anim(0), animMax(2), vanim(120), boxOnLink(true), sayLook(true), followLink(true), direction(direction) {
    image = ResourceManager::getInstance()->loadImage("data/images/link/navi.png", true);
    chrono.reset();

    magic = save->getNaviMagic();
    maxMagic = save->getNaviMaxMagic();
    lvlHeal = save->getLvlHeal();
    lvlAtt = save->getLvlAtt();
    lvlDef = save->getLvlDef();
}

Navi::~Navi() {
    ResourceManager::getInstance()->free(image);
}

void Navi::saveData(Save* save) {
    save->setNaviMagic(magic);
    save->setNaviMaxMagic(maxMagic);
    save->setLvlHeal(lvlHeal);
    save->setLvlAtt(lvlAtt);
    save->setLvlDef(lvlDef);
}

void Navi::loop() {

    if (followLink) {

        Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();

        int oldX = x;
        int oldY = y;

        int lx = link->getX() + 8;
        int ly = link->getY() + 24;

        int dx = lx - (x + 8);
        int dy = ly - (y + 24);

        int dxa = (dx > 0 ? dx : -dx);
        int dya = (dy > 0 ? dy : -dy);

        int dist = dxa > dya ? dxa : dya;

        if (boxOnLink || dist > 64) {

            // max dist
            int v = 0;
            if (dist > 128) v+=4;
            if (dist > 64) v++;
            if (dist > 48) v++;
            if (dist > 32) v++;
            if (dist > 16) v++;

            if (dx > 0) {
                if (dx < v) {
                    x += dx;
                } else {
                    x += v;
                }
            }
            if (dx < 0) {
                if (dx > v) {
                    x -= dx;
                } else {
                    x -= v;
                }
            }
            if (dy > 0) {
                if (dy < v) {
                    y += dy;
                } else {
                    y += v;
                }
            }
            if (dy < 0) {
                if (dy > v) {
                    y -= dy;
                } else {
                    y -= v;
                }
            }

        }

        //compute direction
        if (oldX != x || oldY != y) {
            if (oldX == x) {
                direction = oldY - y > 0 ? N : S;
            } else if (oldY == y) {
                direction = oldX - x > 0 ? W : E;
            } else {
                if (oldY - y > 0 && direction == S) direction = N;
                if (oldY - y < 0 && direction == N) direction = S;
                if (oldX - x > 0 && direction == E) direction = W;
                if (oldX - x < 0 && direction == W) direction = E;
            }
        }

    }

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        if (magic < maxMagic) {
            regen++;
            if (regen == 25) {
                magic ++;
                regen = 0;
            }
        }
        if (tmpForce > 0) {
            tmpForce--;
            if (tmpForce == 0) {
                bonusForce = 0;
            }
        }
        if (tmpDef > 0) {
            tmpDef--;
            if (tmpDef == 0) {
                bonusDef = 0;
            }
        }
        chrono.reset();
    }
}

void Navi::draw(int offsetX, int offsetY) {
    int srcX = anim * 10;
    int srcY = direction * 9;
    WindowManager::getInstance()->draw(image, 0, 36, 10, 3, x - offsetX, y - offsetY + 21);
    WindowManager::getInstance()->draw(image, srcX, srcY, 10, 9, x - offsetX, y - offsetY);
}

void Navi::handleAction(Action* action) {

    if (boxOnLink) return;

    Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();

    bool spell = false;

    // heal
    if (action->isAction(HIT) && magic >= 20
        && link->getStatus()->getVirtualLife() < link->getStatus()->getMaxLife()
        && link->getInventory()->hasObject(NAVI_HEAL)) {
        int toRestore = 0;
        switch (lvlHeal) {
            case 1 : toRestore = 8; break;
            case 2 : toRestore = 14; break;
            case 3 : toRestore = 20; break;
            case 4 : toRestore = 30; break;
            case 5 : toRestore = 40; break;
        }
        link->getStatus()->updateLife(toRestore);
        magic -= 20;
        spell = true;
    }

    // attack up
    if (action->isAction(ACTION) && magic >= 10 && link->getInventory()->hasObject(NAVI_ATT)) {
        bonusForce = lvlAtt;
        switch (lvlAtt) {
            case 1 : tmpForce = 170; break; // 20s
            case 2 : tmpForce = 250; break; // 30s
            case 3 : tmpForce = 330; break; // 40s
            case 4 : tmpForce = 420; break; // 50s
            case 5 : tmpForce = 500; break; // 500 = 1min
        }
        maxTmpForce = tmpForce;
        magic -= 10;
        spell = true;
    }

    // defense up
    if (action->isAction(CARRY) && magic >= 15 && link->getInventory()->hasObject(NAVI_DEF)) {
        bonusDef = lvlDef;
        switch (lvlDef) {
            case 1 : tmpDef = 170; break; // 20s
            case 2 : tmpDef = 250; break; // 30s
            case 3 : tmpDef = 330; break; // 40s
            case 4 : tmpDef = 420; break; // 50s
            case 5 : tmpDef = 500; break; // 500 = 1min
        }
        maxTmpDef = tmpDef;
        magic -= 15;
        spell = true;
    }

    // ulti
    if (action->isAction(ULTI) && magic >= 40 && link->getInventory()->hasObject(NAVI_ULT)) {
        MainController::getInstance()->getGameController()->getSceneController()->getScene()->getAnimationInGame()->startAnim(49);
        magic -= 40;
    }

    if (spell) {
        AudioManager::getInstance()->playSound(TS_MAGIC);
        MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->addEffect(
                new NaviMagic(link->getX() + 8, link->getY() + 12));

        int dx = link->getX() - x;
        int dy = link->getY() - y;

        int dxa = (dx > 0 ? dx : -dx);
        int dya = (dy > 0 ? dy : -dy);

        if (dya > dxa) {
            direction = dy > 0 ? S : N;
        } else {
            direction = dx > 0 ? E : W;
        }
    }


    int dx = 0;
    int dy = 0;

    if (action->isAction(UP)) dy-=2;
    if (action->isAction(DOWN)) dy+=2;
    if (action->isAction(LEFT)) dx-=2;
    if (action->isAction(RIGHT)) dx+=2;

    // check max distance
    if (dx != 0 || dy != 0) {
        int lx = link->getX() + 8;
        int ly = link->getY() + 24;

        int ddx = lx - (x + dx + 8);
        int ddy = ly - (y + dy + 24);

        int dxa = (ddx > 0 ? ddx : -ddx);
        int dya = (ddy > 0 ? ddy : -ddy);

        //int dist = dxa > dya ? dxa : dya;

        if (dxa > 64) dx = 0;
        if (dya > 64) dy = 0;

        //if (dist <= 64) {
        int oldX = x;
        int oldY = y;
        x += dx;
        y += dy;
        //compute direction
        if (oldX != x || oldY != y) {
            if (oldX == x) {
                direction = oldY - y > 0 ? N : S;
            } else if (oldY == y) {
                direction = oldX - x > 0 ? W : E;
            } else {
                if (oldY - y > 0 && direction == S) direction = N;
                if (oldY - y < 0 && direction == N) direction = S;
                if (oldX - x > 0 && direction == E) direction = W;
                if (oldX - x < 0 && direction == W) direction = E;
            }
            if (sayLook) {
                AudioManager::getInstance()->playSound(TS_NAVY_LOOK, 1);
                sayLook = false;
            }
        }
    }

    // if Navi handles this action, there is nothing to do by Link
    action->setAction(UP, false);
    action->setAction(DOWN, false);
    action->setAction(LEFT, false);
    action->setAction(RIGHT, false);
    action->setAction(HIT, false);
    action->setAction(LOAD, false);
    action->setAction(ACTION, false);
    action->setAction(CARRY, false);
    action->setAction(TALK, false);
}

int Navi::getDown() {
    return y + 24;
}

int Navi::getX() {
    return x;
}

int Navi::getY() {
    return y;
}

void Navi::setX(int i) {
    x = i;
}

void Navi::setY(int j) {
    y = j;
}

void Navi::reset(int i, int j, Direction dir) {
    x = i;
    y = j;
    direction = dir;
}

bool Navi::isBoxOnLink() {
    return boxOnLink;
}

void Navi::setBoxOnLink(bool b) {
    Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
    if (!b && (
            link->getStatus()->getVirtualLife() <= 0 ||
            link->getAnimation() == TOUCHE ||
            link->getAnimation() == MORT ||
            link->getAnimation() == CHUTE ||
            link->getAnimation() == ELECTRIC)
        ) b = true;
    if (boxOnLink && !b) {
        AudioManager::getInstance()->playSound(TS_NAVY_HEY, 1);
        sayLook = true;
    }
    boxOnLink = b;
}

void Navi::setFollowLink(bool b) {
    followLink = b;
}

void Navi::setDirection(Direction dir) {
    direction = dir;
}

int Navi::getMagic() {
    return magic;
}

int Navi::getMaxMagic() {
    return maxMagic;
}

void Navi::setMagic(int i) {
    magic = i;
    if (magic > maxMagic) {
        magic = maxMagic;
    } else if (magic < 0) {
        magic = 0;
    }
}

void Navi::setMaxMagic(int i) {
    maxMagic = i;
    if (maxMagic > 40) {
        maxMagic = 40;
    } else if (maxMagic < 0) {
        maxMagic = 0;
    }
}

int Navi::getLvlHeal() {
    return lvlHeal;
}

int Navi::getLvlAtt() {
    return lvlAtt;
}

int Navi::getLvlDef() {
    return lvlDef;
}

void Navi::setLvlHeal(int lvl) {
    lvlHeal = lvl;
}

void Navi::setLvlAtt(int lvl) {
    lvlAtt = lvl;
}

void Navi::setLvlDef(int lvl) {
    lvlDef = lvl;
}

int Navi::getBonusForce() {
    return bonusForce;
}

int Navi::getTmpForce() {
    return tmpForce;
}

int Navi::getMaxTmpForce() {
    return maxTmpForce;
}

int Navi::getBonusDef() {
    return bonusDef;
}

int Navi::getTmpDef() {
    return tmpDef;
}

int Navi::getMaxTmpDef() {
    return maxTmpDef;
}
