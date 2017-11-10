#include "Ennemi148.h"

#include "Ennemi007.h"
#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

Ennemi148::Ennemi148(int i, int j) : anim(0), animMax(2), vanim(180) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi148.png", true);
    chrono.reset();

    step = 0;
    cooldown = 0;
    nbJump = 0;

    x = i;
    y = j;

    // for quadtree operations:
    width = 107;
    height = 60;

    box.setX(x+40);
    box.setY(y+34);
    box.setW(27);
    box.setH(26);

    startX = x;
    startY = y;
    startDir = direction;

    life = 1;
    maxLife = 1;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    forceEnn = expert ? 24 : 12;
}

Ennemi148::~Ennemi148() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi148::reset() {
    Ennemi::reset();
    chrono.reset();
    x = startX;
    y = startY;
    direction = startDir;
    anim = 0;
    animMax = 2;
    step = 0;
    cooldown = 0;
    nbJump = 0;
    checkPosition();
}

bool Ennemi148::isResetable() {
    return alive;
}

void Ennemi148::ennLoop() {

    if (cooldown) cooldown--;

    if (step == 0) {

        // retrieve target position ( = link ^^)
        Link* link = getLink();

        if (link->getStatus()->isVisible() && link->getStatus()->getLife() > 0) {

            direction = (getLink()->getX() + 8 >= x + 40 + 14) ? E : W;

            if (!cooldown) {
                BoundingBox b1(x + 40, y + 34 - 48, 27, 26 + 48);
                if (link->getBoundingBox()->intersect(&b1)) {
                    jump(N);
                } else {
                    BoundingBox b2(x + 40, y + 34, 27, 26 + 48);
                    if (link->getBoundingBox()->intersect(&b2)) {
                        jump(S);
                    } else {
                        BoundingBox b3(x + 40 - 48, y + 34, 27 + 48, 26);
                        if (link->getBoundingBox()->intersect(&b3)) {
                            jump(W);
                        } else {
                            BoundingBox b4(x + 40, y + 34, 27 + 48, 26);
                            if (link->getBoundingBox()->intersect(&b4)) {
                                jump(E);
                            }
                        }
                    }
                }
            }
        }
    }

    if (step == 1 && anim == 1) {
        switch (dirJump) {
            case N : moveY(-2); break;
            case S : moveY(2); break;
            case W : moveX(-2); break;
            case E : moveX(2); break;
        }
        toJump -=2;
        if (toJump <= 0 && nbJump >= (expert ? 16 : 11)) {
            step = 2;
            anim = 0;
            animMax = 8;
            chrono.reset();
        } else if (toJump <= 0) {
            anim = 2;
        }
    }

    if (step != 3) testDegatOnLink(getBoundingBox(), direction, forceEnn, TA_PHYSIC, TE_NORMAL);

    if ((step != 1 || anim != 1) && chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        if (step == 1 && anim == 0) {
            step = 0;
            cooldown = 48;
        }
        if (step == 1 && anim == 1) {
            AudioManager::getInstance()->playSound(TS_THROW);
        }
        if (step == 2 && anim == 5) {
            AudioManager::getInstance()->playSound(TS_BREAK);
        }
        if (step == 2 && anim == 0) {
            step = 3;
            animMax = 3;
        }
        chrono.reset();
    }
}

void Ennemi148::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    switch (step) {
        case 0 :
            WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
            WindowManager::getInstance()->draw(image, anim * 27 + (direction % 2)*81, 0, 27, 26, dstX+40, dstY+34-3);
            break;
        case 1 :
            if (anim == 0 || anim == 2) {
                WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                WindowManager::getInstance()->draw(image, (direction%2)*93, 26, 41, 25, dstX+40 - (direction == E ? 14 : 0), dstY+34+2-3);
            } else {
                WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                WindowManager::getInstance()->draw(image, 41 + (direction%2)*93, 26, 52, 36, dstX+40 - (direction == E ? 25 : 0), dstY+19-3);
            }
            break;
        case 2 :
            switch (anim) {
                case 0 :
                    WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                    WindowManager::getInstance()->draw(image, (direction%2)*93, 62, 45, 32, dstX+40 - (direction == E ? 18 : 0), dstY+19-3);
                    break;
                case 1 :
                    WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                    WindowManager::getInstance()->draw(image, 45 + (direction%2)*93, 62, 48, 32, dstX+40 - (direction == E ? 21 : 0), dstY+19-3);
                    break;
                case 2 :
                    WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                    WindowManager::getInstance()->draw(image, (direction%2)*93, 94, 40, 41, dstX+40 - (direction == E ? 0 : 13), dstY+19-3);
                    break;
                case 3 :
                    WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                    WindowManager::getInstance()->draw(image, 40 + (direction%2)*93, 94, 31, 24, dstX+40 - (direction == E ? 0 : 4), dstY+19-3);
                    break;
                case 4 :
                    WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                    WindowManager::getInstance()->draw(image, (direction%2)*93, 135, 24, 53, dstX+40+2, dstY-3);
                    break;
                case 5 :
                    WindowManager::getInstance()->draw(image, 24 + (direction%2)*93, 135, 47, 60, dstX+30, dstY);
                    break;
                case 6 :
                    WindowManager::getInstance()->draw(image, 0, 195, 75, 30, dstX+16, dstY + 30);
                    break;
                case 7 :
                    WindowManager::getInstance()->draw(image, 0, 225, 100, 39, dstX+4, dstY + 21);
                    break;
                case 8 :
                    WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                    WindowManager::getInstance()->draw(image, 0, 264, 107, 37, dstX, dstY + 23);
                    WindowManager::getInstance()->draw(image, 0, 301, 29, 14, dstX+39, dstY + 46-3);
                    break;
            }
            break;
        case 3 :
            switch (anim) {
                case 0 :
                    WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                    WindowManager::getInstance()->draw(image, 0, 301, 29, 14, dstX+39, dstY + 46-3);
                    break;
                case 1 :
                    WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                    WindowManager::getInstance()->draw(image, 29, 301, 28, 18, dstX+39, dstY + 42-3);
                    break;
                case 2 :
                    WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                    WindowManager::getInstance()->draw(image, 57, 301, 25, 17, dstX+41, dstY + 43-3);
                    break;
                case 3 :
                    WindowManager::getInstance()->draw(image, 100, 200, 21, 6, dstX+40+3, dstY+54); //shadow
                    WindowManager::getInstance()->draw(image, 82, 301, 26, 22, dstX+40, dstY + 38-3);
                    break;
            }
            break;
    }
}

void Ennemi148::moveX(int dx) {

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + 40 + dx);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Ennemi148::moveY(int dy) {

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + 34 + dy);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    if (map->checkCollisions(bb, this, true, false, true, false)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}

int Ennemi148::getX() {
    return x;
}

int Ennemi148::getY() {
    return y;
}

BoundingBox* Ennemi148::getBoundingBox() {
    box.setX(x + 40);
    box.setY(y + 34);
    return &box;
}

bool Ennemi148::hasEffect(TypeAttack type, TypeEffect effect, Direction dir) {
    return step == 3;
}

void Ennemi148::jump(Direction d) {
    dirJump = d;
    nbJump++;
    toJump = 48;
    step = 1;
    anim = 0;
    animMax = 2;
    chrono.reset();
    /*if (direction == W) moveX(2);
    else moveX(-2);*/
}
