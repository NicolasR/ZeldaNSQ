#include "ProjGrappin.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../MainController.h"

#include "../types/Item.h"

ProjGrappin::ProjGrappin(int i, int j, Direction dir) : level(0), step(0), aggripe(false),
        currentLength(0), maxLength(160), speed(8), dx(0), dy(0), item(0), dstLink(0) {

    x = i;
    y = j;
    direction = dir;

    // for quadtree operations:
    if (direction == N || direction == S) {
        width = 12;
        height = 8;
        if (direction == N) y -= 16;
        else y += 14;
    } else {
        width = 8;
        height = 12;
        if (direction == W) x -= 16;
        else x += 20;
    }

    switch(direction) {
        case N : dy = -speed; break;
        case S : dy = speed; break;
        case W : dx = -speed; break;
        case E : dx = speed; break;
    }

    box.setW(width);
    box.setH(height);

    image = ResourceManager::getInstance()->loadImage("data/images/projectiles/grappin.png", true);
}

ProjGrappin::~ProjGrappin() {
    ResourceManager::getInstance()->free(image);
}

void ProjGrappin::projLoop() {

    if (item != 0 && !item->isAlive()) {
        item = 0;
    }

    int i;
    int j;
    BoundingBox bb;
    Collision c;
    Scene* scene;
    Link* link;
    bool over = false;

    switch (step) {
        case 0 :
            // grappin start, check missing collision values

            scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

            switch (direction) {
                case N : i = x + 6; j = y + 8; break;
                case S : i = x + 6; j = y + 7 - 8; break;
                case W : i = x + 8; j = y + 6; break;
                case E : i = x + 7 - 8; j = y + 6; break;
            }

            // check level +
            c = (Collision)scene->getMap()->getMur(i, j);
            switch (direction) {
                case N : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level ++; break;
                case S : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level ++; break;
                case W : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level ++; break;
                case E : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level ++; break;
            }

            step = 1;
            //break;
        case 1 :
            // grappin running ahead

            // compute bounding box for collisions

            switch (direction) {
                case N : i = x + 6; j = y; break;
                case S : i = x + 6; j = y + 7; break;
                case W : i = x; j = y + 6; break;
                case E : i = x + 7; j = y + 6; break;
            }

            scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

            // check level +
            c = (Collision)scene->getMap()->getMur(i, j);
            switch (direction) {
                case N : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level ++; break;
                case S : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level ++; break;
                case W : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level ++; break;
                case E : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level ++; break;
            }

            if (!level) {

                bb.setX(i);
                bb.setY(j);
                bb.setW(1);
                bb.setH(1);

                item = scene->getMap()->getItem(&bb);
                if (item != 0) {
                    step = 2;
                    return;
                }

                if (c == BOIS) {
                    step = 3;

                    // compute the destination of Link

                    switch (direction) {
                        case N : dstLink = y - (y % 8) - 8; break;
                        case S : dstLink = y - (y % 8) - 24 + 8 + 8; break;
                        case W : dstLink = x - (x % 8) + 8 - 8; break;
                        case E : dstLink = x - (x % 8) - 16 + 8; break;
                    }

                    link = scene->getLink();
                    bb.setW(16);
                    bb.setH(16);
                    if (direction == N || direction == S) {
                        bb.setX(link->getX());
                        bb.setY(dstLink);
                    } else {
                        bb.setX(dstLink);
                        bb.setY(link->getY() + 8);
                    }
                    while (!scene->checkCollisions(&bb, (Collisionable*)link, false, false, false, false)) {

                        switch (direction) {
                            case N :
                                dstLink += 8;
                                bb.setY(dstLink);
                                if (dstLink >= link->getY() + 8) {
                                    step = 2;
                                    return;
                                }
                                break;
                            case S :
                                dstLink -= 8;
                                bb.setY(dstLink);
                                if (dstLink <= link->getY() + 8) {
                                    step = 2;
                                    return;
                                }
                                break;
                            case W :
                                dstLink += 8;
                                bb.setX(dstLink);
                                if (dstLink >= link->getX()) {
                                    step = 2;
                                    return;
                                }
                                break;
                            case E :
                                dstLink -= 8;
                                bb.setX(dstLink);
                                if (dstLink <= link->getX()) {
                                    step = 2;
                                    return;
                                }
                                break;
                        }
                    }

                    scene->setStep(SCENE_ONLY_LINK);

                    return;
                }

            }

            bb.setX(i);
            bb.setY(j);
            bb.setW(1);
            bb.setH(1);

            if (!level && scene->testDegat(&bb, direction, 1, TA_GRAPPIN, TE_NORMAL)) {
                step = 2;
                return;
            }
            if (scene->checkCollisions(&bb, (Collisionable*)this, false, false, false, level)) {
                x += dx;
                y += dy;
                currentLength += speed;

                if (currentLength >= maxLength /*|| (!level && scene->testDegat(&bb, direction, 1, TA_GRAPPIN, TE_NORMAL))*/) {
                    step = 2;
                    return;
                }

            } else {
                step = 2;
                return;
            }

            // check level -
            switch (direction) {
                case N : if (c == MUR_B || c == MUR_BG || c == MUR_BD) level --; break;
                case S : if (c == MUR_H || c == MUR_HG || c == MUR_HD) level --; break;
                case W : if (c == MUR_D || c == MUR_HD || c == MUR_BD) level --; break;
                case E : if (c == MUR_G || c == MUR_HG || c == MUR_BG) level --; break;
            }

            break;
        case 2 :
            // grappin go back

            if (item != 0) {


                item->setX(item->getX() - dx);
                item->setY(item->getY() - dy);
            }

            x -= dx;
            y -= dy;
            currentLength -= speed;
            if (currentLength <= 0) {
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink()->setAnimation(IDLE);
                if (item != 0) {
                    item->action();
                }
            }


            break;
        case 3 :
            // Link go ahead

            link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
            switch (direction) {
                case N :
                    if (link->getY() + 8 > dstLink) {
                        if (link->getY() + 8 - dstLink < speed) {
                            link->setY(dstLink - 8);
                        } else {
                            link->setY(link->getY() - speed);
                        }
                    } else {
                        over = true;
                    }
                    break;
                case S :
                    if (link->getY() + 8 < dstLink) {
                        if (dstLink - link->getY() - 8 < speed) {
                            link->setY(dstLink - 8);
                        } else {
                            link->setY(link->getY() + speed);
                        }
                    } else {
                        over = true;
                    }
                    break;
                case W :
                    if (link->getX() > dstLink) {
                        if (link->getX() - dstLink < speed) {
                            link->setX(dstLink);
                        } else {
                            link->setX(link->getX() - speed);
                        }
                    } else {
                        over = true;
                    }
                    break;
                case E :
                    if (link->getX() < dstLink) {
                        if (dstLink - link->getX() < speed) {
                            link->setX(dstLink);
                        } else {
                            link->setX(link->getX() + speed);
                        }
                    } else {
                        over = true;
                    }
                    break;
            }

            if (currentLength > speed) currentLength -= speed;
            if (over) {
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink()->setAnimation(IDLE);
                MainController::getInstance()->getGameController()->getSceneController()->setStep(SCENE_MAIN);
            }

            break;
        default :
            break;
    }
}

void ProjGrappin::draw(int offsetX, int offsetY) {

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    int srcW = box.getW();
    int srcH = box.getH();

    WindowManager::getInstance()->draw(image, 12 * direction, 0, srcW, srcH, dstX, dstY);

    // chain
    srcW = 8;
    srcH = 8;
    int dstX2 = dstX;
    int dstY2 = dstY;

    if (direction == N || direction == S) {
        dstX2 += 2;
    } else {
        dstY2 += 2;
    }

    for (int i = 0; i < currentLength; i += 8) {
        switch (direction) {
            case N : dstY2 += 8; break;
            case S : dstY2 -= 8; break;
            case W : dstX2 += 8; break;
            case E : dstX2 -= 8; break;
        }

        WindowManager::getInstance()->draw(image, 44, 8 * ((i / 8) % 2), srcW, srcH, dstX2, dstY2);
    }

}

BoundingBox* ProjGrappin::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

/*bool ProjGrappin::isCollision(Collision c) {

    if ((c == MUR_H || c == MUR_B) && (direction == W || direction == E)) {
        return true;
    }
    if ((c == MUR_G || c == MUR_D) && (direction == N || direction == S)) {
        return true;
    }

    switch (c) {
        case TROU : case HERBE : case HERBE_HAUTE : case TERRE : case EAU :
        case EAU_PROF : case GLACE : case MURRET : return true;
        default : return false;
    }
}*/

void ProjGrappin::slide(int dx, int dy) {
    x += dx;
    y += dy;
//    if (dx != 0 || dy != 0) checkPosition();
}
