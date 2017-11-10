#include "Portable.h"

#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"
#include "../../game/scene/Scene.h"

#include "../effects/Plouf.h"

Portable::Portable() : carried(false), carriable(true), moving(false), direction(S), dx(0), dy(0), air(0), poids(1),
    safeMode(false), maxDist(0), posable(false), speed(0), level(0) {
}

Portable::~Portable() {
}

void Portable::loop() {
    int oldX = x;
    int oldY = y;

    portLoop();

    if (air > 0 && !carried) {

        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        BoundingBox* b = getHandableBox();

        if (moving) {
            y++;
            air--;
            height--;

            if (safeMode) {
                if (maxDist > 0) {
                    b->setX(x + dx);
                    b->setY(y + dy + air);
                    if (scene->checkCollisions(b, (Collisionable*)this, false, false, false, false)
                    && !scene->testDegat(b, direction, poids * (speed / 2), poids > 1 ? TA_MASSIF : TA_PHYSIC, TE_NORMAL)) {
                        if (dx > 0) {
                            if (dx > maxDist) dx = maxDist;
                            maxDist -= dx;
                        } else if (dx < 0) {
                            if (-dx > maxDist) dx = -maxDist;
                            maxDist += dx;
                        } else if (dy > 0) {
                            if (dy > maxDist) dy = maxDist;
                            maxDist -= dy;
                        } else if (dy < 0) {
                            if (-dy > maxDist) dy = -maxDist;
                            maxDist += dy;
                        }
                        x += dx;
                        y += dy;
                    } else {
                        dx = 0;
                        dy = 0;
                        BoundingBox* secBox = getSecondBoundingBox();
                        secBox->setW(-1);
                        secBox->setH(-1);
                        moving = false;
                    }
                } else {
                    dx = 0;
                    dy = 0;
                    BoundingBox* secBox = getSecondBoundingBox();
                    secBox->setW(-1);
                    secBox->setH(-1);
                    moving = false;
                }

            } else {
                if (scene->checkCollisions(b, (Collisionable*)this, false, false, false, false)
                    && !scene->testDegat(b, direction, poids * (speed / 2), poids > 1 ? TA_MASSIF : TA_PHYSIC, TE_NORMAL)) {
                    x += dx;
                    y += dy;
                } else {
                    dx = 0;
                    dy = 0;
                    moving = false;
                }
            }

        } else {
            air -= 2;
            y += 2;
            height -= 2;
            if (height % 2 != 0) {
                height++;
                air++;
                y--;
            }
        }

        if (air <= 0) {
            carriable = true;
            moving = false;
            b->setX(b->getX() + 2);
            b->setY(b->getY() + 2);
            b->setW(b->getW() - 4);
            b->setH(b->getH() - 4);
            if (isOnWater(b)) {
                AudioManager::getInstance()->playSound(TS_PLOUF);
                scene->getMap()->addEffect(new Plouf(x + 8 - width / 2, y + height - 16));
                alive = false;
            } else if (isOnGap(b)) {
                AudioManager::getInstance()->playSound(TS_TOMBE);
                alive = false;
            } else {
                impact();
            }
        }
    }

    bool idle = (x == oldX && y == oldY);
    if (!idle) checkPosition();
}

bool Portable::isPosable() {
    return posable;
}

void Portable::setPosable(bool p) {
    posable = p;
}

bool Portable::isCarriable() {
    return carriable;
}

void Portable::drawWhenCarried(int xCenter, int yDown, int offsetX, int offsetY) {

    carried = true;

    x = xCenter - width / 2;
    y = yDown - height;

    draw(offsetX, offsetY);
}

void Portable::setDirection(Direction dir) {
    direction = dir;
}

BoundingBox* Portable::getHandableBox() {
    handableBox.setX(x);
    handableBox.setY(y + air);
    handableBox.setW(width);
    handableBox.setH(height - air);
    return &handableBox;
}


void Portable::lance(int down, Direction d, int v) {
    lache(down);
    direction = d;
    speed = v;

    switch (direction) {
        case N : dx = 0; dy = -v; break;
        case S : dx = 0; dy = v; break;
        case W : dx = -v; dy = 0; break;
        case E : dx = v; dy = 0; break;
    }

    if (safeMode) {
        // compute final destination and protect it
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        BoundingBox* b = getHandableBox();
        int oldX = b->getX();
        int oldY = b->getY();
        b->setX(oldX + dx * air);
        b->setY(oldY + dy * air);

        BoundingBox* secBox = getSecondBoundingBox();
        secBox->setX(b->getX());
        secBox->setY(b->getY());
        secBox->setW(b->getW());
        secBox->setH(b->getH());

        maxDist = 0;

        switch (direction) {
            case N :
                while (!scene->checkCollisions(b, (Collisionable*)this, false, false, false, false)) {
                    if (b->getY() % 8 != 0) {
                        b->setY(b->getY() + 8 - b->getY() % 8);
                    } else {
                        b->setY(b->getY() + 8);
                    }
                    if (b->getY() >= oldY) break;
                }
                maxDist = oldY - b->getY();
                secBox->setY(b->getY() - maxDist);
                secBox->setH(b->getH() + maxDist);
                break;
            case S :
                while (!scene->checkCollisions(b, (Collisionable*)this, false, false, false, false)) {
                    if (b->getY() % 8 != 0) {
                        b->setY(b->getY() - b->getY() % 8);
                    } else {
                        b->setY(b->getY() - 8);
                    }
                    if (b->getY() <= oldY) break;
                }
                maxDist = b->getY() - oldY;
                secBox->setH(b->getH() + maxDist);
                break;
            case W :
                while (!scene->checkCollisions(b, (Collisionable*)this, false, false, false, false)) {
                    if (b->getX() % 8 != 0) {
                        b->setX(b->getX() + 8 - b->getX() % 8);
                    } else {
                        b->setX(b->getX() + 8);
                    }
                    if (b->getX() >= oldX) break;
                }
                maxDist = oldX - b->getX();
                secBox->setX(b->getX() - maxDist);
                secBox->setW(b->getW() + maxDist);
                break;
            case E :
                while (!scene->checkCollisions(b, (Collisionable*)this, false, false, false, false)) {
                    if (b->getX() % 8 != 0) {
                        b->setX(b->getX() - b->getX() % 8);
                    } else {
                        b->setX(b->getX() - 8);
                    }
                    if (b->getX() <= oldX) break;
                }
                maxDist = b->getX() - oldX;
                secBox->setW(b->getW() + maxDist);
                break;
        }
    }

    moving = true;
}

void Portable::lache(int down) {
    carriable = false;
    carried = false;
    air = down - y - height;
    height += air;
}

int Portable::getPoids() {
    return poids;
}

void Portable::impact() {
}

void Portable::onLift() {
}

void Portable::onContact() {
}

void Portable::stopBeforeUp() {
}

bool Portable::isOnWater(BoundingBox* b) {
    int x = b->getX();
    int y = b->getY();
    int w = b->getW();
    int h = b->getH();

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    for (int j = (y / 8) * 8; j < y + h; j += 8) {
        for (int i = (x / 8) * 8; i < x + w; i += 8) {
            if (map->getMur(i, j) != EAU_PROF) return false;
        }
    }
    return true;
}

bool Portable::isOnGap(BoundingBox* b) {
    int x = b->getX();
    int y = b->getY();
    int w = b->getW();
    int h = b->getH();

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    for (int j = (y / 8) * 8; j < y + h; j += 8) {
        for (int i = (x / 8) * 8; i < x + w; i += 8) {
            if (map->getMur(i, j) != TROU) return false;
        }
    }
    return true;
}

bool Portable::isCollision(Collision c) {

    switch (direction) {
        case N : if (c == MUR_H || c == MUR_HG || c == MUR_HD) {level++; return true;} break;
        case S : if (c == MUR_B || c == MUR_BG || c == MUR_BD) {level++; return true;} break;
        case W : if (c == MUR_G || c == MUR_HG || c == MUR_BG) {level++; return true;} break;
        case E : if (c == MUR_D || c == MUR_HD || c == MUR_BD) {level++; return true;} break;
    }

    if (level > 0) return true;

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
