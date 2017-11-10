#include "Link.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../../config/ConfigurationManager.h"

#include "../../MainController.h"
#include "../../game/scene/Scene.h"

#include "../effects/Plouf.h"
#include "../effects/Flamme.h"
#include "../effects/FumeeBlanche.h"
#include "../effects/FumeeViolette.h"

#include "../objects/ObjBomb.h"

#include "../helper/ProjectileHelper.h"

Link::Link(Save* save, Metronome* metronome) : metronome(metronome), epee(0), bouclier(0), charging(false), animation(IDLE),
anim(0), animMax(0), vanim(40), imageLink(0), porte(0), elan(2), grappin(0), flamme(0), caisse(0), boomerang(true),
byrna(false), animByrna(0), dirByrna(S), lapin(false), fatal(false) {

    imageEffets = ResourceManager::getInstance()->loadImage("data/images/link/effects.png", true);
    imageObjets = ResourceManager::getInstance()->loadImage("data/images/link/objets.png", true);

    status = new Status(save);
    inventory = new Inventory(save);

    loadFromSave(save);

    w = 16;
    h = 24;
    speed = 0;

    height = h; // for display order

    accuPush = 0;
    charge = 0;
    wasOnIce = false;
    moved = false;

    for (int i = 0; i < 8; i++) {
        oldx[i] = x;
        oldy[i] = y;
    }

    startX = x;
    startY = y;
    startDir = direction;

    startX2 = x;
    startY2 = y;
    startDir2 = direction;

    chrono.reset();
}

Link::~Link() {
    ResourceManager::getInstance()->free(imageLink);
    ResourceManager::getInstance()->free(imageEffets);
    ResourceManager::getInstance()->free(imageObjets);
    delete status;
    delete inventory;
    delete porte;
    delete grappin;
    delete flamme;
    delete epee;
    delete bouclier;
}

void Link::loadFromSave(Save* save) {
    x = save->getX();
    y = save->getY();
    direction = save->getDirection();
    setTunique(save->getTunique());
    setEpee(save->getEpee());
    setBouclier(save->getBouclier());
}

void Link::saveData(Save* save) {
    save->setX(startX);
    save->setY(startY);
    save->setDirection(startDir);
    save->setTunique(tunique);
    save->setEpee(getEpee());
    save->setBouclier(getBouclier());
    status->saveData(save);
    inventory->saveData(save);
}

int Link::getDefense() {
    return tunique - 1 + getBouclier()
            + MainController::getInstance()->getGameController()->getSceneController()->getScene()->getNavi()->getBonusDef()
            + (status->isOniLink() ? 2 : 0);
}

int Link::getForce() {
    return getEpee()
            + MainController::getInstance()->getGameController()->getSceneController()->getScene()->getNavi()->getBonusForce()
            + (status->isOniLink() ? 2 : 0);
}

bool Link::underAttack(Direction dir, int force, TypeAttack type, TypeEffect effect) {
    if (!status->isInvincible() && status->getVirtualLife() > 0 && animation != COULE && animation != TOUCHE
        && animation != CHUTE && animation != ELECTRIC && !byrna) {
        status->setInvincible();

        int def = getDefense();
        int degats = def == 0 ? force * 2 : force / def;

        if (effect == TE_QUASI_MORT) {
            degats = status->getLife() - 1;
        } else if (effect == TE_MORT) {
            degats = status->getMaxLife();
        } else if (effect == TE_GRAVITE) {
            degats = (status->getLife() + 1) / 2;
        } else if (effect == TE_GLACE) {
            status->updateGel(MainController::getInstance()->getGameController()->getSave()->isExpert() ? 256 : 128);
        }

        if (degats < 1) degats = 1;

        status->setLife(status->getLife()-degats);

        if (effect == TE_ELECTRIC) {
            setAnimation(ELECTRIC);
            AudioManager::getInstance()->playSound(TS_SHOCK);
        } else {
            switch (dir) {
                case N : toucheX = 0; toucheY = -1; direction = S; break;
                case S : toucheX = 0; toucheY = 1; direction = N; break;
                case W : toucheX = -1; toucheY = 0; direction = E; break;
                case E : toucheX = 1; toucheY = 0; direction = W; break;
            }
            setAnimation(TOUCHE);
            AudioManager::getInstance()->playSound(TS_HURT);
        }

        return true;
    }
    return false;
}

int Link::compareTo(Listable* other) {
    Drawable* o = dynamic_cast<Drawable*>(other);
    if (o == 0) return 0;
    if (animation == COULE) return 1;
    return o->getDown() - getDown();
}

void Link::startNewMap() {
    startNewRoom(true);
    startX = x;
    startY = y;
    startDir = direction;
    setAnimation(isOnWater() ? FLOTTE : IDLE);
}

void Link::startNewRoom(bool newMap) {
    startX2 = x;
    startY2 = y;
    startDir2 = direction;

    if (animation == PORTE || animation == TRANSPORTE) {
        if (animation == PORTE) {
            setAnimation(IDLE);
        } else {
            setAnimation(WALK);
        }
        if (porte != 0 && !newMap && porte->isResetable()) {
            MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->addObject(porte);
            porte->reset();
        } else {
            delete porte;
        }
        porte = 0;
    }
    unlockBoomerang();
    notifySomariaEnd();
}

void Link::revit() {
    status->setLife(status->getMaxLife());
    status->increaseDeaths();
    status->setInvincible();
    status->setVisible(true);
    status->resetGel();
    status->resetOniLife();
    fatal = false;
    if (status->isOniLink()) {
        setOniLink(false, true);
    }
    byrna = false;
    x = startX;
    y = startY;
    direction = startDir;
    startX2 = x;
    startY2 = y;
    startDir2 = direction;
    setAnimation(isOnWater() ? FLOTTE : IDLE);
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    scene->getNavi()->reset(x + 16, y, S);
    scene->getMap()->reset();
}

void Link::setLapin(bool l) {
    lapin = l;
}

void Link::setOniLink(bool b, bool revit, bool withAnim) {
    status->setOniLink(b);

    if (b) {
        ResourceManager::getInstance()->free(imageLink);
        imageLink = ResourceManager::getInstance()->loadImage("data/images/link/onilink.png", true);
    } else {
        setTunique(tunique);
    }
    if (epee != 0) setEpee(epee->getForce());
    if (bouclier != 0) setBouclier(bouclier->getLevel());

    if (!revit) {
        Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
        map->addEffect(new FumeeViolette(x+8, y+16));
        AudioManager::getInstance()->playSound(TS_CAPE_OFF);
        if (b && withAnim) map->startAnimOniLink();
    }
}

void Link::setFatalAnim() {
    fatal = true;
}

bool Link::isFatalAnim() {
    return fatal;
}

void Link::handleAction(Action* action) {

    moved = false;

    if (status->getVirtualLife() <= 0) {
        if (action->isAction(ENTER)) {
            action->setAction(ENTER, false);
            if (status->isOniLink()) {
                AudioManager::getInstance()->stopMusic();
                MainController::getInstance()->setStep(LOGO);
            } else {
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setStep(SCENE_TRANSITION_LIVE);
            }
        }
        return;
    }

    if (action->isAction(QUIT_GAME)) {
        MainController::getInstance()->getGameController()->displayText(13);
        return;
    }

    bool isAble = true;

    int prevX = x;
    int prevY = y;

    charging = action->isAction(LOAD);
    if (charge && !charging) {
        if (charge == 21) {
            AudioManager::getInstance()->playSound(TS_SPIN);
            setAnimation(SPIN_EPEE);
        }
        charge = 0;
    }

    if ((animation != IDLE && animation != WALK && animation != PUSH && charge == 0) || porte != 0) {
        isAble = false;
    }

    if (porte != 0 && animation != SOULEVE && ((action->isAction(HIT) && epee != 0) || action->isAction(ACTION) || action->isAction(CARRY))) {
        if (animation == PORTE && porte->isPosable()) {
            setAnimation(IDLE);
            lacheObj();
        } else {
            if (animation == PORTE) {
                elan = 2;
            } else {
                elan = speed;
            }
            setAnimation(THROW);
            lanceObj();
        }
    }

    if (isAble && action->isAction(CARRY) && status->isVisible() && !lapin) {
        isAble = !porteObjet();
    }

    if (isAble && action->isAction(ONI) && status->isVisible() && GlobalSave::getInstance()->isOniLinkUnlocked()) {
        setOniLink(!status->isOniLink(), false, false);
        status->resetOniLife();
        isAble = false;
    }

    if (isAble && action->isAction(ACTION) && !lapin && !status->isOniLink()) {
        isAble = !useStuff();
    }

    if (isAble && action->isAction(HIT) && epee != 0 && status->isVisible() && !lapin) {
        TypeSound type = TS_ATTACK1;
        switch (rand()%4) {
            case 0 : type = TS_ATTACK1; break;
            case 1 : type = TS_ATTACK2; break;
            case 2 : type = TS_ATTACK3; break;
            case 3 : type = TS_ATTACK4; break;
        }

        AudioManager::getInstance()->playSound(type);
        AudioManager::getInstance()->playSound(TS_SWORD);
        setAnimation(HIT_EPEE);
        isAble = false;
    }

    if (isAble && action->isAction(TALK)) {
        BoundingBox bb;
        bb.setW(16);
        bb.setH(16);
        switch (direction) {
            case N : bb.setX(x); bb.setY(y - 8 + 8); bb.setH(8); break;
            case S : bb.setX(x); bb.setY(y + 24); bb.setH(8); break;
            case W : bb.setX(x - 16 + 8); bb.setY(y + 8); bb.setW(8); break;
            case E : bb.setX(x + 16); bb.setY(y + 8); bb.setW(8); break;
        }
        isAble = !MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->tryToTalk(&bb, direction);
    }

    int nbDir = 0;
    int dx = 0;
    int dy = 0;
    speed = (inventory->hasObject(BOTTES) && action->isAction(RUN) && !status->getGel()) ? 4 : 2;

    if (isAble || animation == FLOTTE || animation == NAGE || animation == PORTE || animation == TRANSPORTE) {

        if (action->isAction(UP)) {nbDir++; dy--;}
        if (action->isAction(DOWN)) {nbDir++; dy++;}
        if (action->isAction(LEFT)) {nbDir++; dx--;}
        if (action->isAction(RIGHT)) {nbDir++; dx++;}

        if ((action->isAction(UP) && !action->isAction(DOWN))
            || (action->isAction(DOWN) && !action->isAction(UP))
            || (action->isAction(LEFT) && !action->isAction(RIGHT))
            || (action->isAction(RIGHT) && !action->isAction(LEFT))) {

            if (animation == IDLE || animation == FLOTTE || animation == PORTE) {
                if (isAble) {
                    setAnimation(WALK);
                } else if (porte) {
                    setAnimation(TRANSPORTE);
                } else {
                    setAnimation(NAGE);
                }
            } else if (animation == PUSH) {
                if (nbDir > 1 ||
                    (direction == N && !action->isAction(UP)) ||
                    (direction == S && !action->isAction(DOWN)) ||
                    (direction == W && !action->isAction(LEFT)) ||
                    (direction == E && !action->isAction(RIGHT))) {
                    if (isAble) {
                        setAnimation(WALK);
                    } else if (porte) {
                        setAnimation(TRANSPORTE);
                    } else {
                        setAnimation(NAGE);
                    }
                }
            }
        } else {
            if (isAble) {
                setAnimation(IDLE);
            } else if (porte) {
                setAnimation(PORTE);
            } else {
                setAnimation(FLOTTE);
            }
        }

        if (!charge) {
            findDirection(action->isAction(UP), action->isAction(DOWN), action->isAction(LEFT), action->isAction(RIGHT), nbDir);
        }

        moveX(dx, speed, nbDir);

        moveY(dy, speed, nbDir);

    }

    if (isOnIce()) {

        slide.setMoves(dx, dy, speed);

        if (wasOnIce) {

            int oldY = y;
            int oldX = x;

            dy = slide.getSlideY();
            if (dy < 0) {
                moveY(-1, -dy, 2);
            } else if (dy > 0) {
                moveY(1, dy, 2);
            }

            if (dy != 0 && oldY == y) {
                slide.stopY();
            }

            dx = slide.getSlideX();
            if (dx < 0) {
                moveX(-1, -dx, 2);
            } else if (dx > 0) {
                moveX(1, dx, 2);
            }

            if (dx != 0 && oldX == x) {
                slide.stopX();
            }

            if (grappin != 0 && animation == TIR_GRAPPIN) {
                grappin->slide(x - oldX, y - oldY);
            }

            if (flamme != 0 && animation == TIR_LANTERNE) {
                flamme->slide(x - oldX, y - oldY);
            }
        }

        wasOnIce =  true;
    } else if (wasOnIce) {
        slide.raz();
        wasOnIce = false;
    } else if (animation == WALK && !charge) {
        if (prevX == x && prevY == y && accuPush == 0 && nbDir < 2 && !lapin) {
            accuPush = 1;
        }
        if ((prevX != x || prevY != y) && accuPush != 0) {
            accuPush = 0;
        }

    } else if (animation == PUSH) {
        BoundingBox pushBox(x, y + 8, 16, 16);
        switch (direction) {
            case N : pushBox.setY(y); break;
            case S : pushBox.setY(y + 16); break;
            case W : pushBox.setX(x - 8); break;
            case E : pushBox.setX(x + 8); break;
        }
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        if (scene->checkCollisions(&pushBox, this, false, false, false, false, true, x, y + 8, 16, 16)) {
            if (isAble) {
                setAnimation(WALK);
            } else if (porte) {
                setAnimation(TRANSPORTE);
            } else {
                setAnimation(NAGE);
            }
        }
    }

}

void Link::loop() {

    status->loop();

    // if link is dead, nothing to do
    if (status->getVirtualLife() <= 0 && animation != TOUCHE && animation != COULE && animation != ELECTRIC) {
        return;
    }

    // hit
    if (status->getVirtualLife() > 0 && epee > 0) {
        bool hit = epee->hit(x, y, direction, animation, anim, charge, getForce());
        if (hit && charge) {
            charge = 0;
        }
    }

    // hit with byrna stars
    if (byrna) {
        hitByrna();
    }

    // check position

    // jump in water
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    Collision c = (Collision)map->getMur(x + 8, y + 16 /*+ 2*/);
    Collision c2 = (Collision)map->getMur(x + 7, y + 15 /*+ 2*/);
    if (c == EAU_PROF && c2 == EAU_PROF && (animation == WALK || animation == IDLE || animation == PUSH || animation == TRANSPORTE || animation == PORTE)) {
        jumpInWater();
    }

    // jump out water
    if ((animation == NAGE || animation == FLOTTE) && !isOnWater()) {
        jumpOutWater();
    }

    // fall
    if (animation != CHUTE && animation != TOUCHE && animation != COULE && animation != TIR_GRAPPIN && animation != ELECTRIC) {
        if (c == TROU && c2 == TROU) {
            fallInGap();
        }
    }


    // update histo
    if (animation != TOUCHE && animation != COULE && animation != TIR_GRAPPIN && animation != ELECTRIC) {
        for (int i = 6; i >= 0; i--) {
            oldx[i] = oldx[i + 1];
            oldy[i] = oldy[i + 1];
        }
        oldx[7] = x;
        oldy[7] = y;
    }


    if (charge && charge < 21) {
        charge++;
        if (charge == 20) {
            AudioManager::getInstance()->playSound(TS_SWORDCHARGING);
        }
    }

    map->pickUpItems(getBoundingBox());

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }

        switch (animation) {
            case WALK :
                if (accuPush > 0) {
                    accuPush++;
                    if (accuPush == 2) {
                        setAnimation(PUSH);
                        break;
                    }
                }
                if (anim == 0) playStep(0);
                else if (anim == 4) playStep(1);
                break;
            case PUSH :
                // try to push a pushable object
                if (anim == 0) {
                    pousse();
                }
                break;
            case TOUCHE :
                computeProjection();
                if (anim == 0) {
                    setAnimation(IDLE);
                    if (status->getVirtualLife() <= 0) {
                        AudioManager::getInstance()->playMusic(2);
                    }
                }
                break;
            case ELECTRIC :
                if (anim == 0) {
                    setAnimation(IDLE);
                    if (status->getVirtualLife() <= 0) {
                        AudioManager::getInstance()->playMusic(2);
                    }
                }
                break;
            case NAGE : if (anim == 0) AudioManager::getInstance()->playSound(TS_NAGE); break;
            case COULE :
                if (anim == 0) {
                    status->setLife(status->getLife() - 2);
                    if (status->getVirtualLife() > 0) {
                        x = oldx[0];
                        y = oldy[0];
                        setAnimation(IDLE);
                        status->setInvincible();
                    } else {
                        AudioManager::getInstance()->playMusic(2);
                        setAnimation(NOYE);
                    }
                    return;
                }
                break;
            case CHUTE :
                if (anim == 0) {
                    if (map->getId() == 11) {
                        setAnimation(NOYE);
                        MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(25, 29 * 16 + 8, 35 * 16, S, false, true);
                        MainController::getInstance()->getGameController()->setStep(GAME_TELEPORT);
                    } else if (map->getId() == 13) {
                        setAnimation(NOYE);
                        MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(27, 29 * 16 + 8, 36 * 16 + 8, S, false, true);
                        MainController::getInstance()->getGameController()->setStep(GAME_TELEPORT);
                    } else if (map->getId() == 58) {
                        setAnimation(NOYE);
                        MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(58, x + 100 * 16, y, direction, false, false, true);
                        MainController::getInstance()->getGameController()->setStep(GAME_TELEPORT);
                    } else if (map->getId() == 59 && y > 75 * 16) {
                        setAnimation(NOYE);
                        MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(59, x, y - 75 * 16, direction, false, false, true);
                        MainController::getInstance()->getGameController()->setStep(GAME_TELEPORT);
                    } else if (map->getId() == 60) {
                        setAnimation(NOYE);
                        if (x > 60 * 16) {
                            MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(60, x - 40 * 16, y + 15 * 16, direction, false, false, true);
                        } else if (y < 45 * 16) {
                            MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(60, x + 20 * 16, y + 60 * 16, direction, false, false, true);
                        } else {
                            MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(60, x + 120 * 16, y - 15 * 16, direction, false, false, true);
                        }
                        MainController::getInstance()->getGameController()->setStep(GAME_TELEPORT);
                    } else if (map->getId() == 62 && y > 75 * 16) {
                        setAnimation(NOYE);
                        MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(62, x, y - 75 * 16, direction, false, false, true);
                        MainController::getInstance()->getGameController()->setStep(GAME_TELEPORT);
                    } else if (map->getId() == 63 && y > 75 * 16) {
                        setAnimation(NOYE);
                        MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(63, x, y - 75 * 16, direction, false, false, true);
                        MainController::getInstance()->getGameController()->setStep(GAME_TELEPORT);
                    } else if (map->getId() == 64) {
                        setAnimation(NOYE);
                        MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(64, x, y - 75 * 16, direction, false, false, true);
                        MainController::getInstance()->getGameController()->setStep(GAME_TELEPORT);
                    } else {
                        status->setLife(status->getLife() - ((map->getId() == 62) ? 2 : 1));
                        if (status->getVirtualLife() > 0) {
                            x = startX2;
                            y = startY2;
                            direction = startDir2;
                            setAnimation(IDLE);
                            status->setInvincible();
                        } else {
                            AudioManager::getInstance()->playMusic(2);
                            setAnimation(NOYE);
                        }
                    }
                    return;
                }
                break;
            case HIT_EPEE :
                if (anim == 0) {
                    if (charging) {
                        charge = 1;
                    }
                    setAnimation(IDLE);
                }
                break;
            case SPIN_EPEE :
            case THROW :
            case TIR_LANTERNE:
                if (anim == 0) {
                    setAnimation(IDLE);
                }
                break;
            case TIR_ARC :
                if (anim == 0) {
                    setAnimation(IDLE);
                }
                if (anim == 2 && status->getArrows()) {
                    int a;
                    int b;
                    switch (direction) {
                        case N: a = x + 4; b = y; break;
                        case S: a = x + 6; b = y + 16; break;
                        case W: a = x - 4; b = y + 11; break;
                        case E: a = x + 4; b = y + 11; break;
                    }
                    TypeProjectile type;
                    switch (inventory->getCurrent()) {
                        default :
                        case ARC : type = TP_ARROW; break;
                        case ARC_FEE : type = TP_ARROW_2; break;
                    }
                    ProjectileHelper::getInstance()->addProjectile(type, a, b, direction);
                    AudioManager::getInstance()->playSound(TS_SHOOT);
                    status->setArrows(status->getArrows() - 1);
                }
                break;
            case TRANSPORTE :
                if (anim == 0) playStep(0);
                else if (anim == 2) playStep(1);
                break;
            case SOULEVE :
                if (anim == 0) {
                    Portable* p = getPortable();
                    if (p != 0 &&  p->getPoids() <= status->getMaxWeight()) {
                        porte = p;
                        p->onLift();
                        setAnimation(PORTE);
                        map->removeObject(p);
                        AudioManager::getInstance()->playSound(TS_PORTE);
                    } else {
                        porte = 0;
                        setAnimation(IDLE);
                    }
                }
                break;
            case TIR_BOOMERANG :
                if (anim == 0) {
                    setAnimation(IDLE);
                }
                if (anim == 1) {
                    TypeProjectile type;
                    switch (inventory->getCurrent()) {
                        default :
                        case BOOMERANG : type = TP_BOOMERANG; break;
                        case BOOMERANG_FEE : type = TP_BOOMERANG_2; break;
                    }
                    ProjectileHelper::getInstance()->addProjectile(type, x + 3, y + 8, direction);
                    boomerang = false;
                }
                break;
            case TIR_BAGUETTE_FEU :
                if (anim == 0) {
                    setAnimation(IDLE);
                }
                if (anim == 2 && status->getVirtualMagic() > 4) {
                    int a;
                    int b;
                    switch (direction) {
                        case N: a = x; b = y; break;
                        case S: a = x; b = y + 16; break;
                        case W: a = x - 16 + 1; b = y + 8; break;
                        case E: a = x + 16 - 1 - 4; b = y + 8; break;
                    }
                    ProjectileHelper::getInstance()->addProjectile(TP_BAGUETTE_FEU, a, b, direction);
                    AudioManager::getInstance()->playSound(TS_BAGUETTE_FEU);
                    status->updateMagic(-4);
                }
                break;
            case TIR_BAGUETTE_GLACE :
                if (anim == 0) {
                    setAnimation(IDLE);
                }
                if (anim == 2 && status->getVirtualMagic() > 4) {
                    int a;
                    int b;
                    switch (direction) {
                        case N: a = x; b = y; break;
                        case S: a = x; b = y + 16; break;
                        case W: a = x - 16 + 1; b = y + 8; break;
                        case E: a = x + 16 - 1 - 4; b = y + 8; break;
                    }
                    ProjectileHelper::getInstance()->addProjectile(TP_BAGUETTE_GLACE, a, b, direction);
                    AudioManager::getInstance()->playSound(TS_BAGUETTE_GLACE);
                    status->updateMagic(-4);
                }
                break;
            case TIR_MARTEAU:
                if (anim == 0) {
                    setAnimation(IDLE);
                } else if (anim == 2) {
                    BoundingBox bb;
                    bb.setW(16);
                    bb.setH(16);
                    switch (direction) {
                        case N : bb.setX(x); bb.setY(y - 8); break;
                        case S : bb.setX(x); bb.setY(y + 24); break;
                        case W : bb.setX(x - 16); bb.setY(y + 8); break;
                        case E : bb.setX(x + 16); bb.setY(y + 8); break;
                    }
                    MainController::getInstance()->getGameController()->getSceneController()->getScene()
                        ->testDegat(&bb, direction, 3, TA_MARTEAU, TE_NORMAL);
                    AudioManager::getInstance()->playSound(TS_MARTEAU);
                }
                break;
            case TIR_CANNE_SOMARIA :
                if (anim == 0) {
                    setAnimation(IDLE);
                    if (caisse == 0) {
                        if (status->getVirtualMagic() > 4) {
                            AudioManager::getInstance()->playSound(TS_CANE);
                            int a = 16 * ((x + 8)/16);
                            int b = 16 * ((y + 16)/16);
                            switch (direction) {
                                case N :
                                    b -= 16;
                                    if (b > y - 8) b -= 16;
                                    break;
                                case S :
                                    b += 16;
                                    if (b < y + 24) b += 16;
                                    break;
                                case W :
                                    a -= 16;
                                    if (a > x - 16) a -= 16;
                                    break;
                                case E :
                                    a += 16;
                                    if (a < x + 16) a += 16;
                                    break;
                            }

                            Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
                            scene->getMap()->addEffect(new FumeeBlanche(a + 8, b + 8, 4));
                            BoundingBox dest;
                            dest.setX(a);
                            dest.setY(b);
                            dest.setW(16);
                            dest.setH(16);
                            if (scene->checkCollisions(&dest, this, true)) {
                                caisse = new CaisseSomaria(a, b);
                                scene->getMap()->addObject(caisse);
                                status->updateMagic(-4);
                            }
                        }
                    } else {
                        AudioManager::getInstance()->playSound(TS_CANE);
                        caisse->explose();
                        caisse = 0;
                        notifySomariaEnd();
                    }
                }
                break;
            case TIR_CANNE_BYRNA :
                if (anim == 0) {
                    setAnimation(IDLE);
                    if (!byrna) {
                        if (status->getVirtualMagic() > 0) {
                            AudioManager::getInstance()->playSound(TS_CANE);
                            byrna = true;
                            animByrna = 0;
                            dirByrna = direction;
                        }
                    } else {
                        AudioManager::getInstance()->playSound(TS_CANE);
                        byrna = false;
                    }
                }
                break;
            case BOIT_POTION :
                if (anim == 0) {
                    setAnimation(IDLE);
                }
                if (anim == 2) {
                    Navi* navi = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getNavi();
                    switch (inventory->getCurrentFlacon()) {
                        case 1 : // rouge
                            status->updateLife(status->getMaxLife() - status->getVirtualLife());
                            if (status->getLife() == status->getMaxLife()) {
                                AudioManager::getInstance()->playSound(TS_ITEM);
                            }
                            break;
                        case 2 : // verte
                            status->updateMagic(status->getMaxMagic() - status->getVirtualMagic());
                            if (status->getMagic() == status->getMaxMagic()) {
                                AudioManager::getInstance()->playSound(TS_ITEM);
                            } else {
                                AudioManager::getInstance()->playSound(TS_MAGICCHARGE);
                            }
                            break;
                        case 3 : // bleue
                            status->updateLife(status->getMaxLife() - status->getVirtualLife());
                            status->updateMagic(status->getMaxMagic() - status->getVirtualMagic());
                            if (status->getMagic() == status->getMaxMagic()
                                && status->getLife() == status->getMaxLife()) {
                                AudioManager::getInstance()->playSound(TS_ITEM);
                            } else if (status->getMagic() != status->getMaxMagic()) {
                                AudioManager::getInstance()->playSound(TS_MAGICCHARGE);
                            }
                            break;
                        case 4 : // jaune
                            if (navi->getMagic() == navi->getMaxMagic()) {
                                AudioManager::getInstance()->playSound(TS_ITEM);
                            } else {
                                AudioManager::getInstance()->playSound(TS_MAGICCHARGE);
                            }
                            navi->setMagic(navi->getMaxMagic());
                            break;
                        case 5 : // orange
                            status->updateLife(status->getMaxLife() - status->getVirtualLife());
                            AudioManager::getInstance()->playSound(TS_ITEM);
                            navi->setMagic(navi->getMaxMagic());
                            break;
                        case 6 : // violette
                            status->updateMagic(status->getMaxMagic() - status->getVirtualMagic());
                            navi->setMagic(navi->getMaxMagic());
                            AudioManager::getInstance()->playSound(TS_ITEM);
                            break;
                        case 7 : // grise
                            status->updateLife(status->getMaxLife() - status->getVirtualLife());
                            status->updateMagic(status->getMaxMagic() - status->getVirtualMagic());
                            status->setArrows(status->getMaxArrows());
                            status->setBombs(status->getMaxBombs());
                            navi->setMagic(navi->getMaxMagic());
                            AudioManager::getInstance()->playSound(TS_ITEM);
                            break;
                        default : break;
                    }
                    inventory->setCurrentFlacon(0);
                }
                break;
            default : break;
        }

        chrono.reset();
    }

    if (animation == IDLE && c == EAU_PROF && c2 == EAU_PROF) {
        // plouf
        AudioManager::getInstance()->playSound(TS_PLOUF);
        map->addEffect(new Plouf(x, y));

        if (inventory->hasObject(PALMES)) {
            setAnimation(FLOTTE);
        } else {
            setAnimation(COULE);
        }
    }

    if (porte != 0 && animation != SOULEVE) {
        if (!porte->isAlive()) {
            delete porte;
            porte = 0;
            if (animation == PORTE) {
                setAnimation(IDLE);
            } else {
                setAnimation(WALK);
            }
        } else {
            porte->portLoop();
        }
    }

    if (grappin != 0) {
        if (animation == TIR_GRAPPIN) {
            grappin->projLoop();
        } else {
            delete grappin;
            grappin = 0;
        }
    }

    if (flamme != 0) {
        if (animation == TIR_LANTERNE) {
            flamme->loop();
        } else {
            delete flamme;
            flamme = 0;
        }
    }

    if (byrna) {
        animByrna++;
        if (animByrna > 27) {
            animByrna = 10;
            status->updateMagic(-2);
            if (status->getVirtualMagic() > 1) {
                AudioManager::getInstance()->playSound(TS_BYRNA);
            } else {
                byrna = false;
            }
        }
    }

}

void Link::draw(int offsetX, int offsetY) {

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    int dstX2 = dstX;

    if (status->getVirtualLife() > 0 &&
        (animation == IDLE || animation == WALK ||
         animation == SPIN_EPEE || animation == TIR_ARC ||
         animation == TIR_BOOMERANG ||
         animation == TIR_CANNE_SOMARIA || animation == TIR_CANNE_BYRNA)
        && status->isVisible()) {
        if (direction == W) {
            dstX2 = dstX + 2;
        } else if (direction == E) {
            dstX2 = dstX - 2;
        }
    }

    if (status->getVirtualLife() > 0 && direction == N && bouclier > 0 && animation != HIT_EPEE && !charge && status->isVisible() && !lapin) {
        bouclier->draw(dstX2, dstY, direction, animation, anim, 0);
    }

    if (status->getVirtualLife() > 0 && epee > 0 && animation == SPIN_EPEE && status->isVisible()) {
        if ((direction == N && anim >= 9 && anim <= 12)
            || (direction == S && anim >= 5 && anim <= 17)
            || (direction == W && anim >= 13 && anim <= 16)
            || (direction == E && anim >= 6 && anim <= 8))
        epee->draw(dstX2, dstY, direction, animation, anim, charge);
    }

    if (status->isVisible()) {

        switch (animation) {
            case IDLE :
                if (status->getVirtualLife() <= 0) drawDead(dstX2, dstY);
                else if (charge) drawCharge(dstX2, dstY);
                else drawIdle(dstX2, dstY); break;
            case WALK :
                if (charge) drawCharge(dstX2, dstY);
                else drawWalk(dstX2, dstY); break;
            case PUSH : drawPush(dstX2, dstY); break;
            case TOUCHE : drawTouche(dstX2, dstY); break;
            case ELECTRIC : drawElectric(dstX2, dstY); break;
            case FLOTTE :
            case NAGE : if (status->getVirtualLife() > 0) drawFlotte(dstX2, dstY); break;
            case COULE : drawCoule(dstX2, dstY); break;
            case CHUTE : drawChute(dstX2, dstY); break;
            case HIT_EPEE : drawEpee(dstX2, dstY); break;
            case SPIN_EPEE : drawSpin(dstX2, dstY); break;
            case TROUVE_SIMPLE : drawTrouveSimple(dstX2, dstY); break;
            case TROUVE_DOUBLE : drawTrouveDouble(dstX2, dstY); break;
            case TIR_ARC : drawArc(dstX2, dstY); break;
            case PORTE :
            case TRANSPORTE : drawPorte(dstX2, dstY, offsetX, offsetY); break;
            case THROW : drawThrow(dstX2, dstY); break;
            case SOULEVE : drawSouleve(dstX2, dstY); break;
            case TIR_GRAPPIN : drawGrappin(dstX2, dstY, offsetX, offsetY); break;
            case TIR_BOOMERANG : drawBoomerang(dstX2, dstY); break;
            case TIR_BAGUETTE_FEU :
            case TIR_BAGUETTE_GLACE :
            case TIR_MARTEAU : drawBaguette(dstX2, dstY); break;
            case TIR_LANTERNE : drawLanterne(dstX2, dstY, offsetX, offsetY); break;
            case TIR_CANNE_SOMARIA :
            case TIR_CANNE_BYRNA : drawCanne(dstX2, dstY); break;
            case BOIT_POTION : drawFlacon(dstX2, dstY); break;
            case JOUE_OCARINA : drawOcarina(dstX2, dstY); break;
            default : break;
        }

    } else {
        drawShadow(dstX2, dstY);
    }


    if (animation != TOUCHE && animation != CHUTE && animation != ELECTRIC) {
        Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
        Collision c = (Collision)map->getMur(x + 7, y + 16);
        if (c == HERBE_HAUTE) {
            int tmp = (animation == WALK) ? (anim / 2) : 0;
            WindowManager::getInstance()->draw(imageEffets, tmp * 16, 0, 16, 8, dstX, dstY + 16);
        }
        if (c == EAU || (c == EAU_PROF && ((Collision)map->getMur(x + 8, y + 16)) == EAU)) {
            int tmp = (metronome->getValue() / 2) % 4;
            WindowManager::getInstance()->draw(imageEffets, tmp * 16, 8, 16, 8, dstX, dstY + 16);
        }
    }

    if (status->getVirtualLife() > 0 && (direction != N || animation == HIT_EPEE || charge) && bouclier > 0 && status->isVisible() && !lapin) {
        bouclier->draw(dstX2, dstY, direction, animation, anim, charge);
    }

    if (status->getVirtualLife() > 0 && epee > 0 && status->isVisible()) {
        if (animation != SPIN_EPEE || !(
            (direction == N && anim >= 9 && anim <= 12)
            || (direction == S && anim >= 5 && anim <= 17)
            || (direction == W && anim >= 13 && anim <= 16)
            || (direction == E && anim >= 6 && anim <= 8)))
        epee->draw(dstX2, dstY, direction, animation, anim, charge);
    }

    if (byrna) {
        drawByrna(dstX2, dstY, offsetX, offsetY);
    }

}

void Link::setAnimation(Animation newAnim) {
    if (animation == newAnim) {
        return;
    }
    if (porte != 0 && animation == SOULEVE && (newAnim == TOUCHE || newAnim == CHUTE || newAnim == ELECTRIC)) {
        porte = 0;
    }

    accuPush = 0;
    animation = newAnim;
    anim = 0; animMax = 0;
    switch (animation) { // animMax = nbAnim - 1
        case WALK : anim = 0; animMax = 7; vanim = 90; playStep(0); break;
        case PUSH : anim = 0; animMax = 14; vanim = 80; break;
        case TOUCHE :
            anim = 0; animMax = 9; vanim = 30;
            charge = 0; byrna = false;
            if (porte != 0) lacheObj();
            status->setVisible(true);
            break;
        case ELECTRIC :
            anim = 0; animMax = 9; vanim = 90;
            charge = 0; byrna = false;
            if (porte != 0) lacheObj();
            status->setVisible(true);
            break;
        case FLOTTE : anim = 0; animMax = 1; vanim = 240; break;
        case NAGE : anim = 0; animMax = 3; vanim = 240; AudioManager::getInstance()->playSound(TS_NAGE); break;
        case COULE : anim = 0; animMax = 4; vanim = 240; break;
        case CHUTE :
            anim = 0; animMax = 5; vanim = 240;
            charge = 0; byrna = false;
            if (porte != 0) lacheObj();
            status->setVisible(true);
            break;
        case HIT_EPEE : anim = 0; animMax = 4; vanim = 45; break;
        case SPIN_EPEE : anim = 0; animMax = 30; vanim = 10; break;
        //case TROUVE_SIMPLE : break;
        case TIR_ARC : anim = 0; animMax = 2; vanim = 120; break;
        //case PORTE : break;
        case TRANSPORTE : anim = 0; animMax = 3; vanim = 180; playStep(0); break;
        case THROW : anim = 0; animMax = 0; vanim = 120; break;
        case SOULEVE : anim = 0; animMax = 1; vanim = 180; break;
        case TIR_GRAPPIN :
            anim = 0; animMax = 0; vanim = 240;
            for (int i = 0; i < 8; i++) {
                oldx[i] = x;
                oldy[i] = y;
            }
            break;
        case TIR_BOOMERANG : anim = 0; animMax = 1; vanim = 240; break;
        case TIR_BAGUETTE_FEU :
        case TIR_BAGUETTE_GLACE : anim = 0; animMax = 2; vanim = 120; break;
        case TIR_LANTERNE : anim = 0; animMax = 3; vanim = 240; break; // to avoid problem if 'enter' is pressed
        case TIR_MARTEAU : anim = 0; animMax = 2; vanim = 120; break;
        case TIR_CANNE_SOMARIA : anim = 0; animMax = 2; vanim = 120; break;
        case TIR_CANNE_BYRNA : anim = 0; animMax = 2; vanim = 120; break;
        case BOIT_POTION : anim = 0; animMax = 2; vanim = 360; break;
        case JOUE_OCARINA : anim = 0; animMax = 0; direction = S; break;
        default : break;
    }
    chrono.reset();
}

void Link::findDirection(bool up, bool down, bool left, bool right, int nb_dir) {

    switch (nb_dir) {
        case 1 :
            if (up) {direction = N; break;}
            if (down) {direction = S; break;}
            if (left) {direction = W; break;}
            if (right) {direction = E; break;}
            break;
        case 2 :
            if (up && direction != N) {
                if (left && direction != W) {
                    direction = N;
                    break;
                }
                if (right && direction != E) {
                    direction = N;
                    break;
                }
                break;
            }
            if (down && direction != S) {
                if (left && direction != W) {
                    direction = S;
                    break;
                }
                if (right && direction != E) {
                    direction = S;
                    break;
                }
                break;
            }
            break;
        case 3 :
            if (!up) {direction = S; break;}
            if (!down) {direction = N; break;}
            if (!left) {direction = E; break;}
            if (!right) {direction = W; break;}
            break;
        default : break;
    }

}

void Link::moveX(int dx, int vitesse, int nbDir) {

    int v = vitesse;
    if (dx != 0) {
        int modX = x % vitesse;
        if (modX != 0) {
            if (dx > 0) {
                v -= modX;
            } else {
                v = modX;
            }
        }
    }

    dx *= v;
    BoundingBox* bb = getBoundingBox();
    bb->setX(x + dx);

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    if (scene->checkCollisions(bb, this, false, false, false, false, true, x, y + 8, 16, 16)) {
        x += dx;
        moved = true;
    } else if (nbDir == 1) {

        int mod = y % 8;

        bb->setY(y + 8 - (mod == 0 ? 8 : mod));
        if (scene->checkCollisions(bb, this, false)) {
            moveY(-1, vitesse, 0);
        } else {
            bb->setY(y + 8 + 8 - mod);
            if (scene->checkCollisions(bb, this, false)) {
                moveY(1, vitesse, 0);
            }
        }
    }
}

void Link::moveY(int dy, int vitesse, int nbDir) {

    int v = vitesse;
    if (dy != 0) {
        int modY = y % vitesse;
        if (modY != 0) {
            if (dy > 0) {
                v -= modY;
            } else {
                v = modY;
            }
        }
    }

    dy *= v;
    BoundingBox* bb = getBoundingBox();
    bb->setY(y + dy + 8);

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    if (scene->checkCollisions(bb, this, false, false, false, false, true, x, y + 8, 16, 16)) {
        y += dy;
        moved = true;
    } else if (nbDir == 1) {

        int mod = x % 8;

        bb->setX(x - (mod == 0 ? 8 : mod)) ;
        if (scene->checkCollisions(bb, this, false)) {
            moveX(-1, vitesse, 0);
        } else {
            bb->setX(x + 8 - mod);
            if (scene->checkCollisions(bb, this, false)) {
                moveX(1, vitesse, 0);
            }
        }
    }
}

bool Link::hasMoved() {
    return moved;
}

/**
 * id have to be between 0 and 1
 */
void Link::playStep(int id) {

    Collision c = (Collision)MainController::getInstance()->getGameController()->
        getSceneController()->getScene()->getMap()->getMur(x + 7, y + 16 /*+ 2*/);

    TypeSound type;

    switch (c) {
        case HERBE : type = id == 0 ? TS_STEP_GRASS1 : TS_STEP_GRASS2; break;
        case HERBE_HAUTE : type = id == 0 ? TS_STEP_TALLGRASS1 : TS_STEP_TALLGRASS2; break;
        case TERRE : type = id == 0 ? TS_STEP_DIRT1 : TS_STEP_DIRT2; break;
        case GLACE : type = id == 0 ? TS_STEP_ICE1 : TS_STEP_ICE2; break;
        case EAU : type = id == 0 ? TS_STEP_WATER1 : TS_STEP_WATER2; break;
        case SABLE : type = id == 0 ? TS_STEP_SAND1 : TS_STEP_SAND2; break;
        case PIERRE : type = id == 0 ? TS_STEP_STONE1 : TS_STEP_STONE2; break;
        case SOL_BOIS : type = id == 0 ? TS_STEP_WOOD1 : TS_STEP_WOOD2; break;
        case TAPIS : type = id == 0 ? TS_STEP_CARPET1 : TS_STEP_CARPET2; break;
        default : return;
    }

    AudioManager::getInstance()->playSound(type);

}

void Link::jumpInWater() {

    AudioManager::getInstance()->playSound(TS_JUMP);
    setAnimation(TOUCHE);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    toucheX = 0;
    toucheY = 0;

    if ((Collision)map->getMur(x + 8, y + 8) != EAU_PROF) {
        toucheY = 1;
    }
    if ((y - 8) % 16 == 0) {
        if ((Collision)map->getMur(x + 8, y + 23) != EAU_PROF ||
            ((Collision)map->getMur(x + 8 + 16, y + 23/*+1*/) != EAU_PROF &&
             (Collision)map->getMur(x + 8 - 16, y + 23/*+1*/) != EAU_PROF)) {
            toucheY = -1;
        }
    } else {
        if ((Collision)map->getMur(x + 8, y + 24) != EAU_PROF) {
            toucheY = -1;
        }
    }

    if ((Collision)map->getMur(x, y + 16 /*+ 2*/) != EAU_PROF) {
        toucheX = 1;
    }

    if (x % 16 == 0) {
        if ((Collision)map->getMur(x + 15, y + 16 /*+ 2*/) != EAU_PROF ||
            ((Collision)map->getMur(x + 15/*+1*/, y + 16 - 16 /*+ 2*/) != EAU_PROF &&
            (Collision)map->getMur(x + 15/*+1*/, y + 16 + 16 /*+ 2*/) != EAU_PROF)) {
            toucheX = -1;
        }
    } else {
        if ((Collision)map->getMur(x + 16, y + 16 /*+ 2*/) != EAU_PROF) {
            toucheX = -1;
        }
    }

    if (toucheX == 0 && toucheY == 0) {
        if ((Collision)map->getMur(x, y + 8) != EAU_PROF) {
            toucheX = 1;
            toucheY = 1;
        }
        if ((Collision)map->getMur(x + 15, y + 8) != EAU_PROF) {
            toucheX = -1;
            toucheY = 1;
        }
        if ((Collision)map->getMur(x, y + 15 + 8) != EAU_PROF) {
            toucheX = 1;
            toucheY = -1;
        }
        if ((Collision)map->getMur(x + 15, y + 15 + 8) != EAU_PROF) {
            toucheX = -1;
            toucheY = -1;
        }
    }
}

void Link::jumpOutWater() {
    AudioManager::getInstance()->playSound(TS_JUMP);
    setAnimation(TOUCHE);

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    toucheX = 0;
    toucheY = 0;

    if ((Collision)map->getMur(x + 8, y + 8) != EAU_PROF) {
        toucheY = -1;
    }
    if ((y - 8) % 8 == 0 && (y - 8) % 16 != 0) {
        if ((Collision)map->getMur(x + 8, y + 24) != EAU_PROF) {
            toucheY = 1;
        }
    } else {
        if ((Collision)map->getMur(x + 8, y + 23) != EAU_PROF) {
            toucheY = 1;
        }
    }

    if ((Collision)map->getMur(x, y + 16) != EAU_PROF) {
        toucheX = -1;
    }
    if (x % 8 == 0 && x % 16 != 0) {
        if ((Collision)map->getMur(x + 16, y + 16) != EAU_PROF) {
            toucheX = 1;
        }
    } else {
        if ((Collision)map->getMur(x + 15, y + 16) != EAU_PROF) {
            toucheX = 1;
        }
    }


    if (toucheX == 0 && toucheY == 0) {
        if ((Collision)map->getMur(x, y + 8) != EAU_PROF) {
            toucheX = -1;
            toucheY = -1;
        }
        if ((Collision)map->getMur(x + 15, y + 8) != EAU_PROF) {
            toucheX = 1;
            toucheY = -1;
        }
        if ((Collision)map->getMur(x, y + 15 + 8) != EAU_PROF) {
            toucheX = -1;
            toucheY = 1;
        }
        if ((Collision)map->getMur(x + 15, y + 15 + 8) != EAU_PROF) {
            toucheX = 1;
            toucheY = 1;
        }
    }
}

void Link::computeProjection() {
    if (toucheY != 0) {
        int prevY = y;
        moveY(toucheY, 2, 1);
        if (prevY == y) toucheY = 0;
    }
    if (toucheX != 0) {
        int prevX = x;
        moveX(toucheX, 2, 1);
        if (prevX == x) toucheX = 0;
    }
}

void Link::fallInGap() {

    if (isOnGap()) {
        setAnimation(CHUTE);
        AudioManager::getInstance()->playSound(TS_FALL);
        return;
    }

    if (metronome->getValue() % 2 == 0) {
        return;
    }

    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    int tombeX = 0;
    int tombeY = 0;

    if ((Collision)map->getMur(x + 8, y + 8) != TROU) {
        tombeY = 1;
    }
    if ((Collision)map->getMur(x + 8, y + 23) != TROU) {
        tombeY = -1;
    }
    if ((Collision)map->getMur(x, y + 16) != TROU) {
        tombeX = 1;
    }
    if ((Collision)map->getMur(x + 15, y + 16) != TROU) {
        tombeX = -1;
    }

    if (tombeY != 0) {
        moveY(tombeY, 1, 1);
    }
    if (tombeX != 0) {
        moveX(tombeX, 1, 1);
    }
}

Animation Link::getAnimation() {
    return animation;
}

BoundingBox* Link::getBoundingBox() {
    box.setX(x);
    box.setY(y + 8);
    box.setW(w);
    box.setH(h - 8);
    return &box;
}

Status* Link::getStatus() {return status;}
Inventory* Link::getInventory() {return inventory;}

int Link::getX() {return x;}
int Link::getY() {return y;}
int Link::getW() {return w;}
int Link::getH() {return h;}
Direction Link::getDirection() {return direction;}

void Link::setX(int i) {x = i;}
void Link::setY(int j) {y = j;}
void Link::setDirection(Direction dir) {direction = dir;}

int Link::getTunique() {return tunique;}
int Link::getEpee() {return (epee == 0 ? 0 : epee->getForce());}
int Link::getBouclier() {return (bouclier == 0 ? 0 : bouclier->getLevel());}

void Link::setTunique(int i) {
    tunique = i;
    ResourceManager::getInstance()->free(imageLink);
    ostringstream os;
    os << ConfigurationManager::getInstance()->getSkinName() << tunique;
    string filename = "data/images/link/link" + os.str() + ".png";
    imageLink = ResourceManager::getInstance()->loadImage(filename, true);
}

void Link::setEpee(int i) {
    delete epee;
    if (i > 0 && i < 6) {
        epee = new Sword(i, status->isOniLink());
    }
}

void Link::setBouclier(int i) {
    delete bouclier;
    if (i > 0 && i < 5) {
        bouclier = new Shield(i, status->isOniLink());
    }
}

bool Link::isByrna() {
    return byrna;
}

bool Link::isAbleToChangeStuff() {
    return (animation == IDLE || animation == WALK) && charge == 0 && boomerang && !byrna && status->isVisible() && !lapin && !status->isOniLink();
}

void Link::trouveObjet(TypeItem type, int id) {
    AudioManager::getInstance()->playSound(TS_HAPPY, 2);

    int tmp = 0;
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    switch (type) {
        case TI_QUART_COEUR :
            inventory->setQuartCoeur(id);
            if (inventory->nbQuartCoeur() == 0) {
                status->setMaxLife(status->getMaxLife() + 2);
            }
            status->updateLife(status->getMaxLife() - status->getLife());
            MainController::getInstance()->getGameController()->displayText(10);
            break;
        case TI_FLACON_1 :
            inventory->setObject(FLACON_1);
            MainController::getInstance()->getGameController()->displayText(37);
            break;
        case TI_FLACON_2 :
            inventory->setObject(FLACON_2);
            MainController::getInstance()->getGameController()->displayText(37);
            break;
        case TI_FLACON_3 :
            inventory->setObject(FLACON_3);
            MainController::getInstance()->getGameController()->displayText(37);
            break;
        case TI_FLACON_4 :
            inventory->setObject(FLACON_4);
            MainController::getInstance()->getGameController()->displayText(37);
            break;
        case TI_GEMME_FORCE :
            inventory->setGemmeForce(id);
            MainController::getInstance()->getGameController()->displayText(39);
            break;
        case TI_COQUILLAGE :
            inventory->setCoquillages(id);
            MainController::getInstance()->getGameController()->displayText(70);
            break;
        case TI_SAC_RIZ :
            inventory->addTroc(TT_SAC_RIZ);
            MainController::getInstance()->getGameController()->displayText(97);
            break;
        case TI_SAC_OLIVES :
            inventory->addTroc(TT_SAC_OLIVES);
            MainController::getInstance()->getGameController()->displayText(98);
            break;
        case TI_BOCAL_EPICES :
            inventory->addTroc(TT_BOCAL_EPICES);
            MainController::getInstance()->getGameController()->displayText(99);
            break;
        case TI_POULET :
            inventory->addTroc(TT_POULET);
            MainController::getInstance()->getGameController()->displayText(100);
            break;
        case TI_PLUME :
            inventory->addTroc(TT_PLUME);
            MainController::getInstance()->getGameController()->displayText(101);
            break;
        case TI_PERLES :
            inventory->addTroc(TT_PERLES);
            MainController::getInstance()->getGameController()->displayText(102);
            break;
        case TI_TAPISSERIE :
            inventory->addTroc(TT_TAPISSERIE);
            MainController::getInstance()->getGameController()->displayText(103);
            break;
        case TI_AMULETTE :
            inventory->addTroc(TT_AMULETTE);
            MainController::getInstance()->getGameController()->displayText(104);
            break;
        case TI_PARFUM :
            inventory->addTroc(TT_PARFUM);
            MainController::getInstance()->getGameController()->displayText(105);
            break;
        case TI_ROC_GIVRE :
            inventory->addTroc(TT_ROC_GIVRE);
            MainController::getInstance()->getGameController()->displayText(561);
            break;
        case TI_BOURSE_1 :
            status->setMaxRupees(99);
            MainController::getInstance()->getGameController()->displayText(121);
            break;
        case TI_BOURSE_2 :
            status->setMaxRupees(200);
            MainController::getInstance()->getGameController()->displayText(122);
            break;
        case TI_BOURSE_3 :
            status->setMaxRupees(500);
            MainController::getInstance()->getGameController()->displayText(123);
            break;
        case TI_BOURSE_4 :
            status->setMaxRupees(999);
            MainController::getInstance()->getGameController()->displayText(124);
            break;
        case TI_BOUCLIER_1 :
            setBouclier(1);
            MainController::getInstance()->getGameController()->displayText(132);
            break;
        case TI_BOUCLIER_2 :
            setBouclier(2);
            MainController::getInstance()->getGameController()->displayText(133);
            break;
        case TI_BOUCLIER_3 :
            setBouclier(3);
            MainController::getInstance()->getGameController()->displayText(134);
            break;
        case TI_CHANT_1 :
            inventory->setObject(CHANT_1);
            MainController::getInstance()->getGameController()->displayText(210);
            break;
        case TI_CHANT_2 :
            inventory->setObject(CHANT_2);
            MainController::getInstance()->getGameController()->displayText(211);
            break;
        case TI_CHANT_3 :
            inventory->setObject(CHANT_3);
            MainController::getInstance()->getGameController()->displayText(212);
            break;
        case TI_EPEE_1 :
            setEpee(1);
            MainController::getInstance()->getGameController()->displayText(219);
            break;
        case TI_EPEE_2 :
            setEpee(2);
            MainController::getInstance()->getGameController()->displayText(221);
            break;
        case TI_EPEE_3 :
            setEpee(3);
            MainController::getInstance()->getGameController()->displayText(222);
            break;
        case TI_EPEE_5 :
            setEpee(5);
            MainController::getInstance()->getGameController()->displayText(223);
            break;
        case TI_RECETTE_POTION_JAUNE :
            inventory->setObject(RECETTE_POTION_JAUNE);
            MainController::getInstance()->getGameController()->displayText(265);
            break;
        case TI_GANT :
            inventory->setObject(GANTS);
            MainController::getInstance()->getGameController()->displayText(304);
            break;
        case TI_GANT2 :
            inventory->setObject(GANTS_2);
            MainController::getInstance()->getGameController()->displayText(305);
            break;
        case TI_ARC :
            inventory->setObject(ARC);
            status->setMaxArrows(30);
            status->setArrows(30);
            MainController::getInstance()->getGameController()->displayText(306);
            break;
        case TI_ARC_FEE :
            inventory->setObject(ARC_FEE);
            MainController::getInstance()->getGameController()->displayText(307);
            break;
        case TI_BOOMERANG :
            inventory->setObject(BOOMERANG);
            MainController::getInstance()->getGameController()->displayText(308);
            break;
        case TI_BOOMERANG_FEE :
            inventory->setObject(BOOMERANG_FEE);
            MainController::getInstance()->getGameController()->displayText(309);
            break;
        case TI_COEUR :
            inventory->setCoeur(id);
            status->setMaxLife(status->getMaxLife() + 2);
            status->updateLife(status->getMaxLife() - status->getLife());
            MainController::getInstance()->getGameController()->displayText(318);
            break;
        case TI_PALMES :
            inventory->setObject(PALMES);
            MainController::getInstance()->getGameController()->displayText(322);
            break;
        case TI_OCARINA :
            inventory->setObject(OCARINA);
            MainController::getInstance()->getGameController()->displayText(324);
            break;
        case TI_MARTEAU :
            inventory->setObject(MARTEAU);
            MainController::getInstance()->getGameController()->displayText(325);
            break;
        case TI_LANTERNE :
            inventory->setObject(LANTERNE);
            if (status->getMaxMagic() == 0) {
                status->setMaxMagic(32);
            }
            status->setMagic(status->getMaxMagic());
            MainController::getInstance()->getGameController()->displayText(338);
            break;
        case TI_PERLE_LUNE :
            inventory->setObject(PERLE_LUNE);
            MainController::getInstance()->getGameController()->displayText(339);
            break;
        case TI_CARTE_ILE :
            inventory->setObject(CARTE_ILE);
            MainController::getInstance()->getGameController()->displayText(343);
            break;
        case TI_DETECTEUR :
            inventory->setObject(DETECTEUR);
            MainController::getInstance()->getGameController()->displayText(348);
            break;
        case TI_TUNIQUE_BLEUE :
            MainController::getInstance()->getGameController()->displayText(352);
            break;
        case TI_TUNIQUE_ROUGE :
            MainController::getInstance()->getGameController()->displayText(355);
            break;
        case TI_CANNE_BYRNA :
            inventory->setObject(CANNE_BYRNA);
            if (status->getMaxMagic() == 0) {
                status->setMaxMagic(32);
            }
            status->setMagic(status->getMaxMagic());
            MainController::getInstance()->getGameController()->displayText(358);
            break;
        case TI_BOTTES :
            inventory->setObject(BOTTES);
            MainController::getInstance()->getGameController()->displayText(363);
            break;
        case TI_SAC_TROC :
            inventory->setObject(SAC_TROC);
            inventory->setTroc(TT_SAC_RIZ, 2);
            inventory->setTroc(TT_SAC_OLIVES, 2);
            inventory->setTroc(TT_BOCAL_EPICES, 2);
            MainController::getInstance()->getGameController()->displayText(427);
            break;
        case TI_SAC_BOMBES :
            inventory->setObject(BOMBES);
            status->setMaxBombs(10);
            status->setBombs(10);
            MainController::getInstance()->getGameController()->displayText(458);
            break;
        case TI_POTION_ROUGE :
            if (inventory->hasObject(FLACON_1) && !inventory->getFlacon(0)) tmp = 0;
            else if (inventory->hasObject(FLACON_2) && !inventory->getFlacon(1)) tmp = 1;
            else if (inventory->hasObject(FLACON_3) && !inventory->getFlacon(2)) tmp = 2;
            else if (inventory->hasObject(FLACON_4) && !inventory->getFlacon(3)) tmp = 3;
            inventory->setFlacon(tmp, 1);
            MainController::getInstance()->getGameController()->displayText(529);
            break;
        case TI_POTION_VERTE :
            if (inventory->hasObject(FLACON_1) && !inventory->getFlacon(0)) tmp = 0;
            else if (inventory->hasObject(FLACON_2) && !inventory->getFlacon(1)) tmp = 1;
            else if (inventory->hasObject(FLACON_3) && !inventory->getFlacon(2)) tmp = 2;
            else if (inventory->hasObject(FLACON_4) && !inventory->getFlacon(3)) tmp = 3;
            inventory->setFlacon(tmp, 2);
            MainController::getInstance()->getGameController()->displayText(530);
            break;
        case TI_POTION_JAUNE :
            if (inventory->hasObject(FLACON_1) && !inventory->getFlacon(0)) tmp = 0;
            else if (inventory->hasObject(FLACON_2) && !inventory->getFlacon(1)) tmp = 1;
            else if (inventory->hasObject(FLACON_3) && !inventory->getFlacon(2)) tmp = 2;
            else if (inventory->hasObject(FLACON_4) && !inventory->getFlacon(3)) tmp = 3;
            inventory->setFlacon(tmp, 4);
            MainController::getInstance()->getGameController()->displayText(531);
            break;
        case TI_POTION_BLEUE :
            if (inventory->hasObject(FLACON_1) && !inventory->getFlacon(0)) tmp = 0;
            else if (inventory->hasObject(FLACON_2) && !inventory->getFlacon(1)) tmp = 1;
            else if (inventory->hasObject(FLACON_3) && !inventory->getFlacon(2)) tmp = 2;
            else if (inventory->hasObject(FLACON_4) && !inventory->getFlacon(3)) tmp = 3;
            inventory->setFlacon(tmp, 3);

            for (int i = 0; i < NB_FLACON; i++) {
                if (inventory->getFlacon(i) == 1) {
                    inventory->setFlacon(i, 0);
                    break;
                }
            }

            for (int i = 0; i < NB_FLACON; i++) {
                if (inventory->getFlacon(i) == 2) {
                    inventory->setFlacon(i, 0);
                    break;
                }
            }

            MainController::getInstance()->getGameController()->displayText(532);
            break;
        case TI_POTION_ORANGE :
            if (inventory->hasObject(FLACON_1) && !inventory->getFlacon(0)) tmp = 0;
            else if (inventory->hasObject(FLACON_2) && !inventory->getFlacon(1)) tmp = 1;
            else if (inventory->hasObject(FLACON_3) && !inventory->getFlacon(2)) tmp = 2;
            else if (inventory->hasObject(FLACON_4) && !inventory->getFlacon(3)) tmp = 3;
            inventory->setFlacon(tmp, 5);

            for (int i = 0; i < NB_FLACON; i++) {
                if (inventory->getFlacon(i) == 1) {
                    inventory->setFlacon(i, 0);
                    break;
                }
            }

            for (int i = 0; i < NB_FLACON; i++) {
                if (inventory->getFlacon(i) == 4) {
                    inventory->setFlacon(i, 0);
                    break;
                }
            }

            MainController::getInstance()->getGameController()->displayText(533);
            break;
        case TI_POTION_VIOLETTE :
            if (inventory->hasObject(FLACON_1) && !inventory->getFlacon(0)) tmp = 0;
            else if (inventory->hasObject(FLACON_2) && !inventory->getFlacon(1)) tmp = 1;
            else if (inventory->hasObject(FLACON_3) && !inventory->getFlacon(2)) tmp = 2;
            else if (inventory->hasObject(FLACON_4) && !inventory->getFlacon(3)) tmp = 3;
            inventory->setFlacon(tmp, 6);

            for (int i = 0; i < NB_FLACON; i++) {
                if (inventory->getFlacon(i) == 2) {
                    inventory->setFlacon(i, 0);
                    break;
                }
            }

            for (int i = 0; i < NB_FLACON; i++) {
                if (inventory->getFlacon(i) == 4) {
                    inventory->setFlacon(i, 0);
                    break;
                }
            }

            MainController::getInstance()->getGameController()->displayText(534);
            break;
        case TI_POTION_GRISE :
            if (inventory->hasObject(FLACON_1) && !inventory->getFlacon(0)) tmp = 0;
            else if (inventory->hasObject(FLACON_2) && !inventory->getFlacon(1)) tmp = 1;
            else if (inventory->hasObject(FLACON_3) && !inventory->getFlacon(2)) tmp = 2;
            else if (inventory->hasObject(FLACON_4) && !inventory->getFlacon(3)) tmp = 3;
            inventory->setFlacon(tmp, 7);

            for (int i = 0; i < NB_FLACON; i++) {
                if (inventory->getFlacon(i) == 3 || inventory->getFlacon(i) == 5 || inventory->getFlacon(i) == 6) {
                    inventory->setFlacon(i, 0);
                }
            }

            MainController::getInstance()->getGameController()->displayText(535);
            break;
        case TI_CARTE_DONJON :
            tmp = scene->getMap()->getId();
            scene->setCoffre(tmp - 57, 0, 1);
            MainController::getInstance()->getGameController()->displayText(586);
            break;
        case TI_BOUSSOLE :
            tmp = scene->getMap()->getId();
            scene->setCoffre(tmp - 57, 1, 1);
            MainController::getInstance()->getGameController()->displayText(587);
            break;
        case TI_CLE_BOSS :
            tmp = scene->getMap()->getId();
            scene->setCoffre(tmp - 57, 2, 1);
            MainController::getInstance()->getGameController()->displayText(588);
            break;
        case TI_CLE :
            tmp = scene->getMap()->getId();
            scene->setCoffre(tmp - 57, 3, scene->getCoffre(tmp - 57, 3) + 1);
            scene->setCoffre(tmp - 57, id, 1);
            MainController::getInstance()->getGameController()->displayText(589);
            break;
        case TI_MEDAILLON_1 : case TI_MEDAILLON_2 : case TI_MEDAILLON_3 : case TI_MEDAILLON_4 :
        case TI_MEDAILLON_5 : case TI_MEDAILLON_6 : case TI_MEDAILLON_7 :
            inventory->setMedaillons(id, 1);

            if (id == 3) {
                BoundingBox b(70*16-8, 6*16-8, 16, 16);
                if (x < 320*7) {
                    b.setX(210*16-8);
                }
                scene->getMap()->killItems(&b);
            }

            MainController::getInstance()->getGameController()->displayText(590 + id);
            break;
        case TI_GRAPPIN :
            inventory->setObject(GRAPPIN);
            MainController::getInstance()->getGameController()->displayText(621);
            break;
        case TI_BAGUETTE_FEU :
            inventory->setObject(BAGUETTE_FEU);
            status->setMagic(status->getMaxMagic());
            MainController::getInstance()->getGameController()->displayText(622);
            break;
        case TI_CANNE_SOMARIA :
            inventory->setObject(CANNE_SOMARIA);
            status->setMagic(status->getMaxMagic());
            MainController::getInstance()->getGameController()->displayText(623);
            break;
        case TI_RUBIS_ROUGE :
            tmp = scene->getMap()->getId();
            status->updateRupees(20);
            if (tmp >= 57) scene->setCoffre(tmp - 57, id, 1);
            MainController::getInstance()->getGameController()->displayText(624);
            break;
        case TI_CAPE_INVISIBILITE :
            inventory->setObject(CAPE_INVISIBILITE);
            status->setMagic(status->getMaxMagic());
            MainController::getInstance()->getGameController()->displayText(628);
            break;
        case TI_BAGUETTE_GLACE :
            inventory->setObject(BAGUETTE_GLACE);
            status->setMagic(status->getMaxMagic());
            MainController::getInstance()->getGameController()->displayText(631);
            break;
        default : break;
    }

    if ((animation == IDLE || animation == WALK || animation == PUSH) && charge == 0) {
        // anim with one arm or two
        if (type == TI_QUART_COEUR || type == TI_GEMME_FORCE || type == TI_COQUILLAGE
             || type == TI_SAC_RIZ || type == TI_SAC_OLIVES || type == TI_BOCAL_EPICES || type == TI_POULET
             || type == TI_PLUME || type == TI_PERLES || type == TI_TAPISSERIE || type == TI_AMULETTE
             || type == TI_PARFUM || type == TI_ROC_GIVRE || type == TI_EPEE_1 || type == TI_EPEE_2 || type == TI_EPEE_3
             || type == TI_EPEE_5 || type == TI_CLE || type == TI_CLE_BOSS || type == TI_CARTE_DONJON || type == TI_BOUSSOLE) {
            setAnimation(TROUVE_SIMPLE);
        } else {
            setAnimation(TROUVE_DOUBLE);
        }
        trouve = type;
    }
}

void Link::unlockBoomerang() {
    boomerang = true;
}

void Link::notifySomariaEnd() {
    if (caisse != 0) {
        caisse->killNode();
    }
    caisse = 0;
}

bool Link::useStuff() {
    Equipment e = inventory->getCurrent();

    if (!inventory->hasObject(e)) {
        return false;
    }

    switch (e) {
        case ARC :
        case ARC_FEE :
            setAnimation(TIR_ARC);
            return true;
        case BOOMERANG :
        case BOOMERANG_FEE :
            if (!boomerang) return false;
            setAnimation(TIR_BOOMERANG);
            return true;
        case BOMBES :
            if (status->getBombs()) {
                status->setBombs(status->getBombs() - 1);
                porte = new ObjBomb(x, y);
                setAnimation(PORTE);
                AudioManager::getInstance()->playSound(TS_PORTE);
                return true;
            }
            return false;
        case GRAPPIN :
            setAnimation(TIR_GRAPPIN);
            delete grappin;
            grappin = new ProjGrappin(x + 2, y + 10, direction);
            AudioManager::getInstance()->playSound(TS_GRAPPIN);
            return true;
        case BAGUETTE_FEU :
            setAnimation(TIR_BAGUETTE_FEU);
            return true;
        case BAGUETTE_GLACE :
            setAnimation(TIR_BAGUETTE_GLACE);
            return true;
        case LANTERNE :
            if (status->getVirtualMagic() > 0) {
                int a;
                int b;
                switch (direction) {
                    case N : a = x; b = y - 8; break;
                    case S : a = x; b = y + 24; break;
                    case W : a = x - 16; b = y + 8; break;
                    case E : a = x + 16; b = y + 8; break;
                }
                delete flamme;
                flamme = new Flamme(a, b, direction);
                setAnimation(TIR_LANTERNE);
                AudioManager::getInstance()->playSound(TS_BURN);
                status->updateMagic(-1);
                return true;
            }
            return false;

        case CAPE_INVISIBILITE :
            if (!status->isVisible()) {
                status->setVisible(true);
                return true;
            } else if (status->getVirtualMagic() > 0) {
                status->setVisible(false);
                return true;
            }
            return false;
        case MARTEAU :
            setAnimation(TIR_MARTEAU);
            return true;
        case OCARINA :
            setAnimation(JOUE_OCARINA);
            if (inventory->hasObject(CHANT_3)) {
                MainController::getInstance()->getGameController()->displayText(205);
            } else if (inventory->hasObject(CHANT_2)) {
                MainController::getInstance()->getGameController()->displayText(202);
            } else if (inventory->hasObject(CHANT_1)) {
                MainController::getInstance()->getGameController()->displayText(200);
            } else {
                MainController::getInstance()->getGameController()->displayText(199);
            }
            direction = S;
            return true;
        case CANNE_SOMARIA :
            setAnimation(TIR_CANNE_SOMARIA);
            return true;
            break;
        case CANNE_BYRNA :
            setAnimation(TIR_CANNE_BYRNA);
            return true;
            break;
        case FLACON_1 :
        case FLACON_2 :
        case FLACON_3 :
        case FLACON_4 :
            setAnimation(BOIT_POTION);
            return true;
        default : return false;
    }
}

bool Link::porteObjet() {
    if (porte != 0) {
        return false;
    }

    Portable* p = getPortable();
    if (p == 0) {
        return false;
    }
    porte = p;
    porte->stopBeforeUp();
    setAnimation(SOULEVE);
    return true;
}

Portable* Link::getPortable() {
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    BoundingBox bb;
    switch (direction) {
        case N : bb.setX(x + 7); bb.setY(y + 6); bb.setW(2); bb.setH(2); break;
        case S : bb.setX(x + 7); bb.setY(y + 24); bb.setW(2); bb.setH(2); break;
        case W : bb.setX(x - 2); bb.setY(y + 15); bb.setW(2); bb.setH(2); break;
        case E : bb.setX(x + 16); bb.setY(y + 15); bb.setW(2); bb.setH(2); break;
    }

    return map->pickUpObject(&bb);
}

void Link::pousse() {
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    BoundingBox bb;
    switch (direction) {
        case N : bb.setX(x + 7); bb.setY(y + 6); bb.setW(2); bb.setH(2); break;
        case S : bb.setX(x + 7); bb.setY(y + 24); bb.setW(2); bb.setH(2); break;
        case W : bb.setX(x - 2); bb.setY(y + 15); bb.setW(2); bb.setH(2); break;
        case E : bb.setX(x + 16); bb.setY(y + 15); bb.setW(2); bb.setH(2); break;
    }

    return map->pushObject(&bb, direction);
}

void Link::lacheObj() {
    if (porte != 0) {
        porte->lache(getDown());
        Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
        map->addObject(porte);
        porte = 0;
    }
}

void Link::lanceObj() {
    if (porte != 0) {
        porte->lance(getDown(), direction, elan);
        Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
        map->addObject(porte);
        porte = 0;
        AudioManager::getInstance()->playSound(TS_THROW);
    }
}

/*******
* DRAW *
*******/

void Link::drawShadow(int dstX, int dstY) {
    WindowManager::getInstance()->draw(imageEffets, 32, 38, 12, 6, dstX + 2, dstY + 18);
}

void Link::drawIdle(int dstX, int dstY) {
    if (lapin) WindowManager::getInstance()->draw(imageLink, 352 + direction * w, 120, w, h, dstX, dstY);
    else WindowManager::getInstance()->draw(imageLink, direction * w, 0, w, h, dstX, dstY);
}

void Link::drawDead(int dstX, int dstY) {
    if (!isOnWater() && !isOnGap()) {
        if (lapin) WindowManager::getInstance()->draw(imageLink, 416, 128, 24, 16, dstX - 4, dstY + 8);
        else WindowManager::getInstance()->draw(imageLink, 192, 200, 24, 16, dstX - 4, dstY + 8);
    }
}

void Link::drawWalk(int dstX, int dstY) {
    if (lapin) WindowManager::getInstance()->draw(imageLink, 352 + direction * w, 144 + h * ((anim / 2)%2), w, h, dstX, dstY);
    else WindowManager::getInstance()->draw(imageLink, direction * w, h * (anim + 1), w, h, dstX, dstY);
}

void Link::drawPush(int dstX, int dstY) {
    WindowManager::getInstance()->draw(imageLink, 352 + direction * w, h * (anim / 3), w, h, dstX, dstY);
}

void Link::drawPorte(int dstX, int dstY, int offsetX, int offsetY) {

    int dstX0 = dstX;

    int srcX = 272 + direction * w;
    int srcY = 48 + h * anim;
    int srcW = w;
    int srcH = h;

    if (direction == W) {
        dstX += 2;
        srcW = 24;
        if (anim == 2) {
            dstX--;
        }
    }
    if (direction == E) {
        dstX -= 10;
        srcX += 8;
        srcW = 24;
        if (anim == 2) {
            dstX++;
        }
    }

    WindowManager::getInstance()->draw(imageLink, srcX, srcY, srcW, srcH, dstX, dstY);

    if (porte != 0) {
        dstX0 += 8;
        dstY += 7;
        if (direction == N || direction == S) dstY++;
        if (anim % 2 && (direction == N || direction == S)) dstY++;
        if ((anim == 1 || anim == 3) && (direction == W || direction == E)) dstY++;
        if (anim == 2 && (direction == W || direction == E)) dstY+=2;
        porte->drawWhenCarried(dstX0 + offsetX, dstY + offsetY, offsetX, offsetY);
    }
}

void Link::drawThrow(int dstX, int dstY) {
    int srcX = 272 + direction * w;
    int srcY = 144;
    int srcW = w;
    int srcH = h;

    if (direction == W) {
        dstX += 2;
        srcW = 24;
    }
    if (direction == E) {
        dstX -= 10;
        srcX += 8;
        srcW = 24;
    }

    WindowManager::getInstance()->draw(imageLink, srcX, srcY, srcW, srcH, dstX, dstY);
}

void Link::drawSouleve(int dstX, int dstY) {

    int srcX = 272 + direction * w;
    int srcY = anim * h;
    int srcW = w;
    int srcH = h;

    if (direction == W) {
        dstX += 2;
        srcW = 24;
    }
    if (direction == E) {
        dstX -= 10;
        srcX += 8;
        srcW = 24;
    }

    WindowManager::getInstance()->draw(imageLink, srcX, srcY, srcW, srcH, dstX, dstY);
}

void Link::drawTouche(int dstX, int dstY) {

    int var = 0;
    switch (anim) {
        case 0 : var = 2; break;
        case 1 : var = 5; break;
        case 2 : var = 6; break;
        case 3 : var = 6; break;
        case 4 : var = 6; break;
        case 5 : var = 6; break;
        case 6 : var = 6; break;
        case 7 : var = 6; break;
        case 8 : var = 5; break;
        case 9 : var = 2; break;
        case 10 : var = 0; break;
    }

    if (lapin) WindowManager::getInstance()->draw(imageLink, 416 + direction * w, 144, w, h, dstX, dstY - var);
    else WindowManager::getInstance()->draw(imageLink, 64 + direction * w, 192, w, h, dstX, dstY - var);

    WindowManager::getInstance()->draw(imageEffets, 58, 34, 8, 4, dstX + 4, dstY + 20);

}

void Link::drawChute(int dstX, int dstY) {

    int srcX;
    int srcY;
    int srcW = 22;
    int srcH = 22;

    dstX -= 3; dstY += 5;

    srcX = 216 + srcW * anim;
    srcY = 194;

    WindowManager::getInstance()->draw(imageLink, srcX, srcY, srcW, srcH, dstX, dstY);

}

void Link::drawTrouveSimple(int dstX, int dstY) {
    WindowManager::getInstance()->draw(imageLink, 256, 48, 16, 24, dstX, dstY);
    ItemHelper::getInstance()->drawItem(trouve, dstX + 14, dstY - 6);
}

void Link::drawTrouveDouble(int dstX, int dstY) {
    WindowManager::getInstance()->draw(imageLink, 256, 24, 16, 24, dstX, dstY);
    ItemHelper::getInstance()->drawItem(trouve, dstX + 8, dstY - 6);
}

void Link::drawArc(int dstX, int dstY) {
    int tmp = (int)((anim+1)/2);

    if (direction==N) {
        WindowManager::getInstance()->draw(imageObjets, 16 * direction, 16 * tmp, 16, 16, dstX - 2 - 3 * tmp, dstY + 7 + 3 * tmp);
    }

    WindowManager::getInstance()->draw(imageLink, 128 + w * direction, 120 + h * anim, 16, 24, dstX, dstY);

    if (direction==N) {
        return;
    }

    switch (direction) {
        case S : dstX += 10 - 8 * tmp; dstY += 10 + 5 * tmp; break;
        case W : dstX -= 1 + 2 * tmp; dstY += 11 - 4 * tmp; break;
        case E : dstX += 4 + 8 * tmp; dstY += 11 - 4 * tmp; break;
        default : break;
    }

    WindowManager::getInstance()->draw(imageObjets, 16 * direction, 16 * tmp, 16, 16, dstX, dstY);
}

void Link::drawGrappin(int dstX, int dstY, int offsetX, int offsetY) {

    if (grappin != 0) {
        grappin->draw(offsetX, offsetY);
    }

    WindowManager::getInstance()->draw(imageLink, 128 + w * direction, 192, 16, 24, dstX, dstY);

    if (direction==N) {
        return;
    }

    int srcX = 56;
    int srcY = 74;
    int srcW = 8;
    int srcH = 4;
    switch (direction) {
        case S : dstX += 6; dstY += 17; srcW = 4; srcH = 8; break;
        case W : dstX -= 7; dstY += 14; srcY += 8; break;
        case E : dstX += 15; dstY += 14; srcY += 12; break;
        default : break;
    }

    WindowManager::getInstance()->draw(imageObjets, srcX, srcY, srcW, srcH, dstX, dstY);
}

void Link::drawLanterne(int dstX, int dstY, int offsetX, int offsetY) {

    if (flamme != 0) {
        flamme->draw(offsetX, offsetY);
    }

    WindowManager::getInstance()->draw(imageLink, 128 + w * direction, 192, 16, 24, dstX, dstY);
}

void Link::drawFlacon(int dstX, int dstY) {
    WindowManager::getInstance()->draw(imageLink, 256, 0, 16, 24, dstX, dstY);

    int flacon = inventory->getCurrentFlacon();

    int srcY = anim == 2 ? 0 : 8 * (flacon%4);
    int srcX = anim == 2 || flacon == 0 ? 96 : 96 + 8 * anim + 32 * (flacon/4);

    dstX += 4;
    dstY += 12;
    if (anim != 1) dstY++;

    WindowManager::getInstance()->draw(imageObjets, srcX, srcY, 8, 7, dstX, dstY);
}

void Link::drawOcarina(int dstX, int dstY) {
    WindowManager::getInstance()->draw(imageLink, 256, 0, 16, 24, dstX, dstY);
    WindowManager::getInstance()->draw(imageObjets, 104, 0, 8, 4, dstX + 4, dstY + 13);
}

void Link::drawBoomerang(int dstX, int dstY) {

    if (anim == 0) {
        WindowManager::getInstance()->draw(imageLink, 416 + w * direction, 0, 16, 24, dstX, dstY);

        int srcX = 64;
        int srcY = inventory->getCurrent() == BOOMERANG_FEE ? 10 : 0;
        switch (direction) {
            case N : dstX -= 6; break;
            case S : dstX += 13; srcX += 10; break;
            case W : dstX += 10; srcX += 10; break;
            case E : dstX -= 4; break;
            default : break;
        }
        WindowManager::getInstance()->draw(imageObjets, srcX, srcY, 10, 10, dstX, dstY + 1);
    } else {
        WindowManager::getInstance()->draw(imageLink, 128 + w * direction, 192, 16, 24, dstX, dstY);
    }
}

void Link::drawBaguette(int dstX, int dstY) {

    WindowManager::getInstance()->draw(imageLink, 192 + w * direction, h * anim, w, h, dstX, dstY);

    int srcX = 14 * direction;
    int srcY = 32 + anim * 14;
    if (animation == TIR_BAGUETTE_GLACE) srcX += 56;
    if (animation == TIR_MARTEAU) srcY += 42;
    switch (direction) {
        case N :
            dstX -= 2; dstY -= 5;
            if (anim == 1) dstY += 3;
            if (anim == 2) dstY += 7;
            break;
        case S :
            dstX += 10; dstY -= 3;
            if (anim == 1) dstY += 13;
            if (anim == 2) {dstX--; dstY += 23;}
            break;
        case W :
            dstY --;
            if (anim == 1) {dstX -= 8; dstY += 5;}
            if (anim == 2) {dstX -= 10; dstY += 15;}
            break;
        case E :
            dstX += 8; dstY --;
            if (anim == 1) {dstX += 5; dstY += 5;}
            if (anim == 2) {dstX += 4; dstY +=15;}
            break;
    }
    WindowManager::getInstance()->draw(imageObjets, srcX, srcY, 14, 14, dstX, dstY);
}

void Link::drawCanne(int dstX, int dstY) {
    WindowManager::getInstance()->draw(imageLink, 416 + w * direction, 24 + h * anim, w, h, dstX, dstY);

    int srcX = 0;
    int srcY = 116;
    int srcW = 14;
    int srcH = 14;
    if (animation == TIR_CANNE_BYRNA) srcY += 14;
    switch (direction) {
        case N :
            srcW = 8;
            if (anim == 0) {dstX --; dstY += 6;}
            if (anim == 1) {dstX -= 3; dstY--;}
            if (anim == 2) {dstX -= 3; dstY -= 3; srcX = 8; srcH = 10;}
            break;
        case S :
            srcW = 8;
            if (anim == 0) {dstX += 9; dstY -= 7; srcX = 16; srcH = 10;}
            if (anim == 1) {dstX += 12; dstY--;}
            if (anim == 2) {dstX += 8; dstY += 18; srcX = 24; srcH = 12;}
            break;
        case W :
            if (anim == 0) {dstX += 11; dstY += 3; srcX = 32; srcW = 12; srcH = 12;}
            if (anim == 1) {dstX += 2; dstY -= 2; srcW = 8;}
            if (anim == 2) {dstX -= 13; dstY += 12; srcX = 44; srcW = 14; srcH = 8;}
            break;
        case E :
            if (anim == 0) {dstX -= 7; dstY += 3; srcX = 66; srcW = 12; srcH = 12;}
            if (anim == 1) {dstX += 6; dstY -= 2; srcX = 58; srcW = 8;}
            if (anim == 2) {dstX += 15; dstY += 12; srcX = 78; srcW = 14; srcH = 8;}
            break;
    }
    WindowManager::getInstance()->draw(imageObjets, srcX, srcY, srcW, srcH, dstX, dstY);
}

void Link::drawFlotte(int dstX, int dstY) {

    int srcX = 192 + 16 * direction;
    int srcY = 72 + h * anim;
    int srcW = 16;

    if (direction == W) {
        dstX--;
        srcW=24;
    }
    if (direction == E) {
        dstX -= 7;
        srcW = 24;
        srcX += 8;
    }

    if (animation == NAGE) srcY += h * 2;
    if (srcY == 192) srcY -= 48;

    WindowManager::getInstance()->draw(imageLink, srcX, srcY, srcW, h, dstX, dstY);
}

void Link::drawCoule(int dstX, int dstY) {
    WindowManager::getInstance()->draw(imageEffets, 16 * (anim % 2), 38, 16, 15, dstX, dstY + 8);
}

void Link::drawEpee(int dstX, int dstY) {

    int srcX = 64 + 16 * direction;
    int srcY = h * anim;

    if (direction == W) {
        dstX += 2;
        switch (anim) {
            case 1 : dstX -= 2; break;
            case 2 : dstX -= 3; break;
            case 3 : dstX -= 2; break;
            case 4 : dstX --; break;
            default : break;
        }
    }
    if (direction == E) {
        dstX -= 2;
        switch (anim) {
            case 1 : dstX += 2; break;
            case 2 : dstX += 3; break;
            case 3 : dstX += 2; break;
            case 4 : dstX ++; break;
            default : break;
        }
    }

    WindowManager::getInstance()->draw(imageLink, srcX, srcY, 16, 24, dstX, dstY);
}

void Link::drawCharge(int dstX, int dstY) {

    int tmp = anim / 2;
    int srcX = 128 + 16 * direction;
    int srcY = 24 * tmp;

    WindowManager::getInstance()->draw(imageLink, srcX, srcY, 16, 24, dstX, dstY);
}

void Link::drawSpin(int dstX, int dstY) {

    int srcX;
    int srcY;

    switch (direction) {
        case N :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { srcX = 64; srcY = 120;}
            if (anim>=1 && anim<=4) { srcX = 64; srcY = 144;}
            if (anim>=6 && anim<=8) { srcX = 64; srcY = 168;}
            if (anim>=9 && anim<=12) { srcX = 80; srcY = 120;}
            if (anim>=13 && anim<=16) { srcX = 80; srcY = 144;}
            if (anim>=17 && anim<=20) { srcX = 80; srcY = 168;}
            break;
        case S :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { srcX = 96; srcY = 120;}
            if (anim>=1 && anim<=4) { srcX = 96; srcY = 144;}
            if (anim>=6 && anim<=8) { srcX = 80; srcY = 144;}
            if (anim>=9 && anim<=12) { srcX = 80; srcY = 168;}
            if (anim>=13 && anim<=16) { srcX = 64; srcY = 168;}
            if (anim>=17 && anim<=20) { srcX = 80; srcY = 120;}
            break;
        case W :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { srcX = 96; srcY = 168;}
            if (anim>=1 && anim<=4) { srcX = 112; srcY = 120;}
            if (anim>=6 && anim<=8) { srcX = 80; srcY = 168;}
            if (anim>=9 && anim<=12) { srcX = 64; srcY = 168;}
            if (anim>=13 && anim<=16) { srcX = 80; srcY = 120;}
            if (anim>=17 && anim<=20) { srcX = 80; srcY = 144;}
            break;
        case E :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { srcX = 112; srcY = 144;}
            if (anim>=1 && anim<=4) { srcX = 112; srcY = 168;}
            if (anim>=6 && anim<=8) { srcX = 80; srcY = 120;}
            if (anim>=9 && anim<=12) { srcX = 80; srcY = 144;}
            if (anim>=13 && anim<=16) { srcX = 80; srcY = 168;}
            if (anim>=17 && anim<=20) { srcX = 64; srcY = 168;}
            break;
    }

    if (anim > 26) drawIdle(dstX, dstY);
    else WindowManager::getInstance()->draw(imageLink, srcX, srcY, w, h, dstX, dstY);

}

void Link::drawByrna(int dstX, int dstY, int offsetX, int offsetY) {
    int srcX = 0;
    int srcY = 0;
    int srcW = 0;
    int srcH = 0;
    switch (animByrna) {
        case 0 :
            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 7; dstY -= 8;}
            if (dirByrna==S) { dstX += 6; dstY += 30;}
            if (dirByrna==W) { dstX -= 12; dstY += 17;}
            if (dirByrna==E) { dstX += 23; dstY += 15;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 1 :
            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 9; dstY -= 6; }
            if (dirByrna==S) { dstX += 8; dstY += 32; }
            if (dirByrna==W) { dstX -= 10; dstY += 19; }
            if (dirByrna==E) { dstX += 25; dstY += 17; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 0; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 3; dstY += 35; }
            if (dirByrna==S) { dstX -= 5; dstY -= 44; }
            if (dirByrna==W) { dstX += 32; dstY -= 13; }
            if (dirByrna==E) { dstX -= 38; dstY -= 1; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 2 :
            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 9; dstY -= 6;}
            if (dirByrna==S) { dstX += 8; dstY += 32;}
            if (dirByrna==W) { dstX -= 10; dstY += 19;}
            if (dirByrna==E) { dstX += 25; dstY += 17;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 8; srcY = 0; srcW = 14; srcH = 14;
            if (dirByrna==N) { dstX -= 6; dstY += 32; }
            if (dirByrna==S) { dstX -= 8; dstY -= 47; }
            if (dirByrna==W) { dstX += 29; dstY -= 16; }
            if (dirByrna==E) { dstX -= 41; dstY -= 4; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 3 :
            srcX = 8; srcY = 0; srcW = 14; srcH = 14;
            if (dirByrna==N) { dstX += 3; dstY += 26; }
            if (dirByrna==S) { dstY -= 15; }
            if (dirByrna==W) { dstX += 19; dstY += 3; }
            if (dirByrna==E) { dstX -= 15; dstY += 13; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 4 :
        case 5 :
            srcX = 22; srcY = 0; srcW = 14; srcH = 14;
            if (dirByrna==N) { dstX += 3; dstY += 26; }
            if (dirByrna==S) { dstY -= 15; }
            if (dirByrna==W) { dstX += 19; dstY += 3; }
            if (dirByrna==E) { dstX -= 15; dstY += 13; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 6 :
            srcX = 36; srcY = 0; srcW = 16; srcH = 16;
            if (dirByrna==N) { dstX += 2; dstY += 25; }
            if (dirByrna==S) { dstX -= 1; dstY -= 15; }
            if (dirByrna==W) { dstX += 19; dstY += 2; }
            if (dirByrna==E) { dstX -= 17; dstY += 12; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 7 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 5; dstY += 27; }
            if (dirByrna==S) { dstX += 17; dstY -= 6; }
            if (dirByrna==W) { dstX += 20; dstY += 19; }
            if (dirByrna==E) { dstX -= 13; dstY -= 1; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 8 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 11; dstY += 22; }
            if (dirByrna==S) { dstX += 22; }
            if (dirByrna==W) { dstX += 14; dstY += 25; }
            if (dirByrna==E) { dstX -= 8; dstY -= 7; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 7; dstY += 6;}
            if (dirByrna==S) { dstX -= 7; dstY -= 7;}
            if (dirByrna==W) { dstX += 7; dstY -= 7;}
            if (dirByrna==E) { dstX -= 5; dstY += 8;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 9 :
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 14; dstY += 15; }
            if (dirByrna==S) { dstX += 23; dstY += 7; }
            if (dirByrna==W) { dstX += 8; dstY += 28; }
            if (dirByrna==E) { dstX -= 2; dstY -= 10; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 5; dstY += 9; }
            if (dirByrna==S) { dstX -= 3; dstY -= 8; }
            if (dirByrna==W) { dstX += 9; dstY -= 4; }
            if (dirByrna==E) { dstX -= 8; dstY += 6; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 9; dstY += 6;}
            if (dirByrna==S) { dstX -= 7; dstY -= 6;}
            if (dirByrna==W) { dstX += 6; dstY -= 6;}
            if (dirByrna==E) { dstX -= 4; dstY += 9;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 10 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 15; dstY += 7; }
            if (dirByrna==S) { dstX += 22; dstY += 15; }
            if (dirByrna==W) { dstY += 28; }
            if (dirByrna==E) { dstX += 6; dstY -= 11; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 11; dstY += 11;}
            if (dirByrna==S) { dstX += 1; dstY -= 9;}
            if (dirByrna==W) dstX += 10;
            if (dirByrna==E) { dstX -= 10; dstY += 3;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 6; dstY += 8; }
            if (dirByrna==S) { dstX -= 4; dstY -= 7; }
            if (dirByrna==W) { dstX += 9; dstY -= 4; }
            if (dirByrna==E) { dstX -= 6; dstY += 6; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 11; dstY += 6;}
            if (dirByrna==S) { dstX -= 5; dstY -= 4;}
            if (dirByrna==W) { dstX += 7; dstY -= 7;}
            if (dirByrna==E) { dstX -= 3; dstY += 10;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 11 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 13; }
            if (dirByrna==S) { dstX += 18; dstY += 22; }
            if (dirByrna==W) { dstX -= 7; dstY += 25; }
            if (dirByrna==E) { dstX += 13; dstY -= 9; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 2; dstY += 10;}
            if (dirByrna==S) { dstX += 5; dstY -= 8;}
            if (dirByrna==W) { dstX += 9; dstY += 4;}
            if (dirByrna==E) { dstX -= 9; dstY -= 1;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 3; dstY += 10;}
            if (dirByrna==S) { dstY -= 9;}
            if (dirByrna==W) { dstX += 11; dstY -= 1;}
            if (dirByrna==E) { dstX -= 8; dstY += 3;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 9; dstY += 9; }
            if (dirByrna==S) { dstX -= 3; dstY -= 7; }
            if (dirByrna==W) { dstX += 9; dstY -= 4; }
            if (dirByrna==E) { dstX -= 6; dstY += 8; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 12 :
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 8; dstY -= 6; }
            if (dirByrna==S) { dstX += 12; dstY += 27; }
            if (dirByrna==W) { dstX -= 13; dstY += 19; }
            if (dirByrna==E) { dstX += 19; dstY -= 4; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 5; dstY += 8;}
            if (dirByrna==S) { dstX += 8; dstY -= 6;}
            if (dirByrna==W) { dstX += 8; dstY += 8;}
            if (dirByrna==E) { dstX -= 7; dstY += 4;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX -= 1; dstY += 11;}
            if (dirByrna==S) { dstX += 4; dstY -= 8;}
            if (dirByrna==W) { dstX += 10; dstY += 3;}
            if (dirByrna==E) { dstX -= 9; dstY -= 1;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 5; dstY += 10;}
            if (dirByrna==S) { dstY -= 9;}
            if (dirByrna==W) { dstX += 11; dstY -= 1;}
            if (dirByrna==E) { dstX -= 7; dstY += 4;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 13 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            dirByrna=dirByrna;
            if (dirByrna==N) { dstX -= 2; dstY -= 10;}
            if (dirByrna==S) { dstX += 6; dstY += 29;}
            if (dirByrna==W) { dstX -= 15; dstY += 12;}
            if (dirByrna==E) { dstX += 22; dstY += 3;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 8; dstY += 6; }
            if (dirByrna==S) { dstX += 9; dstY -= 2; }
            if (dirByrna==W) { dstX += 5; dstY += 10; }
            if (dirByrna==E) { dstX -= 5; dstY -= 7; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX -= 4; dstY += 9;}
            if (dirByrna==S) { dstX += 8; dstY -= 7;}
            if (dirByrna==W) { dstX += 8; dstY += 6;}
            if (dirByrna==E) { dstX -= 7; dstY -= 4;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 1; dstY += 11;}
            if (dirByrna==S) { dstX += 4; dstY -= 8;}
            if (dirByrna==W) { dstX += 12; dstY += 3;}
            if (dirByrna==E) dstX -= 8;
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 14 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            dirByrna=dirByrna;
            if (dirByrna==N) { dstX += 6; dstY -= 11;}
            if (dirByrna==S) { dstX -= 2; dstY += 28;}
            if (dirByrna==W) { dstX -= 15; dstY += 4;}
            if (dirByrna==E) { dstX += 23; dstY += 11;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 10; dstY += 3;}
            if (dirByrna==S) { dstX += 10; dstY += 2;}
            if (dirByrna==W) { dstX += 1; dstY += 11;}
            if (dirByrna==E) { dstX -= 2; dstY -= 9;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX -= 6; dstY += 6; }
            if (dirByrna==S) { dstX += 10; dstY -= 4; }
            if (dirByrna==W) { dstX += 6; dstY += 9; }
            if (dirByrna==E) { dstX -= 4; dstY += 6; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX -= 3; dstY += 10;}
            if (dirByrna==S) { dstX += 7; dstY -= 6;}
            if (dirByrna==W) { dstX += 10; dstY += 6;}
            if (dirByrna==E) { dstX -= 7; dstY -= 3;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 15 :
            dirByrna=dirByrna;
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX += 13; dstY -= 9;}
            if (dirByrna==S) { dstX -= 9; dstY += 24;}
            if (dirByrna==W) { dstX -= 12; dstY -= 3;}
            if (dirByrna==E) { dstX += 21; dstY += 18;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 9; dstY -= 1;}
            if (dirByrna==S) { dstX += 9; dstY += 6;}
            if (dirByrna==W) { dstX -= 3; dstY += 10;}
            if (dirByrna==E) { dstX += 2; dstY -= 8;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX -= 8; dstY += 3;}
            if (dirByrna==S) dstX += 11;
            if (dirByrna==W) { dstX += 3; dstY += 11;}
            if (dirByrna==E) { dstX -= 1; dstY -= 8;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX -= 6; dstY += 8; }
            if (dirByrna==S) { dstX += 10; dstY -= 4; }
            if (dirByrna==W) { dstX += 7; dstY += 8; }
            if (dirByrna==E) { dstX -= 5; dstY -= 6; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 18; dstY += 29;}
            if (dirByrna==S) { dstX -= 17; dstY -= 36;}
            if (dirByrna==W) { dstX += 30; dstY -= 19;}
            if (dirByrna==E) { dstX -= 34; dstY += 19;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 16 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            dirByrna=dirByrna;
            if (dirByrna==N) { dstX += 19; dstY -= 4;}
            if (dirByrna==S) { dstX -= 14; dstY += 18;}
            if (dirByrna==W) { dstX -= 6; dstY -= 9;}
            if (dirByrna==E) { dstX += 16; dstY += 24;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 7; dstY -= 4;}
            if (dirByrna==S) { dstX += 7; dstY += 9;}
            if (dirByrna==W) { dstX -= 7; dstY += 9;}
            if (dirByrna==E) { dstX += 5; dstY -= 6;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX -= 9; dstY -= 1;}
            if (dirByrna==S) { dstX += 10; dstY += 4;}
            if (dirByrna==W) { dstX -= 1; dstY += 10;}
            if (dirByrna==E) { dstX += 3; dstY -= 9;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX -= 7; dstY += 4;}
            if (dirByrna==S) { dstX += 12; dstY -= 1;}
            if (dirByrna==W) { dstX += 4; dstY += 10;}
            if (dirByrna==E) { dstX -= 1; dstY -= 8;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 13; dstY += 36;}
            if (dirByrna==S) { dstX -= 10; dstY -= 39;}
            if (dirByrna==W) { dstX += 36; dstY -= 12;}
            if (dirByrna==E) { dstX -= 39; dstY += 14;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 17 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            dirByrna=dirByrna;
            if (dirByrna==N) { dstX += 22; dstY += 3;}
            if (dirByrna==S) { dstX -= 15; dstY += 11;}
            if (dirByrna==W) dstY -= 11;
            if (dirByrna==E) { dstX += 10; dstY += 28;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 5; dstY -= 7;}
            if (dirByrna==S) { dstX += 3; dstY += 10;}
            if (dirByrna==W) { dstX -= 9; dstY += 6;}
            if (dirByrna==E) { dstX += 8; dstY -= 4;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX -= 7; dstY -= 4;}
            if (dirByrna==S) { dstX += 9; dstY += 8;}
            if (dirByrna==W) { dstX -= 4; dstY += 8;}
            if (dirByrna==E) { dstX += 6; dstY -= 7;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX -= 8; }
            if (dirByrna==S) { dstX += 11; dstY += 3; }
            if (dirByrna==W) { dstY += 11; }
            if (dirByrna==E) { dstX += 3; dstY -= 9; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            if (dirByrna==N) { dstX += 8; dstY += 40;}
            if (dirByrna==S) dstY -= 40;
            if (dirByrna==W) { dstX += 40; dstY -= 4;}
            if (dirByrna==E) { dstX -= 40; dstY += 8;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 18 :
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            dirByrna=dirByrna;
            if (dirByrna==N) { dstX += 23; dstY += 11;}
            if (dirByrna==S) { dstX -= 14; dstY += 3;}
            if (dirByrna==W) { dstX += 8; dstY -= 11;}
            if (dirByrna==E) { dstX += 2; dstY += 29;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 2; dstY -= 9;}
            if (dirByrna==S) { dstX -= 1; dstY += 11;}
            if (dirByrna==W) { dstX -= 10; dstY += 2;}
            if (dirByrna==E) { dstX += 10; dstY -= 1;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX -= 4; dstY -= 6; }
            if (dirByrna==S) { dstX += 6; dstY += 10; }
            if (dirByrna==W) { dstX -= 7; dstY += 6; }
            if (dirByrna==E) { dstX += 8; dstY -= 4; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX -= 7; dstY -= 4;}
            if (dirByrna==S) { dstX += 9; dstY += 6;}
            if (dirByrna==W) { dstX -= 3; dstY += 9;}
            if (dirByrna==E) { dstX += 7; dstY -= 8;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 19 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            dirByrna=dirByrna;
            if (dirByrna==N) { dstX += 21; dstY += 18;}
            if (dirByrna==S) { dstX -= 10; dstY -= 4;}
            if (dirByrna==W) { dstX += 15; dstY -= 8;}
            if (dirByrna==E) { dstX -= 5; dstY += 27;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 2; dstY -= 8; }
            if (dirByrna==S) { dstX -= 5; dstY += 10; }
            if (dirByrna==W) { dstX -= 9; dstY -= 2; }
            if (dirByrna==E) { dstX += 9; dstY += 3; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX -= 1; dstY -= 8;}
            if (dirByrna==S) { dstX += 2; dstY += 11;}
            if (dirByrna==W) { dstX -= 9; dstY += 3;}
            if (dirByrna==E) { dstX += 10; dstY -= 1;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX -= 5; dstY -= 7;}
            if (dirByrna==S) { dstX += 7; dstY += 9;}
            if (dirByrna==W) { dstX -= 5; dstY += 6;}
            if (dirByrna==E) { dstX += 10; dstY -= 6;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 27; dstY += 30;}
            if (dirByrna==S) { dstX += 26; dstY -= 29;}
            if (dirByrna==W) { dstX += 26; dstY += 25;}
            if (dirByrna==E) { dstX -= 35; dstY -= 26;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 20 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX += 16; dstY += 24; }
            if (dirByrna==S) { dstX -= 4; dstY -= 9; }
            if (dirByrna==W) { dstX += 21; dstY -= 2; }
            if (dirByrna==E) { dstX -= 11; dstY += 22; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 5; dstY -= 6;}
            if (dirByrna==S) { dstX -= 8; dstY += 8;}
            if (dirByrna==W) { dstX -= 8; dstY -= 6;}
            if (dirByrna==E) { dstX += 7; dstY += 6;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 3; dstY -= 9;}
            if (dirByrna==S) { dstX -= 2; dstY += 10;}
            if (dirByrna==W) { dstX -= 8; dstY -= 1;}
            if (dirByrna==E) { dstX += 11; dstY += 3;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX -= 1; dstY -= 8;}
            if (dirByrna==S) { dstX += 4; dstY += 11;}
            if (dirByrna==W) { dstX -= 7; dstY += 3;}
            if (dirByrna==E) { dstX += 11; dstY -= 2;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX -= 32; dstY += 25;}
            if (dirByrna==S) { dstX += 31; dstY -= 22;}
            if (dirByrna==W) { dstX += 21; dstY += 31;}
            if (dirByrna==E) { dstX -= 28; dstY -= 31;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 21 :
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX += 10; dstY += 28; }
            if (dirByrna==S) { dstX += 2; dstY -= 12; }
            if (dirByrna==W) { dstX += 23; dstY += 4; }
            if (dirByrna==E) { dstX -= 14; dstY += 14; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 8; dstY -= 4; }
            if (dirByrna==S) { dstX -= 9; dstY += 4; }
            if (dirByrna==W) { dstX -= 5; dstY -= 8; }
            if (dirByrna==E) { dstX += 5; dstY += 9; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 6; dstY -= 7;}
            if (dirByrna==S) { dstX -= 6; dstY += 9;}
            if (dirByrna==W) { dstX -= 6; dstY -= 4;}
            if (dirByrna==E) { dstX += 9; dstY += 6;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 3; dstY -= 9;}
            if (dirByrna==S) dstY += 10;
            if (dirByrna==W) { dstX -= 8; dstY -= 1;}
            if (dirByrna==E) { dstX += 12; dstY += 2;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            if (dirByrna==N) { dstX -= 33; dstY += 19;}
            if (dirByrna==S) { dstX += 35; dstY -= 13;}
            if (dirByrna==W) { dstX += 16; dstY += 34;}
            if (dirByrna==E) { dstX -= 19; dstY -= 33;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 22 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX += 2; dstY += 29; }
            if (dirByrna==S) { dstX += 10; dstY -= 11; }
            if (dirByrna==W) { dstX += 23; dstY += 12; }
            if (dirByrna==E) { dstX -= 15; dstY += 6; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 10; dstY -= 1;}
            if (dirByrna==S) dstX -= 10;
            if (dirByrna==W) { dstX -= 1; dstY -= 9;}
            if (dirByrna==E) { dstX += 2; dstY += 11;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 8; dstY -= 4; }
            if (dirByrna==S) { dstX -= 8; dstY += 6; }
            if (dirByrna==W) { dstX -= 4; dstY -= 7; }
            if (dirByrna==E) { dstX += 6; dstY += 8; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 7; dstY -= 8;}
            if (dirByrna==S) { dstX -= 3; dstY += 8;}
            if (dirByrna==W) { dstX -= 6; dstY -= 4;}
            if (dirByrna==E) { dstX += 11; dstY += 6;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 23 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 5; dstY += 27; }
            if (dirByrna==S) { dstX += 17; dstY -= 7; }
            if (dirByrna==W) { dstX += 20; dstY += 19; }
            if (dirByrna==E) { dstX -= 13; dstY -= 1; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 9; dstY += 3;}
            if (dirByrna==S) { dstX -= 9; dstY -= 4;}
            if (dirByrna==W) { dstX += 3; dstY -= 8;}
            if (dirByrna==E) { dstX -= 2; dstY += 10;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 10; dstY -= 1;}
            if (dirByrna==S) { dstX -= 9; dstY += 2;}
            if (dirByrna==W) { dstX -= 1; dstY -= 9;}
            if (dirByrna==E) { dstX += 3; dstY += 10;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 10; dstY -= 6; }
            if (dirByrna==S) { dstX -= 6; dstY += 6; }
            if (dirByrna==W) { dstX -= 3; dstY -= 6; }
            if (dirByrna==E) { dstX += 9; dstY += 9; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 40; dstY -= 13;}
            if (dirByrna==S) { dstX += 31; dstY += 16;}
            if (dirByrna==W) { dstX -= 18; dstY += 33;}
            if (dirByrna==E) { dstX += 8; dstY -= 39;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 24 :
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 11; dstY += 22; }
            if (dirByrna==S) { dstX += 22; dstY -= 1; }
            if (dirByrna==W) { dstX += 14; dstY += 25; }
            if (dirByrna==E) { dstX -= 8; dstY -= 7; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 7; dstY += 6;}
            if (dirByrna==S) { dstX -= 7; dstY -= 7;}
            if (dirByrna==W) { dstX += 7; dstY -= 7;}
            if (dirByrna==E) { dstX -= 5; dstY += 8;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 11; dstY += 3;}
            if (dirByrna==S) { dstX -= 8; dstY -= 2;}
            if (dirByrna==W) { dstX += 3; dstY -= 8;}
            if (dirByrna==E) { dstX -= 1; dstY += 11;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 11; dstY -= 2;}
            if (dirByrna==S) { dstX -= 8; dstY += 3;}
            if (dirByrna==W) dstY -= 8;
            if (dirByrna==E) { dstX += 5; dstY += 10;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX -= 33; dstY -= 18;}
            if (dirByrna==S) { dstX += 26; dstY += 21;}
            if (dirByrna==W) { dstX -= 22; dstY += 28;}
            if (dirByrna==E) { dstX += 15; dstY -= 32;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 25 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 15; dstY += 15; }
            if (dirByrna==S) { dstX += 24; dstY += 6; }
            if (dirByrna==W) { dstX += 7; dstY += 28; }
            if (dirByrna==E) { dstX -= 6; dstY -= 9; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 5; dstY += 9; }
            if (dirByrna==S) { dstX -= 3; dstY -= 8; }
            if (dirByrna==W) { dstX += 9; dstY -= 4; }
            if (dirByrna==E) { dstX -= 7; dstY += 9; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 9; dstY += 6;}
            if (dirByrna==S) { dstX -= 7; dstY -= 6;}
            if (dirByrna==W) { dstX += 6; dstY -= 6;}
            if (dirByrna==E) { dstX -= 1; dstY += 10;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 12; dstY += 2;}
            if (dirByrna==S) { dstX -= 7; dstY -= 1;}
            if (dirByrna==W) { dstX += 4; dstY -= 9;}
            if (dirByrna==E) { dstX += 4; dstY += 10;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            if (dirByrna==N) { dstX -= 24; dstY -= 20;}
            if (dirByrna==S) { dstX += 20; dstY += 24;}
            if (dirByrna==W) { dstX -= 22; dstY += 22;}
            if (dirByrna==E) { dstX += 18; dstY -= 28;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 26 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dirByrna==N) { dstX -= 16; dstY += 7; }
            if (dirByrna==S) { dstX += 23; dstY += 14; }
            if (dirByrna==W) { dstX -= 1; dstY += 28; }
            if (dirByrna==E) { dstX += 7; dstY -= 12; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX += 2; dstY += 11;}
            if (dirByrna==S) { dstX += 1; dstY -= 9;}
            if (dirByrna==W) dstX += 10;
            if (dirByrna==E) { dstX -= 10; dstY += 3;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 6; dstY += 8; }
            if (dirByrna==S) { dstX -= 4; dstY -= 8; }
            if (dirByrna==W) { dstX += 9; dstY -= 4; }
            if (dirByrna==E) { dstX -= 6; dstY += 6; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 11; dstY += 6;}
            if (dirByrna==S) { dstX -= 5; dstY -= 4;}
            if (dirByrna==W) { dstX += 7; dstY -= 7;}
            if (dirByrna==E) { dstX -= 3; dstY += 10;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 27 :
            srcX = 10; srcY = 14; srcW = 8; srcH += 8;
            if (dirByrna==N) { dstX -= 14; }
            if (dirByrna==S) { dstX += 19; dstY += 22; }
            if (dirByrna==W) { dstX -= 8; dstY += 25; }
            if (dirByrna==E) { dstX += 14; dstY -= 10; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 2; dstY += 10;}
            if (dirByrna==S) { dstX += 5; dstY -= 8;}
            if (dirByrna==W) { dstX += 9; dstY += 4;}
            if (dirByrna==E) { dstX -= 9; dstY -= 1;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dirByrna==N) { dstX += 3; dstY += 10;}
            if (dirByrna==S) dstY -= 9;
            if (dirByrna==W) { dstX += 11; dstY -= 1;}
            if (dirByrna==E) { dstX -= 8; dstY += 3;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dirByrna==N) { dstX += 9; dstY += 9; }
            if (dirByrna==S) { dstX -= 3; dstY -= 7; }
            if (dirByrna==W) { dstX += 9; dstY -= 4; }
            if (dirByrna==E) { dstX -= 6; dstY += 8; }
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dirByrna==N) { dstX -= 5; dstY -= 34;}
            if (dirByrna==S) { dstX -= 8; dstY += 30;}
            if (dirByrna==W) { dstX -= 35; dstY -= 4;}
            if (dirByrna==E) { dstX += 29; dstY += 4;}
            WindowManager::getInstance()->draw(imageEffets, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
    }
}

void Link::hitByrna() {
    // light
    int i = 0;
    int j = 0;
    int force = 2;
    Direction dirByrna2 = dirByrna;
    switch (animByrna) {
        case 0 :
            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 7; j -= 8;}
            if (dirByrna==S) { i += 6; j += 30;}
            if (dirByrna==W) { i -= 12; j += 17;}
            if (dirByrna==E) { i += 23; j += 15;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 1 :
            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 9; j -= 6; dirByrna2=N;}
            if (dirByrna==S) { i += 8; j += 32; dirByrna2=S;}
            if (dirByrna==W) { i -= 10; j += 19; dirByrna2=W;}
            if (dirByrna==E) { i += 25; j += 17; dirByrna2=E;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 3; j += 35; dirByrna2=S;}
            if (dirByrna==S) { i -= 5; j -= 44; dirByrna2=N;}
            if (dirByrna==W) { i += 32; j -= 13; dirByrna2=E;}
            if (dirByrna==E) { i -= 38; j -= 1; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 2 :
            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 9; j -= 6;}
            if (dirByrna==S) { i += 8; j += 32;}
            if (dirByrna==W) { i -= 10; j += 19;}
            if (dirByrna==E) { i += 25; j += 17;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(14); box.setH(14);
            if (dirByrna==N) { i -= 6; j += 32; dirByrna2=S;}
            if (dirByrna==S) { i -= 8; j -= 47; dirByrna2=N;}
            if (dirByrna==W) { i += 29; j -= 16; dirByrna2=E;}
            if (dirByrna==E) { i -= 41; j -= 4; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 3 :
            box.setW(14); box.setH(14);
            if (dirByrna==N) { i += 3; j += 26; dirByrna2=S;}
            if (dirByrna==S) { j -= 15; dirByrna2=N;}
            if (dirByrna==W) { i += 19; j += 3; dirByrna2=E;}
            if (dirByrna==E) { i -= 15; j += 13; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 4 :
        case 5 :
            box.setW(14); box.setH(14);
            if (dirByrna==N) { i += 3; j += 26; dirByrna2=S;}
            if (dirByrna==S) { j -= 15; dirByrna2=N;}
            if (dirByrna==W) { i += 19; j += 3; dirByrna2=E;}
            if (dirByrna==E) { i -= 15; j += 13; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 6 :
            box.setW(16); box.setH(16);
            if (dirByrna==N) { i += 2; j += 25; dirByrna2=S;}
            if (dirByrna==S) { i -= 1; j -= 15; dirByrna2=N;}
            if (dirByrna==W) { i += 19; j += 2; dirByrna2=E;}
            if (dirByrna==E) { i -= 17; j += 12; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 7 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 5; j += 27; dirByrna2=S;}
            if (dirByrna==S) { i += 17; j -= 6; dirByrna2=N;}
            if (dirByrna==W) { i += 20; j += 19; dirByrna2=E;}
            if (dirByrna==E) { i -= 13; j -= 1; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 8 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 11; j += 22; dirByrna2=S;}
            if (dirByrna==S) { i += 22; dirByrna2=N;}
            if (dirByrna==W) { i += 14; j += 25; dirByrna2=E;}
            if (dirByrna==E) { i -= 8; j -= 7; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 7; j += 6;}
            if (dirByrna==S) { i -= 7; j -= 7;}
            if (dirByrna==W) { i += 7; j -= 7;}
            if (dirByrna==E) { i -= 5; j += 8;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 9 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 14; j += 15; dirByrna2=W;}
            if (dirByrna==S) { i += 23; j += 7; dirByrna2=E;}
            if (dirByrna==W) { i += 8; j += 28; dirByrna2=S;}
            if (dirByrna==E) { i -= 2; j -= 10; dirByrna2=N;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 5; j += 9; dirByrna2=S;}
            if (dirByrna==S) { i -= 3; j -= 8; dirByrna2=N;}
            if (dirByrna==W) { i += 9; j -= 4; dirByrna2=E;}
            if (dirByrna==E) { i -= 8; j += 6; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 9; j += 6;}
            if (dirByrna==S) { i -= 7; j -= 6;}
            if (dirByrna==W) { i += 6; j -= 6;}
            if (dirByrna==E) { i -= 4; j += 9;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 10 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 15; j += 7; dirByrna2=W;}
            if (dirByrna==S) { i += 22; j += 15; dirByrna2=E;}
            if (dirByrna==W) { j += 28; dirByrna2=S;}
            if (dirByrna==E) { i += 6; j -= 11; dirByrna2=N;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 11; j += 11;}
            if (dirByrna==S) { i += 1; j -= 9;}
            if (dirByrna==W) i += 10;
            if (dirByrna==E) { i -= 10; j += 3;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 6; j += 8; dirByrna2=S;}
            if (dirByrna==S) { i -= 4; j -= 7; dirByrna2=N;}
            if (dirByrna==W) { i += 9; j -= 4; dirByrna2=E;}
            if (dirByrna==E) { i -= 6; j += 6; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 11; j += 6;}
            if (dirByrna==S) { i -= 5; j -= 4;}
            if (dirByrna==W) { i += 7; j -= 7;}
            if (dirByrna==E) { i -= 3; j += 10;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 11 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 13; dirByrna2=W;}
            if (dirByrna==S) { i += 18; j += 22; dirByrna2=E;}
            if (dirByrna==W) { i -= 7; j += 25; dirByrna2=S;}
            if (dirByrna==E) { i += 13; j -= 9; dirByrna2=N;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i -= 2; j += 10;}
            if (dirByrna==S) { i += 5; j -= 8;}
            if (dirByrna==W) { i += 9; j += 4;}
            if (dirByrna==E) { i -= 9; j -= 1;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 3; j += 10;}
            if (dirByrna==S) { j -= 9;}
            if (dirByrna==W) { i += 11; j -= 1;}
            if (dirByrna==E) { i -= 8; j += 3;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 9; j += 9; dirByrna2=S;}
            if (dirByrna==S) { i -= 3; j -= 7; dirByrna2=N;}
            if (dirByrna==W) { i += 9; j -= 4; dirByrna2=E;}
            if (dirByrna==E) { i -= 6; j += 8; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 12 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 8; j -= 6; dirByrna2=W;}
            if (dirByrna==S) { i += 12; j += 27; dirByrna2=E;}
            if (dirByrna==W) { i -= 13; j += 19; dirByrna2=S;}
            if (dirByrna==E) { i += 19; j -= 4; dirByrna2=N;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i -= 5; j += 8;}
            if (dirByrna==S) { i += 8; j -= 6;}
            if (dirByrna==W) { i += 8; j += 8;}
            if (dirByrna==E) { i -= 7; j += 4;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i -= 1; j += 11;}
            if (dirByrna==S) { i += 4; j -= 8;}
            if (dirByrna==W) { i += 10; j += 3;}
            if (dirByrna==E) { i -= 9; j -= 1;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 5; j += 10;}
            if (dirByrna==S) { j -= 9;}
            if (dirByrna==W) { i += 11; j -= 1;}
            if (dirByrna==E) { i -= 7; j += 4;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 13 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 2; j -= 10;}
            if (dirByrna==S) { i += 6; j += 29;}
            if (dirByrna==W) { i -= 15; j += 12;}
            if (dirByrna==E) { i += 22; j += 3;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i -= 8; j += 6; dirByrna2=W;}
            if (dirByrna==S) { i += 9; j -= 2; dirByrna2=E;}
            if (dirByrna==W) { i += 5; j += 10; dirByrna2=S;}
            if (dirByrna==E) { i -= 5; j -= 7; dirByrna2=N;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i -= 4; j += 9;}
            if (dirByrna==S) { i += 8; j -= 7;}
            if (dirByrna==W) { i += 8; j += 6;}
            if (dirByrna==E) { i -= 7; j -= 4;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 1; j += 11;}
            if (dirByrna==S) { i += 4; j -= 8;}
            if (dirByrna==W) { i += 12; j += 3;}
            if (dirByrna==E) i -= 8;
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 14 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i += 6; j -= 11;}
            if (dirByrna==S) { i -= 2; j += 28;}
            if (dirByrna==W) { i -= 15; j += 4;}
            if (dirByrna==E) { i += 23; j += 11;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i -= 10; j += 3;}
            if (dirByrna==S) { i += 10; j += 2;}
            if (dirByrna==W) { i += 1; j += 11;}
            if (dirByrna==E) { i -= 2; j -= 9;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i -= 6; j += 6; dirByrna2=W;}
            if (dirByrna==S) { i += 10; j -= 4; dirByrna2=E;}
            if (dirByrna==W) { i += 6; j += 9; dirByrna2=S;}
            if (dirByrna==E) { i -= 4; j += 6; dirByrna2=N;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i -= 3; j += 10;}
            if (dirByrna==S) { i += 7; j -= 6;}
            if (dirByrna==W) { i += 10; j += 6;}
            if (dirByrna==E) { i -= 7; j -= 3;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 15 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i += 13; j -= 9;}
            if (dirByrna==S) { i -= 9; j += 24;}
            if (dirByrna==W) { i -= 12; j -= 3;}
            if (dirByrna==E) { i += 21; j += 18;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i -= 9; j -= 1;}
            if (dirByrna==S) { i += 9; j += 6;}
            if (dirByrna==W) { i -= 3; j += 10;}
            if (dirByrna==E) { i += 2; j -= 8;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i -= 8; j += 3;}
            if (dirByrna==S) i += 11;
            if (dirByrna==W) { i += 3; j += 11;}
            if (dirByrna==E) { i -= 1; j -= 8;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i -= 6; j += 8; dirByrna2=W;}
            if (dirByrna==S) { i += 10; j -= 4; dirByrna2=E;}
            if (dirByrna==W) { i += 7; j += 8; dirByrna2=S;}
            if (dirByrna==E) { i -= 5; j -= 6; dirByrna2=N;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 16 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i += 19; j -= 4;}
            if (dirByrna==S) { i -= 14; j += 18;}
            if (dirByrna==W) { i -= 6; j -= 9;}
            if (dirByrna==E) { i += 16; j += 24;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i -= 7; j -= 4;}
            if (dirByrna==S) { i += 7; j += 9;}
            if (dirByrna==W) { i -= 7; j += 9;}
            if (dirByrna==E) { i += 5; j -= 6;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i -= 9; j -= 1;}
            if (dirByrna==S) { i += 10; j += 4;}
            if (dirByrna==W) { i -= 1; j += 10;}
            if (dirByrna==E) { i += 3; j -= 9;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i -= 7; j += 4;}
            if (dirByrna==S) { i += 12; j -= 1;}
            if (dirByrna==W) { i += 4; j += 10;}
            if (dirByrna==E) { i -= 1; j -= 8;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 17 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i += 22; j += 3;}
            if (dirByrna==S) { i -= 15; j += 11;}
            if (dirByrna==W) j -= 11;
            if (dirByrna==E) { i += 10; j += 28;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i -= 5; j -= 7;}
            if (dirByrna==S) { i += 3; j += 10;}
            if (dirByrna==W) { i -= 9; j += 6;}
            if (dirByrna==E) { i += 8; j -= 4;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i -= 7; j -= 4;}
            if (dirByrna==S) { i += 9; j += 8;}
            if (dirByrna==W) { i -= 4; j += 8;}
            if (dirByrna==E) { i += 6; j -= 7;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i -= 8; dirByrna2=E;}
            if (dirByrna==S) { i += 11; j += 3; dirByrna2=W;}
            if (dirByrna==W) { j += 11; dirByrna2=N;}
            if (dirByrna==E) { i += 3; j -= 9; dirByrna2=S;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 18 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i += 23; j += 11;}
            if (dirByrna==S) { i -= 14; j += 3;}
            if (dirByrna==W) { i += 8; j -= 11;}
            if (dirByrna==E) { i += 2; j += 29;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i -= 2; j -= 9;}
            if (dirByrna==S) { i -= 1; j += 11;}
            if (dirByrna==W) { i -= 10; j += 2;}
            if (dirByrna==E) { i += 10; j -= 1;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i -= 4; j -= 6; dirByrna2=E;}
            if (dirByrna==S) { i += 6; j += 10; dirByrna2=W;}
            if (dirByrna==W) { i -= 7; j += 6; dirByrna2=N;}
            if (dirByrna==E) { i += 8; j -= 4; dirByrna2=S;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i -= 7; j -= 4;}
            if (dirByrna==S) { i += 9; j += 6;}
            if (dirByrna==W) { i -= 3; j += 9;}
            if (dirByrna==E) { i += 7; j -= 8;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 19 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i += 21; j += 18;}
            if (dirByrna==S) { i -= 10; j -= 4;}
            if (dirByrna==W) { i += 15; j -= 8;}
            if (dirByrna==E) { i -= 5; j += 27;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 2; j -= 8; dirByrna2=E;}
            if (dirByrna==S) { i -= 5; j += 10; dirByrna2=W;}
            if (dirByrna==W) { i -= 9; j -= 2; dirByrna2=N;}
            if (dirByrna==E) { i += 9; j += 3; dirByrna2=S;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i -= 1; j -= 8;}
            if (dirByrna==S) { i += 2; j += 11;}
            if (dirByrna==W) { i -= 9; j += 3;}
            if (dirByrna==E) { i += 10; j -= 1;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i -= 5; j -= 7;}
            if (dirByrna==S) { i += 7; j += 9;}
            if (dirByrna==W) { i -= 5; j += 6;}
            if (dirByrna==E) { i += 10; j -= 6;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 20 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i += 16; j += 24; dirByrna2=E;}
            if (dirByrna==S) { i -= 4; j -= 9; dirByrna2=W;}
            if (dirByrna==W) { i += 21; j -= 2; dirByrna2=N;}
            if (dirByrna==E) { i -= 11; j += 22; dirByrna2=S;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 5; j -= 6;}
            if (dirByrna==S) { i -= 8; j += 8;}
            if (dirByrna==W) { i -= 8; j -= 6;}
            if (dirByrna==E) { i += 7; j += 6;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 3; j -= 9;}
            if (dirByrna==S) { i -= 2; j += 10;}
            if (dirByrna==W) { i -= 8; j -= 1;}
            if (dirByrna==E) { i += 11; j += 3;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i -= 1; j -= 8;}
            if (dirByrna==S) { i += 4; j += 11;}
            if (dirByrna==W) { i -= 7; j += 3;}
            if (dirByrna==E) { i += 11; j -= 2;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 21 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i += 10; j += 28; dirByrna2=S;}
            if (dirByrna==S) { i += 2; j -= 12; dirByrna2=N;}
            if (dirByrna==W) { i += 23; j += 4; dirByrna2=E;}
            if (dirByrna==E) { i -= 14; j += 14; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 8; j -= 4; dirByrna2=E;}
            if (dirByrna==S) { i -= 9; j += 4; dirByrna2=W;}
            if (dirByrna==W) { i -= 5; j -= 8; dirByrna2=N;}
            if (dirByrna==E) { i += 5; j += 9; dirByrna2=S;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 6; j -= 7;}
            if (dirByrna==S) { i -= 6; j += 9;}
            if (dirByrna==W) { i -= 6; j -= 4;}
            if (dirByrna==E) { i += 9; j += 6;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 3; j -= 9;}
            if (dirByrna==S) j += 10;
            if (dirByrna==W) { i -= 8; j -= 1;}
            if (dirByrna==E) { i += 12; j += 2;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 22 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i += 2; j += 29; dirByrna2=S;}
            if (dirByrna==S) { i += 10; j -= 11; dirByrna2=N;}
            if (dirByrna==W) { i += 23; j += 12; dirByrna2=E;}
            if (dirByrna==E) { i -= 15; j += 6; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 10; j -= 1;}
            if (dirByrna==S) i -= 10;
            if (dirByrna==W) { i -= 1; j -= 9;}
            if (dirByrna==E) { i += 2; j += 11;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 8; j -= 4; dirByrna2=E;}
            if (dirByrna==S) { i -= 8; j += 6; dirByrna2=W;}
            if (dirByrna==W) { i -= 4; j -= 7; dirByrna2=N;}
            if (dirByrna==E) { i += 6; j += 8; dirByrna2=S;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 7; j -= 8;}
            if (dirByrna==S) { i -= 3; j += 8;}
            if (dirByrna==W) { i -= 6; j -= 4;}
            if (dirByrna==E) { i += 11; j += 6;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 23 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 5; j += 27; dirByrna2=S;}
            if (dirByrna==S) { i += 17; j -= 7; dirByrna2=N;}
            if (dirByrna==W) { i += 20; j += 19; dirByrna2=E;}
            if (dirByrna==E) { i -= 13; j -= 1; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 9; j += 3;}
            if (dirByrna==S) { i -= 9; j -= 4;}
            if (dirByrna==W) { i += 3; j -= 8;}
            if (dirByrna==E) { i -= 2; j += 10;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 10; j -= 1;}
            if (dirByrna==S) { i -= 9; j += 2;}
            if (dirByrna==W) { i -= 1; j -= 9;}
            if (dirByrna==E) { i += 3; j += 10;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 10; j -= 6; dirByrna2=E;}
            if (dirByrna==S) { i -= 6; j += 6; dirByrna2=W;}
            if (dirByrna==W) { i -= 3; j -= 6; dirByrna2=N;}
            if (dirByrna==E) { i += 9; j += 9; dirByrna2=S;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 24 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 11; j += 22; dirByrna2=S;}
            if (dirByrna==S) { i += 22; j -= 1; dirByrna2=N;}
            if (dirByrna==W) { i += 14; j += 25; dirByrna2=E;}
            if (dirByrna==E) { i -= 8; j -= 7; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 7; j += 6;}
            if (dirByrna==S) { i -= 7; j -= 7;}
            if (dirByrna==W) { i += 7; j -= 7;}
            if (dirByrna==E) { i -= 5; j += 8;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 11; j += 3;}
            if (dirByrna==S) { i -= 8; j -= 2;}
            if (dirByrna==W) { i += 3; j -= 8;}
            if (dirByrna==E) { i -= 1; j += 11;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 11; j -= 2;}
            if (dirByrna==S) { i -= 8; j += 3;}
            if (dirByrna==W) j -= 8;
            if (dirByrna==E) { i += 5; j += 10;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 25 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 15; j += 15; dirByrna2=W;}
            if (dirByrna==S) { i += 24; j += 6; dirByrna2=E;}
            if (dirByrna==W) { i += 7; j += 28; dirByrna2=S;}
            if (dirByrna==E) { i -= 6; j -= 9; dirByrna2=N;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 5; j += 9; dirByrna2=S;}
            if (dirByrna==S) { i -= 3; j -= 8; dirByrna2=N;}
            if (dirByrna==W) { i += 9; j -= 4; dirByrna2=E;}
            if (dirByrna==E) { i -= 7; j += 9; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 9; j += 6;}
            if (dirByrna==S) { i -= 7; j -= 6;}
            if (dirByrna==W) { i += 6; j -= 6;}
            if (dirByrna==E) { i -= 1; j += 10;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 12; j += 2;}
            if (dirByrna==S) { i -= 7; j -= 1;}
            if (dirByrna==W) { i += 4; j -= 9;}
            if (dirByrna==E) { i += 4; j += 10;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 26 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 16; j += 7; dirByrna2=W;}
            if (dirByrna==S) { i += 23; j += 14; dirByrna2=E;}
            if (dirByrna==W) { i -= 1; j += 28; dirByrna2=S;}
            if (dirByrna==E) { i += 7; j -= 12; dirByrna2=N;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i += 2; j += 11;}
            if (dirByrna==S) { i += 1; j -= 9;}
            if (dirByrna==W) i += 10;
            if (dirByrna==E) { i -= 10; j += 3;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 6; j += 8; dirByrna2=S;}
            if (dirByrna==S) { i -= 4; j -= 8; dirByrna2=N;}
            if (dirByrna==W) { i += 9; j -= 4; dirByrna2=E;}
            if (dirByrna==E) { i -= 6; j += 6; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 11; j += 6;}
            if (dirByrna==S) { i -= 5; j -= 4;}
            if (dirByrna==W) { i += 7; j -= 7;}
            if (dirByrna==E) { i -= 3; j += 10;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
        case 27 :
            box.setW(8); box.setH(8);
            if (dirByrna==N) { i -= 14; dirByrna2=W;}
            if (dirByrna==S) { i += 19; j += 22; dirByrna2=E;}
            if (dirByrna==W) { i -= 8; j += 25; dirByrna2=S;}
            if (dirByrna==E) { i += 14; j -= 10; dirByrna2=N;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(7); box.setH(7);
            if (dirByrna==N) { i -= 2; j += 10;}
            if (dirByrna==S) { i += 5; j -= 8;}
            if (dirByrna==W) { i += 9; j += 4;}
            if (dirByrna==E) { i -= 9; j -= 1;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(5); box.setH(5);
            if (dirByrna==N) { i += 3; j += 10;}
            if (dirByrna==S) j -= 9;
            if (dirByrna==W) { i += 11; j -= 1;}
            if (dirByrna==E) { i -= 8; j += 3;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);

            box.setW(1); box.setH(1);
            if (dirByrna==N) { i += 9; j += 9; dirByrna2=S;}
            if (dirByrna==S) { i -= 3; j -= 7; dirByrna2=N;}
            if (dirByrna==W) { i += 9; j -= 4; dirByrna2=E;}
            if (dirByrna==E) { i -= 6; j += 8; dirByrna2=W;}
            box.setX(i + x); box.setY(j + y);
            MainController::getInstance()->getGameController()->
                getSceneController()->getScene()->
                testDegat(&box, dirByrna2, force, TA_MAGIC, TE_NORMAL);
            break;
    }
}

void Link::drawElectric(int dstX, int dstY) {
    WindowManager::getInstance()->draw(imageLink, 416+16*(anim%2), 96, w, h, dstX, dstY);
}
