#include "AnimationInGame.h"

#include "../link/Navi.h"

#include "../effects/NaviMagic.h"

#include "../../../game/MainController.h"
#include "../../../engine/audio/AudioManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/resources/ResourceManager.h"

#include "../objects/Pnj.h"

#include "../effects/NaviMagic.h"

AnimationInGame::AnimationInGame() : id(0), anim(0), running(false), image(0), surface(0)  {
}

AnimationInGame::~AnimationInGame() {
    ResourceManager::getInstance()->free(image);
    delete surface;
}

void AnimationInGame::testAnim() {

    if (running) {
        return;
    }

    Avancement avancement = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getAvancement();

    link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
    map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();

    switch (avancement) {
        case AV_START :
            startAnim(1);
            break;
        case AV_NAVI_IS_HERE :
            if (map->getId() == 15) {
                startAnim(12);
            }
            break;
        case AV_TUTO_STARTED :
            if (map->getId() == 15 && map->getBounds()->getY() == 240) {
                startAnim(13);
            }
            break;
        case AV_TUTO_DONE :
            if (map->getId() == 1 && link->getEpee()) {
                startAnim(2);
            }
            break;
        case AV_NAVI_IS_MOVABLE :
            if (map->getId() == 2) {
                startAnim(3);
            }
            break;
        case AV_NAVI_CAN_HEAL :
            if (map->getId() == 3) {
                startAnim(4);
            }
            break;
        case AV_MAIRE_VU :
            if (map->getId() == 3) {
                startAnim(5);
            }
            break;
        case AV_GARDE_OUT :
            if (map->getId() == 2 && link->getX() > 90 * 16) {
                startAnim(6);
            }
            break;
        case AV_PRESENTATIONS_DONE :
            if (map->getId() == 2 && link->getInventory()->hasMedaillons(0)) {
                startAnim(7);
            }
            break;
        case AV_NAVI_QUEST_ON :
            if (map->getId() == 6 && link->getInventory()->hasMedaillons(1) && !link->getInventory()->hasObject(OCARINA)) {
                startAnim(8);
            } else if (map->getId() == 6 && link->getInventory()->hasMedaillons(1) && link->getInventory()->hasObject(CHANT_1)) {
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_NAVI_IMPATIENTE);
            }
            break;
        case AV_TUTO_PERLE_LUNE :
            if (map->getId() == 63 && link->getY() < 150 * 16) {
                startAnim(39);
            }
            break;
        case AV_TUTO_SS1_DONE :
            if (map->getId() == 63 && link->getY() < 75 * 16) {
                startAnim(40);
            }
            break;
        case AV_FINAL_TUTO_RDC_DONE :
            if (map->getId() == 66 && link->getX() > 80 * 16) {
                startAnim(47);
            }
            break;
        case AV_FINAL_TUTO_SS1_DONE :
            if (map->getId() == 66 && link->getX() > 160 * 16) {
                startAnim(48);
            }
            break;
        default : break;
    }
}

void AnimationInGame::startAnim(int i) {
    id = i;

    navi = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getNavi();
    link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
    map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    camera = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getCamera();

    lmap = false;
    lcamera = false;
    llink = false;
    lnavi = false;

    switch (id) {
        case 1 :
            navi->setFollowLink(false);
            navi->setX(16 * 108);
            navi->setDirection(W);
            lmap = true;
            llink = true;
            lnavi = true;
            break;
        case 2 : case 3 : case 4 : case 5 : case 6 : case 7 : case 8 : case 9 :
        case 12 : case 13 : case 19 : case 36 : case 37 : case 39 : case 40 :
        case 50 :
            lmap = true;
            llink = true;
            lnavi = true;
            AudioManager::getInstance()->playSound(TS_NAVY_LISTEN);
            break;
        case 10 :
            ResourceManager::getInstance()->free(image);
            map->addObject(new Pnj(39*16, 31*16, 22, 215));
            image = ResourceManager::getInstance()->loadImage("data/images/npcs/personnage22.png", true);
            image->setAlpha(0);
            break;
        case 11 :
            lmap = true;
            llink = true;
            lnavi = true;
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo(39*16+8, 27*16+8, 1);
            break;
        case 14 :
            ResourceManager::getInstance()->free(image);
            map->addObject(new Pnj(9*16, 3*16 + 8, 22, 298));
            image = ResourceManager::getInstance()->loadImage("data/images/npcs/personnage22.png", true);
            image->setAlpha(0);
            break;
        case 15 :
            ResourceManager::getInstance()->free(image);
            map->addObject(new Pnj(9*16 + 4, 5*16 - 3, 29, 310));
            image = ResourceManager::getInstance()->loadImage("data/images/npcs/personnage29.png", true);
            image->setAlpha(0);
            break;
        case 16 :
            ResourceManager::getInstance()->free(image);
            map->addObject(new Pnj(9*16, 3*16 + 8, 22, 316));
            image = ResourceManager::getInstance()->loadImage("data/images/npcs/personnage22.png", true);
            image->setAlpha(0);
            break;
        case 17 :
            ResourceManager::getInstance()->free(image);
            map->addObject(new Pnj(9*16, 3*16 + 8, 31, 326));
            image = ResourceManager::getInstance()->loadImage("data/images/npcs/personnage31.png", true);
            image->setAlpha(0);
            break;
        case 18 :
            ResourceManager::getInstance()->free(image);
            map->addObject(new Pnj(29*16 + 4, 5*16 - 3, 29, 310));
            image = ResourceManager::getInstance()->loadImage("data/images/npcs/personnage29.png", true);
            image->setAlpha(0);
            break;
        case 20 : case 21 : case 22 : case 23 : case 24 :
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo(50*16, 29*16+8, 1);
            ResourceManager::getInstance()->free(image);
            image = ResourceManager::getInstance()->loadImage("data/images/objects/dalle.png", false);
            image->setAlpha(255);
            break;
        case 25 : case 26 : case 27 : case 28 :
            break;
        case 29 :
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo(70*16, 187*16+8, 1);
            break;
        case 30 :
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo((80+10)*16, (150+7)*16+8, 1);
            break;
        case 31 :
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo((80+10)*16, (105+7)*16+8, 1);
            break;
        case 32 :
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo((60+10)*16, (90+7)*16+8, 1);
            break;
        case 33 :
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo((100+10)*16, (45+7)*16+8, 1);
            break;
        case 34 :
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo((60+10)*16, (15+7)*16+8, 1);
            break;
        case 35 :
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo((0+10)*16, (15+7)*16+8, 1);
            break;
        case 38 :
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo((20+10)*16, (15+7)*16+8, 1);
            break;
        case 41 :
            lcamera = true;
            camera->setManuel(true);
            camera->moveTo(10*16, (15+7)*16+8, 1);
            break;
        case 42 :
            navi->setFollowLink(false);
            lmap = true;
            llink = true;
            lnavi = true;
            break;
        case 43 : case 44 :
            link->setAnimation(IDLE);
            link->setDirection(S);
            lmap = true;
            llink = true;
            lnavi = true;
            break;
        case 45 :
            AudioManager::getInstance()->stopMusic();
            llink = true;
            lnavi = true;
            transition.start();
            break;
        case 46 : case 47 : case 48 :
            llink = true;
            lnavi = true;
            AudioManager::getInstance()->playSound(TS_NAVY_LISTEN);
            break;
        case 49 :
            AudioManager::getInstance()->stopMusic();
            lmap = true;
            llink = true;
            lnavi = true;
            MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->getFinalBoss()->stop();
            delete surface;
            surface = new Surface(320, 240);
            surface->toImage()->setAlpha(0);
            ResourceManager::getInstance()->free(image);
            image = ResourceManager::getInstance()->loadImage("data/images/projectiles/foudre.png", true);
            image->setAlpha(255);
            transition.start();
            break;
        default : break;
    }

    running = true;
    anim = 0;
}

void AnimationInGame::loop() {
    if (!running) {
        return;
    }

    switch (id) {
        case 1 :
            if (navi->getX() > 16 * 88) {
                navi->setX(navi->getX() - 1);
            } else {
                anim++;
                if (anim == 10) {
                    AudioManager::getInstance()->playSound(TS_NAVY_HELLO);
                } else if (anim == 150) {
                    AudioManager::getInstance()->playSound(TS_NAVY_HEY);
                } else if (anim == 250) {
                    AudioManager::getInstance()->playSound(TS_MAGIC);
                    MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->addEffect(
                            new NaviMagic(16 * 87 + 8, 16 * 48 + 12));
                    link->getStatus()->updateLife(6);
                    navi->setFollowLink(true);
                    AudioManager::getInstance()->stopMusic();
                } else if (anim == 300) {
                    MainController::getInstance()->getGameController()->displayText(54);
                    MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_NAVI_IS_HERE);
                    running = false;
                }
            }
            break;
        case 2 :
            anim++;
            if (anim == 25) {
                int i = link->getStatus()->getNbDeaths();
                int text = 59;
                if (i == 0) {
                    text = 57;
                } else if (i < 5) {
                    text = 58;
                }
                MainController::getInstance()->getGameController()->displayText(text);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_NAVI_IS_MOVABLE);
                running = false;
            }
            break;
        case 3 :
            anim++;
            if (anim == 25) {
                int i = link->getStatus()->getNbDeaths();
                int text = 62;
                if (i == 0) {
                    text = 60;
                } else if (i < 5) {
                    text = 61;
                }
                MainController::getInstance()->getGameController()->displayText(text);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_NAVI_CAN_HEAL);
                running = false;
            }
            break;
        case 4 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(64);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_VILLAGE_OK);
                running = false;
            }
            break;
        case 5 :
            anim++;
            if (anim == 25) {
                int i = link->getStatus()->getNbDeaths();
                int text = 92;
                if (i == 0) {
                    text = 90;
                } else if (i < 5) {
                    text = 91;
                }
                MainController::getInstance()->getGameController()->displayText(text);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_NAVI_RESTE);
                running = false;
            }
            break;
        case 6 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(144);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_PRESENTATIONS_DONE);
                running = false;
            }
            break;
        case 7 :
            anim++;
            if (anim == 25) {
                int i = link->getStatus()->getNbDeaths();
                int text = 148;
                if (i == 0) {
                    text = 146;
                } else if (i < 15) {
                    text = 147;
                }
                MainController::getInstance()->getGameController()->displayText(text);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_NAVI_QUEST_ON);
                running = false;
            }
            break;
        case 8 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(162);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_NAVI_IMPATIENTE);
                running = false;
            }
            break;
        case 9 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(213);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_SONG_FOUND);
                running = false;
            }
            break;
        case 10 : case 14 : case 15 : case 16 : case 17 : case 18 :
            anim += 5;
            image->setAlpha(anim);
            if (anim == 255) {
                running = false;
            }
            break;
        case 11 :
            if (camera->getBoundingBox()->getY() <= 20*16) {
                anim++;
                if (anim == 10) {
                    AudioManager::getInstance()->playSound(TS_MAGIC);
                    map->addEffect(new NaviMagic(40 * 16, 28 * 16));
                }
                if (anim == 20) {
                    map->setSol(39*16, 26*16, 824);
                    map->setSol(40*16, 26*16, 826);
                }
                if (anim == 25) {
                    map->setSol(39*16, 27*16, 322);
                    map->setSol(40*16, 27*16, 323);
                }
                if (anim == 30) {
                    map->setSol(39*16, 28*16, 704);
                    map->setSol(40*16, 28*16, 705);
                }
                if (anim == 35) {
                    map->setSol(39*16, 29*16, 35);
                    map->setSol(40*16, 29*16, 35);
                }
                if (anim == 50) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 150) {
                    MainController::getInstance()->getGameController()->getSceneController()->getScene()->setCoffre(0, 2, 1);
                    map->ouvrePorte(39, 27, 0);
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 12 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(293);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_TUTO_STARTED);
                running = false;
            }
            break;
        case 13 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(295);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_TUTO_DONE);
                running = false;
            }
            break;
        case 19 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(445);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->getPrimes()->stop(true);
                running = false;
            }
            break;
        case 20 : case 21 : case 22 : case 23 : case 24 :
            if (camera->getBoundingBox()->getX() == 40 * 16
                && camera->getBoundingBox()->getY() == 22 * 16) {
                anim++;
                if (anim == 100) {
                    AudioManager::getInstance()->playSound(TS_BOOM);
                    map->ouvrePorte(49, 32 - (id - 20) * 2, 2);
                }
                if (anim == 120 && id == 24) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 180 || (anim > 120 && id != 24)) {
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 25 :
            anim++;
            if (anim == 5) {
                map->setSol(28*16, 106*16, 2231);
                map->setSol(29*16, 106*16, 2232);
            }
            if (anim == 15) {
                map->setSol(28*16, 107*16, 1154);
                map->setSol(29*16, 107*16, 1154);
            }
            if (anim == 65) {
                map->setSol(28*16, 106*16, 2233);
                map->setSol(29*16, 106*16, 2234);
                map->setSol(28*16, 107*16, 2235);
                map->setSol(29*16, 107*16, 2236);
                camera->setBounds(120*16,150*16,320,240);
                camera->setBox(120*16,150*16,320,240);
                link->setX(link->getX() + 100 * 16);
                link->setY(link->getY() + 45 * 16);
                navi->setX(navi->getX() + 100 * 16);
                navi->setY(navi->getY() + 45 * 16);
                map->launchRoom();
            }
            if (anim == 100) {
                camera->setBounds(160*16,105*16,320,240);
                camera->setBox(160*16,105*16,320,240);
                link->setX(link->getX() + 40 * 16);
                link->setY(link->getY() - 45 * 16);
                navi->setX(navi->getX() + 40 * 16);
                navi->setY(navi->getY() - 45 * 16);
                link->startNewRoom();
                map->launchRoom();
                AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
            }
            if (anim >= 150) {
                running = false;
            }
            break;
        case 26 :
            anim++;
            if (anim == 5) {
                map->setSol(168*16, 106*16, 2233);
                map->setSol(169*16, 106*16, 2234);

                map->setSol(128*16, 151*16, 2233);
                map->setSol(129*16, 151*16, 2234);
                map->setSol(128*16, 152*16, 2245);
                map->setSol(129*16, 152*16, 2246);
                map->setSol(128*16, 153*16, 2235);
                map->setSol(129*16, 153*16, 2236);
            }
            if (anim == 15) {
                map->setSol(168*16, 107*16, 2245);
                map->setSol(169*16, 107*16, 2246);
            }
            if (anim == 25) {
                map->setSol(168*16, 108*16, 2251);
                map->setSol(169*16, 108*16, 2252);
            }
            if (anim == 65) {
                map->setSol(168*16, 106*16, 2231);
                map->setSol(169*16, 106*16, 2232);
                map->setSol(168*16, 107*16, 2201);
                map->setSol(169*16, 107*16, 2201);
                map->setSol(168*16, 108*16, 2202);
                map->setSol(169*16, 108*16, 2202);
                camera->setBounds(120*16,150*16,320,240);
                camera->setBox(120*16,150*16,320,240);
                link->setX(link->getX() - 40 * 16);
                link->setY(link->getY() + 45 * 16);
                navi->setX(navi->getX() - 40 * 16);
                navi->setY(navi->getY() + 45 * 16);
                map->launchRoom();
            }
            if (anim == 100) {
                map->setSol(128*16, 151*16, 2231);
                map->setSol(129*16, 151*16, 2232);
                map->setSol(128*16, 152*16, 2201);
                map->setSol(129*16, 152*16, 2201);
                map->setSol(128*16, 153*16, 1154);
                map->setSol(129*16, 153*16, 1154);
                camera->setBounds(20*16,105*16,320,240);
                camera->setBox(20*16,105*16,320,240);
                link->setX(link->getX() - 100 * 16);
                link->setY(link->getY() - 45 * 16);
                navi->setX(navi->getX() - 100 * 16);
                navi->setY(navi->getY() - 45 * 16);
                link->startNewRoom();
                map->launchRoom();
                AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
            }
            if (anim >= 150) {
                running = false;
            }
            break;
        case 27 :
            anim++;
            if (anim == 5) {
                map->setSol(107*16, 1*16, 2231);
                map->setSol(108*16, 1*16, 2232);

                map->setSol(111*16, 1*16, 2231);
                map->setSol(112*16, 1*16, 2232);
            }
            if (anim == 15) {
                map->setSol(107*16, 2*16, 1154);
                map->setSol(108*16, 2*16, 1154);

                map->setSol(111*16, 2*16, 1154);
                map->setSol(112*16, 2*16, 1154);
            }
            if (anim == 65) {
                map->setSol(107*16, 1*16, 2233);
                map->setSol(108*16, 1*16, 2234);
                map->setSol(107*16, 2*16, 2235);
                map->setSol(108*16, 2*16, 2236);

                map->setSol(111*16, 1*16, 2233);
                map->setSol(112*16, 1*16, 2234);
                map->setSol(111*16, 2*16, 2235);
                map->setSol(112*16, 2*16, 2236);

                camera->setBounds(120*16,165*16,320,240);
                camera->setBox(120*16,165*16,320,240);
                link->setX(link->getX() + 20 * 16);
                link->setY(link->getY() + 165 * 16);
                navi->setX(navi->getX() + 20 * 16);
                navi->setY(navi->getY() + 165 * 16);
                map->launchRoom();
            }
            if (anim == 100) {
                camera->setBounds(240*16,0,320,240);
                camera->setBox(240*16,0,320,240);
                link->setX(link->getX() + 120 * 16);
                link->setY(link->getY() - 165 * 16);
                navi->setX(navi->getX() + 120 * 16);
                navi->setY(navi->getY() - 165 * 16);
                link->startNewRoom();
                map->launchRoom();
                AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
            }
            if (anim >= 150) {
                running = false;
            }
            break;
        case 28 :
            anim++;
            if (anim == 5) {
                map->setSol(247*16, 1*16, 2233);
                map->setSol(248*16, 1*16, 2234);

                map->setSol(251*16, 1*16, 2233);
                map->setSol(252*16, 1*16, 2234);

                map->setSol(127*16, 166*16, 2233);
                map->setSol(128*16, 166*16, 2234);
                map->setSol(127*16, 167*16, 2245);
                map->setSol(128*16, 167*16, 2246);
                map->setSol(127*16, 168*16, 2235);
                map->setSol(128*16, 168*16, 2236);

                map->setSol(131*16, 166*16, 2233);
                map->setSol(132*16, 166*16, 2234);
                map->setSol(131*16, 167*16, 2245);
                map->setSol(132*16, 167*16, 2246);
                map->setSol(131*16, 168*16, 2235);
                map->setSol(132*16, 168*16, 2236);
            }
            if (anim == 15) {
                map->setSol(247*16, 2*16, 2245);
                map->setSol(248*16, 2*16, 2246);

                map->setSol(251*16, 2*16, 2245);
                map->setSol(252*16, 2*16, 2246);
            }
            if (anim == 25) {
                map->setSol(247*16, 3*16, 2251);
                map->setSol(248*16, 3*16, 2252);

                map->setSol(251*16, 3*16, 2251);
                map->setSol(252*16, 3*16, 2252);
            }
            if (anim == 65) {
                map->setSol(247*16, 1*16, 2231);
                map->setSol(248*16, 1*16, 2232);
                map->setSol(247*16, 2*16, 2201);
                map->setSol(248*16, 2*16, 2201);
                map->setSol(247*16, 3*16, 2202);
                map->setSol(248*16, 3*16, 2202);

                map->setSol(251*16, 1*16, 2231);
                map->setSol(252*16, 1*16, 2232);
                map->setSol(251*16, 2*16, 2201);
                map->setSol(252*16, 2*16, 2201);
                map->setSol(251*16, 3*16, 2202);
                map->setSol(252*16, 3*16, 2202);

                camera->setBounds(120*16,165*16,320,240);
                camera->setBox(120*16,165*16,320,240);
                link->setX(link->getX() - 120 * 16);
                link->setY(link->getY() + 165 * 16);
                navi->setX(navi->getX() - 120 * 16);
                navi->setY(navi->getY() + 165 * 16);
                map->launchRoom();
            }
            if (anim == 100) {
                map->setSol(127*16, 166*16, 2231);
                map->setSol(128*16, 166*16, 2232);
                map->setSol(127*16, 167*16, 2201);
                map->setSol(128*16, 167*16, 2201);
                map->setSol(127*16, 168*16, 1154);
                map->setSol(128*16, 168*16, 1154);

                map->setSol(131*16, 166*16, 2231);
                map->setSol(132*16, 166*16, 2232);
                map->setSol(131*16, 167*16, 2201);
                map->setSol(132*16, 167*16, 2201);
                map->setSol(131*16, 168*16, 1154);
                map->setSol(132*16, 168*16, 1154);

                camera->setBounds(100*16,0,320,240);
                camera->setBox(100*16,0,320,240);
                link->setX(link->getX() - 20 * 16);
                link->setY(link->getY() - 165 * 16);
                navi->setX(navi->getX() - 20 * 16);
                navi->setY(navi->getY() - 165 * 16);
                link->startNewRoom();
                map->launchRoom();
                AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
            }
            if (anim >= 150) {
                running = false;
            }
            break;
        case 29 :
            if (camera->getBoundingBox()->getX() == 60 * 16
                && camera->getBoundingBox()->getY() == 180 * 16) {
                anim++;
                if (anim == 20) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    map->ouvrePorte(69, 178, 0);
                }
                if (anim == 50) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 110) {
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 30 :
            if (camera->getBoundingBox()->getX() == 80 * 16
                && camera->getBoundingBox()->getY() == 150 * 16) {
                anim++;
                if (anim == 20) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    map->ouvrePorte(78, 156, 1);
                }
                if (anim == 50) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 110) {
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 31 :
            if (camera->getBoundingBox()->getX() == 80 * 16
                && camera->getBoundingBox()->getY() == 105 * 16) {
                anim++;
                if (anim == 20) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    map->ouvrePorte(78, 111, 1);
                }
                if (anim == 50) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 110) {
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 32 :
            if (camera->getBoundingBox()->getX() == 60 * 16
                && camera->getBoundingBox()->getY() == 90 * 16) {
                anim++;
                if (anim == 20) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    map->ouvrePorte(69, 88, 0);
                }
                if (anim == 50) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 110) {
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 33 :
            if (camera->getBoundingBox()->getX() == 100 * 16
                && camera->getBoundingBox()->getY() == 45 * 16) {
                anim++;
                if (anim == 20) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    map->ouvrePorte(109, 58, 0);
                }
                if (anim == 50) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 110) {
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 34 :
            if (camera->getBoundingBox()->getX() == 60 * 16
                && camera->getBoundingBox()->getY() == 15 * 16) {
                anim++;
                if (anim == 20) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    map->ouvrePorte(69, 28, 0);
                }
                if (anim == 50) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 110) {
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 35 :
            if (camera->getBoundingBox()->getX() == 0
                && camera->getBoundingBox()->getY() == 15 * 16) {
                anim++;
                if (anim == 20) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    map->ouvrePorte(9, 13, 0);
                }
                if (anim == 50) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 110) {
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 36 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(625);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_TUTO_LAPIN_DONE);
                running = false;
            }
            break;
        case 37 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(627);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_TUTO_PERLE_LUNE);
                running = false;
            }
            break;
        case 38 :
            if (camera->getBoundingBox()->getX() == 20 * 16
                && camera->getBoundingBox()->getY() == 15 * 16) {
                anim++;
                if (anim == 20) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    map->ouvrePorte(29, 28, 0);
                }
                if (anim == 50) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 110) {
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 39 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(629);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_TUTO_SS1_DONE);
                running = false;
            }
            break;
        case 40 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(630);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_TUTO_SS2_DONE);
                running = false;
            }
            break;
        case 41 :
            if (camera->getBoundingBox()->getX() == 0
                && camera->getBoundingBox()->getY() == 15 * 16) {
                anim++;
                if (anim == 20) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    map->ouvrePorte(9, 28, 0);
                }
                if (anim == 50) {
                    AudioManager::getInstance()->playSound(TS_SURPRISE, 2);
                }
                if (anim > 110) {
                    camera->setManuel(false);
                    running = false;
                }
            }
            break;
        case 42 :
            if (navi->getY() != 16 * 4) {
                if (navi->getY() > 16 * 4) {
                    navi->setY(navi->getY() - 1);
                    navi->setDirection(N);
                } else {
                    navi->setY(navi->getY() + 1);
                    navi->setDirection(S);
                }
            } else if (navi->getX() != 16 * 49 + 8 + 5) {
                if (navi->getX() > 16 * 49 + 8 + 5) {
                    navi->setX(navi->getX() - 1);
                    navi->setDirection(W);
                } else {
                    navi->setX(navi->getX() + 1);
                    navi->setDirection(E);
                }
            } else {
                navi->setDirection(S);
                MainController::getInstance()->getGameController()->displayText(632);
                navi->setFollowLink(true);
                running = false;
            }
            break;
        case 43 :
            anim++;
            if (anim == 25) {
                link->setAnimation(IDLE);
                link->setDirection(S);
                MainController::getInstance()->getGameController()->displayText(645);
                running = false;
            }
            break;
        case 44 :
            anim++;
            if (anim == 25) {
                link->setAnimation(IDLE);
                link->setDirection(S);
                MainController::getInstance()->getGameController()->displayText(646);
                running = false;
            }
            break;
        case 45 :
            if (transition.isRunning()) {
                transition.loop();

                if (transition.isWaiting()) {
                    anim++;
                    if (anim == 25) {
                        AudioManager::getInstance()->playSound(TS_NAVY_LISTEN);
                    } if (anim == 50) {
                        transition.restart();
                        link->getStatus()->setInvincible();
                    }
                } else if (transition.isAchieved()) {
                    transition.reset();
                    MainController::getInstance()->getGameController()->displayText(647);
                    AudioManager::getInstance()->playMusic(38);
                    running = false;
                }
            }
            break;
        case 46 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(648);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_FINAL_TUTO_RDC_DONE);
                running = false;
            }
            break;
        case 47 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(652);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_FINAL_TUTO_SS1_DONE);
                running = false;
            }
            break;
        case 48 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(653);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setAvancement(AV_FINAL_TUTO_SS2_DONE);
                running = false;
            }
            break;
        case 49 :
            anim ++;
            if (anim <= 128) {
                surface->toImage()->setAlpha(anim);
            } else if (anim == 200) {
                AudioManager::getInstance()->playSound(TS_MAGIC);
            } else if (anim == 215) {
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->getFinalBoss()->ulti();
                AudioManager::getInstance()->playMusic(38);
                running = false;
            }
            break;
        case 50 :
            anim++;
            if (anim == 25) {
                MainController::getInstance()->getGameController()->displayText(654);
                running = false;
            }
            break;
        default : running = false; break;
    }
}

void AnimationInGame::draw() {
    if (!running) {
        return;
    }

    switch (id) {
        case 20 : case 21 : case 22 : case 23 : case 24 :
            if (camera->getBoundingBox()->getX() == 40 * 16
                && camera->getBoundingBox()->getY() == 22 * 16) {
                if (anim < 100) {
                    WindowManager::getInstance()->draw(image, 0, 0, 32, 32, 9*16, 10*16 - 32 * (id - 20) - 400 + anim*4);
                }
                if (anim >= 100) {
                    WindowManager::getInstance()->draw(image, 0, 0, 32, 32, 9*16, 10*16 - 32 * (id - 20));
                }
            }
            break;
        case 49 :
            WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);
            if (anim >= 200) {
                int tmpx = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->getFinalBoss()->getX() + 39 - 180*16;
                int tmpy = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->getFinalBoss()->getY() + 43 - 15*16;

                int dstX = tmpx - 5;
                int dstY = tmpy - 112;

                int step = 0;
                if (anim > 205 && anim <= 210) {
                    step = 1;
                } else if (anim > 210) {
                    step = 2;
                }
                WindowManager::getInstance()->draw(image, step*64, 0, 64, 112, dstX, dstY);
                if (dstY > 0) {
                    dstX += 48;
                    dstY -= 112;
                    WindowManager::getInstance()->draw(image, step*64, 0, 64, 112, dstX, dstY);
                }
            }
            break;
    }
}

void AnimationInGame::drawAfterHud() {
    if (!running) {
        return;
    }

    switch (id) {
        case 45 :
            transition.draw();
            break;
    }
}

int AnimationInGame::getSpecialMusicId() {
    if (!running) {
        return 0;
    }

    switch (id) {
        case 1 : return 2;
        default : return 0;
    }
}

bool AnimationInGame::isRunning() {
    return running;
}

bool AnimationInGame::loopMap() {
    return !running || lmap;
}

bool AnimationInGame::loopCamera() {
    return !running || lcamera;
}

bool AnimationInGame::loopLink() {
    return !running || llink;
}

bool AnimationInGame::loopNavi() {
    return !running || lnavi;
}
