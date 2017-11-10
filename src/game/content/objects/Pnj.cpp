#include "Pnj.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../MainController.h"
#include "../../game/scene/Scene.h"

Pnj::Pnj(int i, int j, int tp, int txt) : type(tp), texte(txt), anim(0), animMax(0), vanim(480), w(0), h(0),
    displayW(0), displayH(0), idle(true), direction(S), stop(false) {
    x = i;
    y = j;

    image = 0;

    ostringstream os;
    os << type;
    image = ResourceManager::getInstance()->loadImage("data/images/npcs/personnage" + os.str() + ".png", true);

    // for quadtree operations:
    width = 16;
    height = 16;

    w = width;
    h = height;

    displayW = w;
    displayH = h;

    init();

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    chrono.reset();
}

Pnj::~Pnj() {
    ResourceManager::getInstance()->free(image);
}

void Pnj::init() {
    switch (type) {
        case 1 : height = 29; h = height; displayH = h; animMax = 3; break;
        case 2 : height = 16 * 3 - 4; h = 26; displayH = height - 25; animMax = 3; break;
        case 3 : height = 16 * 3 - 6; h = 24; displayH = height - 25; animMax = 1; break;
        case 4 : height = 16 * 3 - 6; h = 24; displayH = height - 25; animMax = 1; break;
        case 5 : height = 16 * 3 - 6; h = 24; displayH = height - 25; animMax = 1; break;
        case 6 : height = 16 * 3 - 5; h = 25; displayH = height - 25; animMax = 1; break;
        case 7 : height = 16 * 3 - 7; h = 23; displayH = height - 25; animMax = 1; break;
        case 8 : width = 22; w = width; displayW = w; height = 27; h = height; displayH = h; animMax = 0; break;
        case 9 : idle = false; height = 24; animMax = 1; vanim = 240; break;
        case 10 : height = 25; h = height; displayH = h; animMax = 1; break;
        case 11 : height = 22; h = height; displayH = h; animMax = 3; break;
        case 12 : idle = false; height = 24; animMax = 1; vanim = 240; break;
        case 13 : height = 16 * 3 - 5; h = 25; displayH = height - 25; animMax = 1; break;
        case 14 : height = 16 * 3 - 7; h = 23; displayH = height - 25; animMax = 1; break;
        case 15 : width = 24; w = width; displayW = w; height = 16 * 3 - 7; h = 16; displayH = height - 25; animMax = 1; break;
        case 16 : height = 16 * 3 - 7; h = 23; displayH = height - 25; animMax = 1; break;
        case 17 : height = 16 * 3 - 5; h = 25; displayH = height - 25; animMax = 1; break;
        case 18 : height = 16 * 3 - 4; h = 26; displayH = height - 25; animMax = 3; break;
        case 19 : idle = false; height = 25; animMax = 1; vanim = 240; break;
        case 20 : idle = false; height = 25; animMax = 1; vanim = 240; break;
        case 21 : idle = false; height = 25; animMax = 1; vanim = 240; break;
        case 22 : width = 32; w = width; displayW = w; height = 48; h = height; displayH = h; animMax = 1; break;
        case 23 : width = 18; w = width; displayW = w; height = 16 * 3 - 5; h = 25; displayH = height - 25; animMax = 1; break;
        case 24 : height = 16 * 3 - 6; h = 24; displayH = height - 25; animMax = 1; break;
        case 25 : height = 16 * 3 - 7; h = 23; displayH = height - 25; animMax = 1; break;
        case 26 : idle = false; width = 19; height = 22; animMax = 1; vanim = 240; break;
        case 27 : idle = false; height = 24; animMax = 1; vanim = 240; break;
        case 28 : width = 23; w = 16; displayW = 23; height = 25; h = height; displayH = h; animMax = 1; break;
        case 29 : width = 24; w = width; displayW = w; height = 27; h = height; displayH = h; animMax = 3; break;
        case 30 : width = 32; w = width; displayW = w; height = 32; h = height; displayH = h; animMax = 3; break;
        case 31 : width = 32; w = width; displayW = w; height = 48; h = height; displayH = h; animMax = 1; break;
        case 32 : height = 29; h = height; displayH = h; animMax = 3; break;
        case 33 : height = 26; h = height; displayH = h; animMax = 3; break;
        case 34 : height = 26; h = height; displayH = h; animMax = 3; break;
        case 35 : width = 18; w = width; displayW = w; height = 25; h = height; displayH = h; animMax = 1; break;
        case 36 : height = 26; h = height; displayH = h; animMax = 0; break;
        case 37 : width = 17; w = width; displayW = w; height = 25; h = height; displayH = h; animMax = 1; break;
        case 38 : height = 24; h = height; displayH = h; animMax = 1; break;
        case 39 : height = 25; h = height; displayH = h; animMax = 1; vanim = 960; break;
        case 40 : height = 24; h = height; displayH = h; animMax = 1; break;
        case 41 : height = 29; h = height; displayH = h; animMax = 1; break;
        case 42 : height = 23; h = height; displayH = h; animMax = 1; break;
        case 43 : height = 23; h = height; displayH = h; animMax = 1; break;
        case 44 : height = 25; h = height; displayH = h; animMax = 1; break;
        case 45 : height = 25; h = height; displayH = h; animMax = 1; break;
        case 46 : width = 22; w = width; displayW = w; height = 27; h = height; displayH = h; animMax = 1; break;
        case 47 : width = 22; w = width; displayW = w; height = 27; h = height; displayH = h; animMax = 0; break;
        case 48 : height = 23; h = height; displayH = h; animMax = 1; break;
        case 49 : height = 28; h = height; displayH = h; animMax = 0; break;
        case 50 : height = 25; h = height; displayH = h; animMax = 1; break;
        case 51 : width = 25; w = width; displayW = w; height = 35; h = height; displayH = h; animMax = 4; break;
        case 52 : height = 17; h = height; displayH = h; animMax = 3; break;
        case 53 : height = 17; h = height; displayH = h; animMax = 1; break;
        case 54 : height = 17; h = height; displayH = h; animMax = 1; break;
        case 55 : height = 24; h = height; displayH = h; animMax = 1; break;
        case 56 : height = 24; h = height; displayH = h; animMax = 1; break;
        case 57 : height = 25; h = height; displayH = h; animMax = 1; break;
        case 58 : height = 24; h = height; displayH = h; animMax = 1; break;
        case 59 : height = 25; h = height; displayH = h; animMax = 0; break;
        case 60 : height = 21; h = height; displayH = h; animMax = 1; break;
    }
}

void Pnj::loop() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    switch (type) {
        case 8 :
            if (scene->getMap()->getId() == 3) {
                if (scene->getAvancement() == AV_GARDE_MOVE) {
                    animMax = 1;
                }
                if (animMax) {
                    if (y < 16 * 6) {
                        moveY(1);
                    } else if (x > 16*57 - 3) {
                        moveX(-1);
                    } else {
                        animMax = 0;
                        scene->setAvancement(AV_GARDE_OUT);
                    }
                }
            }
            break;
    }

    if (!idle) {

        BoundingBox bb(x - 16*2, y - 16*2, width + 32*2, height + 32*2);
        stop = !scene->checkCollisionsWithLink(&bb);

        if (stop) {
            BoundingBox* bblink = scene->getLink()->getBoundingBox();
            int dN = y - (bblink->getY() + bblink->getH());
            int dS = bblink->getY() - (y + height);
            int dW = x - (bblink->getX() + bblink->getW());
            int dE = bblink->getX() - (x + width);
            if (dN >= dS && dN >= dW && dN >= dE) direction = N;
            if (dS >= dN && dS >= dW && dS >= dE) direction = S;
            if (dW >= dS && dW >= dN && dW >= dE) direction = W;
            if (dE >= dS && dE >= dW && dE >= dN) direction = E;
        } else {
            // move
            int randomValue = (int)((float)rand() / RAND_MAX * (100));
            switch (randomValue) {
                case 1 : moveX(-1);direction=W; break;
                case 2 : moveX(1); direction=E; break;
                case 3 : moveY(-1);direction=N; break;
                case 4 : moveY(1); direction=S; break;
                default :
                    if (randomValue < 8) break;
                    switch (direction) {
                        case N : moveY(-1); break;
                        case S : moveY(1); break;
                        case W : moveX(-1); break;
                        case E : moveX(1); break;
                    }
                    break;
            }
        }
    }

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        chrono.reset();
    }
}

void Pnj::draw(int offsetX, int offsetY) {
    if (idle) {
        switch (type) {
            case 8 :
                if (animMax == 0) {
                    WindowManager::getInstance()->draw(image, 0, h * 2, displayW, displayH, x - offsetX, y - offsetY);
                } else {
                    WindowManager::getInstance()->draw(image, 0, h * anim, displayW, displayH, x - offsetX, y - offsetY);
                }
                break;
            default :
                WindowManager::getInstance()->draw(image, 0, h * anim, displayW, displayH, x - offsetX, y - offsetY);
                break;
        }
    } else {
        WindowManager::getInstance()->draw(image, width * direction, height * anim, width, height, x - offsetX, y - offsetY);
    }
}

bool Pnj::action(Direction dir) {
    MainController::getInstance()->getGameController()->displayText(getDynamicText());
    return true;
}

BoundingBox* Pnj::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    if (idle) {
        box.setW(w);
        box.setH(h);
    }
    return &box;
}

int Pnj::getDown() {
    if (idle) {
        return y + displayH;
    } else {
        return y + height;
    }
}

int Pnj::getDynamicText() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    Navi* navi = scene->getNavi();
    int tmp = 0;
    switch (texte) {
        case 67 :
            if (link->getInventory()->hasCoquillages(0)) {
                return 69;
            } else if (scene->getMap()->nbPouleInBox() == 5) {
                return 68;
            }
            break;
        case 113 :
            if (link->getStatus()->getMaxRupees() == 99) {
                return 115;
            } else if (link->getStatus()->getMaxRupees() == 200) {
                return 117;
            } else if (link->getStatus()->getMaxRupees() == 500) {
                return 119;
            } else if (link->getStatus()->getMaxRupees() == 999) {
                return 125;
            }
            break;
        case 126 :
            if (link->getBouclier() == 1) {
                return 128;
            } else if (link->getBouclier() == 2) {
                return 130;
            } else if (link->getBouclier() == 3) {
                return 125;
            }
            break;
        case 135 :
            if (scene->getAvancement() == AV_MAIRE_OK) {
                if (animMax > 0) {
                    return 138;
                }
                scene->setAvancement(AV_GARDE_MOVE);
                return 136;
            } else if (scene->getAvancement() >= AV_GARDE_MOVE) {
                return 138;
            }
            break;
        case 141 :
            if (link->getInventory()->hasCoquillages(0)) {
                return 142;
            }
            break;
        case 174 :
            if (!link->getInventory()->hasObject(FLACON_1)) {
                return 174;
            } else if (!link->getInventory()->hasObject(FLACON_2)) {
                return 176;
            } else {
                return 125;
            }
            break;
        case 178 :
            if (!link->getInventory()->hasGemmeForce(25)) {
                return 178;
            } else if (!link->getInventory()->hasGemmeForce(26)) {
                return 180;
            } else if (!link->getInventory()->hasGemmeForce(27)) {
                return 182;
            } else if (!link->getInventory()->hasGemmeForce(28)) {
                return 184;
            } else if (!link->getInventory()->hasGemmeForce(29)) {
                return 186;
            } else if (!link->getInventory()->hasCoquillages(1)) {
                return 188;
            } else {
                return 125;
            }
            break;
        case 215 :
            // féé des gemmes
            if (scene->getFees(0) == 2) {
                int nb = link->getInventory()->nbGemmeForce();
                int tot = link->getInventory()->totalGemmeForce();
                int used = tot - nb;
                if (used == 0) {
                    if (nb < 15) {
                        return 216;
                    } else {
                        return 217;
                    }
                }
                if (used == 45) {
                    if (nb < 10) {
                        return 224;
                    } else {
                        return 225;
                    }
                }
                if (used == 85) {
                    if (nb < 15) {
                        return 227;
                    } else {
                        return 228;
                    }
                }
                if (used == 100) {
                    return 230;
                }
                int lvl = (navi->getMaxMagic() - 15) / 5;
                if (lvl < 1) lvl = 1;
                bool heal = navi->getLvlHeal() < lvl;
                bool att = lvl > 1 && navi->getLvlAtt() < lvl;
                bool def = lvl > 2 && navi->getLvlDef() < lvl;

                if (heal || att || def) {
                    if (nb < 5) {
                        return 255;
                    }
                    return 232;
                }
                return 231;
            } else {
                scene->setFees(0, 2);
            }
            break;
        case 263 :
            if (link->getInventory()->hasObject(RECETTE_POTION_JAUNE)) {
                return 125;
            }
            break;
        case 266 :
            if (!link->getInventory()->hasQuartCoeur(8)) {
                return 266;
            } else if (!link->getInventory()->hasQuartCoeur(9)) {
                return 268;
            } else if (!link->getInventory()->hasQuartCoeur(10)) {
                return 270;
            } else if (!link->getInventory()->hasQuartCoeur(11)) {
                return 272;
            } else {
                return 125;
            }
            break;
        case 298 :
            if (link->getInventory()->hasObject(GANTS_2)) {
                if (link->getInventory()->hasObject(ARC_FEE)
                    && link->getInventory()->hasObject(BOOMERANG_FEE)) {
                    return 303;
                }
                if (link->getInventory()->getCurrent() == ARC
                    && link->getInventory()->hasObject(ARC)) {
                    return 300;
                }
                if (link->getInventory()->getCurrent() == BOOMERANG) {
                    return 301;
                }
                return 302;
            }
            break;
        case 310 :
            if ((scene->getMap()->getId() == 17 && scene->getFees(2) == 2)
                || (scene->getMap()->getId() == 19 && scene->getFees(4) == 2)
                || (scene->getMap()->getId() == 25 && scene->getFees(6) == 2)
                || (scene->getMap()->getId() == 28 && scene->getFees(7) == 2)) {
                return 311;
            }
            break;
        case 316 :
            if (link->getStatus()->getMaxMagic() == 64) {
                return 303;
            }
            break;
        case 319 :
            if (link->getInventory()->hasObject(PALMES)) {
                return 323;
            }
            break;
        case 326 :
            if (scene->getFees(5) == 2) {
                if (link->getStatus()->getRecupArrows() && link->getStatus()->getRecupBombs()) {
                    return 327;
                }
                if (link->getStatus()->getMaxArrows() == 70 && link->getStatus()->getMaxBombs() == 30) {
                    return 330;
                }
                if (link->getStatus()->getMaxBombs() == 30 && !link->getInventory()->hasObject(ARC)) {
                    return 643;
                }
                return 328;
            }
            scene->setFees(5, 2);
            break;
        case 359 :
            if (link->getInventory()->hasObject(CARTE_ILE)
                && link->getInventory()->hasCoeur(4)
                && link->getInventory()->hasObject(DETECTEUR)
                && link->getTunique() == 3
                && link->getInventory()->hasObject(CANNE_BYRNA)) {
                return 360;
            }
            break;
        case 380 :
            if (scene->getMap()->areChestsClosed()) {
                return 394;
            }
            if (link->getInventory()->hasCoquillages(2)) {
                return 391;
            }
            if (link->getInventory()->hasGemmeForce(87)) {
                if (!link->getInventory()->hasObject(CARTE_ILE)) {
                    return 390;
                }
                return 392;
            }
            if (link->getInventory()->hasGemmeForce(86)) {
                return 388;
            }
            if (link->getInventory()->hasGemmeForce(85)) {
                return 386;
            }
            if (link->getInventory()->hasGemmeForce(84)) {
                return 384;
            }
            if (link->getInventory()->hasGemmeForce(83)) {
                return 382;
            }
            break;
        case 413 :
            if (link->getInventory()->hasObject(CHANT_3)) {
                return 433;
            }
            if (link->getInventory()->nbMedaillons() == 7) {
                if (scene->getAvancement() < AV_MEDAILLONS_OK) {
                    scene->setAvancement(AV_MEDAILLONS_OK);
                    return 431;
                }
                return 432;
            }
            if (link->getInventory()->nbMedaillons()) {
                return 430;
            }
            if (link->getInventory()->hasObject(SAC_TROC)) {
                if (scene->getAvancement() >= AV_MAIRE_OK) {
                    return 429;
                }
                if (link->getBouclier()
                    && link->getInventory()->hasObject(CARTE_ILE)) {
                    return 428;
                }
                return 425;
            }
            break;
        case 461 :
            if (scene->getMap()->isGameStarted()) {
                return 469;
            }
            if (scene->getMap()->getGameScore() >= 30) {
                return 473;
            }
            if (scene->getMap()->getGameScore() >= 15) {
                return 472;
            }
            if (scene->getMap()->getGameScore() >= 2) {
                return 471;
            }
            if (scene->getMap()->getGameScore() >= 0) {
                return 470;
            }
            if (!link->getInventory()->hasObject(ARC)) {
                return 465;
            }
            if (link->getInventory()->hasGemmeForce(89)
                && !link->getInventory()->hasCoquillages(10)) {
                return 462;
            }
            if (!link->getInventory()->hasGemmeForce(89)
                && link->getInventory()->hasCoquillages(10)) {
                return 463;
            }
            if (link->getInventory()->hasGemmeForce(89)
                && link->getInventory()->hasCoquillages(10)) {
                return 464;
            }
            break;
        case 480 :
            if (scene->getCoffre(0, 11) > 47) {
                return 486;
            }
            break;
        case 495 :
            if (link->getInventory()->hasCoquillages(11)
                && link->getInventory()->hasCoquillages(12)
                && link->getInventory()->hasCoquillages(13)
                && link->getInventory()->hasCoquillages(14)
                && link->getInventory()->hasCoquillages(15)
                && link->getInventory()->hasCoquillages(16)
                && link->getInventory()->hasCoquillages(17)) {
                return 505;
            }
            if (scene->getCoffre(0, 12) && !link->getInventory()->hasCoquillages(11)) {
                return 507;
            }
            if (scene->getCoffre(0, 13) && !link->getInventory()->hasCoquillages(12)) {
                return 508;
            }
            if (scene->getCoffre(0, 14) && !link->getInventory()->hasCoquillages(13)) {
                return 509;
            }
            if (scene->getCoffre(0, 15) && !link->getInventory()->hasCoquillages(14)) {
                return 510;
            }
            if (scene->getCoffre(0, 16) && !link->getInventory()->hasCoquillages(15)) {
                return 511;
            }
            if (scene->getCoffre(0, 17) && !link->getInventory()->hasCoquillages(16)) {
                return 512;
            }
            if (scene->getCoffre(0, 18) && !link->getInventory()->hasCoquillages(17)) {
                return 513;
            }
            break;
        case 514 :
            if (!scene->getCoffre(0, 19) && link->getInventory()->hasObject(RECETTE_POTION_JAUNE)) {
                scene->setCoffre(0, 19, 1);
                return 518;
            }
            if ( (!link->getInventory()->hasObject(FLACON_1) || link->getInventory()->getFlacon(0))
                && (!link->getInventory()->hasObject(FLACON_2) || link->getInventory()->getFlacon(1))
                && (!link->getInventory()->hasObject(FLACON_3) || link->getInventory()->getFlacon(2))
                && (!link->getInventory()->hasObject(FLACON_4) || link->getInventory()->getFlacon(3)) ) {
                return 528;
            }
            break;
        case 536 :
            // check potions
            tmp = -1;
            for (int i = 0; i < NB_FLACON; i++) {
                switch (i) {
                    case 0 : if (!link->getInventory()->hasObject(FLACON_1)) continue; break;
                    case 1 : if (!link->getInventory()->hasObject(FLACON_2)) continue; break;
                    case 2 : if (!link->getInventory()->hasObject(FLACON_3)) continue; break;
                    case 3 : if (!link->getInventory()->hasObject(FLACON_4)) continue; break;
                }
                if (link->getInventory()->getFlacon(i) == 1
                    || link->getInventory()->getFlacon(i) == 2
                    || link->getInventory()->getFlacon(i) == 4) {
                    tmp = i;
                    break;
                }
            }
            if (tmp == -1) return 541;
            for (int i = tmp + 1; i < NB_FLACON; i++) {
                switch (i) {
                    case 0 : if (!link->getInventory()->hasObject(FLACON_1)) continue; break;
                    case 1 : if (!link->getInventory()->hasObject(FLACON_2)) continue; break;
                    case 2 : if (!link->getInventory()->hasObject(FLACON_3)) continue; break;
                    case 3 : if (!link->getInventory()->hasObject(FLACON_4)) continue; break;
                }
                if ((link->getInventory()->getFlacon(i) == 1
                    || link->getInventory()->getFlacon(i) == 2
                    || link->getInventory()->getFlacon(i) == 4)
                    && link->getInventory()->getFlacon(i) != link->getInventory()->getFlacon(tmp)) {
                    if ( (!link->getInventory()->hasObject(FLACON_1) || link->getInventory()->getFlacon(0))
                        && (!link->getInventory()->hasObject(FLACON_2) || link->getInventory()->getFlacon(1))
                        && (!link->getInventory()->hasObject(FLACON_3) || link->getInventory()->getFlacon(2))
                        && (!link->getInventory()->hasObject(FLACON_4) || link->getInventory()->getFlacon(3)) ) {
                        return 541;
                    }
                    return 536;
                }
            }
            return 541;
        case 542 :
            // check potions
            tmp = -1;
            for (int i = 0; i < NB_FLACON; i++) {
                switch (i) {
                    case 0 : if (!link->getInventory()->hasObject(FLACON_1)) continue; break;
                    case 1 : if (!link->getInventory()->hasObject(FLACON_2)) continue; break;
                    case 2 : if (!link->getInventory()->hasObject(FLACON_3)) continue; break;
                    case 3 : if (!link->getInventory()->hasObject(FLACON_4)) continue; break;
                }
                if (link->getInventory()->getFlacon(i) == 3
                    || link->getInventory()->getFlacon(i) == 5
                    || link->getInventory()->getFlacon(i) == 6) {
                    tmp = i;
                    break;
                }
            }
            if (tmp == -1) return 544;
            for (int i = tmp + 1; i < NB_FLACON; i++) {
                switch (i) {
                    case 0 : if (!link->getInventory()->hasObject(FLACON_1)) continue; break;
                    case 1 : if (!link->getInventory()->hasObject(FLACON_2)) continue; break;
                    case 2 : if (!link->getInventory()->hasObject(FLACON_3)) continue; break;
                    case 3 : if (!link->getInventory()->hasObject(FLACON_4)) continue; break;
                }
                if ((link->getInventory()->getFlacon(i) == 3
                    || link->getInventory()->getFlacon(i) == 5
                    || link->getInventory()->getFlacon(i) == 6)
                    && link->getInventory()->getFlacon(i) != link->getInventory()->getFlacon(tmp)) {
                    if ( (!link->getInventory()->hasObject(FLACON_1) || link->getInventory()->getFlacon(0))
                        && (!link->getInventory()->hasObject(FLACON_2) || link->getInventory()->getFlacon(1))
                        && (!link->getInventory()->hasObject(FLACON_3) || link->getInventory()->getFlacon(2))
                        && (!link->getInventory()->hasObject(FLACON_4) || link->getInventory()->getFlacon(3)) ) {
                        return 544;
                    }
                    return 542;
                }
            }
            return 544;
            break;
        case 574 :
            if (scene->getCoffre(0, 9)) {
                return 578;
            }
            if (link->getInventory()->hasObject(CHANT_3)) {
                return 577;
            }
            if (link->getInventory()->nbMedaillons() == 7) {
                return 575;
            }
            break;
        default : break;
    }
    return texte;
}

void Pnj::moveX(int dx) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    int oldX = x;

    BoundingBox* bb = getBoundingBox();
    bb->setX(x + dx);

    if (scene->checkCollisions(bb, this, false) && scene->checkCollisionsWithLink(bb)) {
        x += dx;
    }

    if (x != oldX) checkPosition();
}

void Pnj::moveY(int dy) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    int oldY = y;

    BoundingBox* bb = getBoundingBox();
    bb->setY(y + dy);

    if (scene->checkCollisions(bb, this, false) && scene->checkCollisionsWithLink(bb)) {
        y += dy;
    }

    if (y != oldY) checkPosition();
}
