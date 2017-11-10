#include "Menu.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"
#include "../../../engine/texts/TextManager.h"

#include "../../MainController.h"

Menu::Menu() : i(0), j(0), step(MENU_OFF), anim(0), link(0), nbQuartCoeur(0), nbGemme(0), nbTotalGemme(0),
        allGemme(2), displayMedaillons(false), force(0), defense(0), tunique(0), epee(0), bouclier(0),
        maxRupees(0), coquillages(0), hasCoquillage(0), navi(0), maxFleches(0), fleches(0), recupFleches(0),
        maxBombes(0), bombes(0), recupBombes(0), naviHeal(0), naviAtt(0), naviDef(0) {
    imageMenu = ResourceManager::getInstance()->loadImage("data/images/status/bord.png", true);
    imageCoeur = ResourceManager::getInstance()->loadImage("data/images/status/coeur.png", true);
    imageChiffres = ResourceManager::getInstance()->loadImage("data/images/status/chiffres.png", true);
    imageMedaillons = ResourceManager::getInstance()->loadImage("data/images/status/medaillons.png", true);
    imageStuff = ResourceManager::getInstance()->loadImage("data/images/status/stuff.png", true);
    imageStatus = ResourceManager::getInstance()->loadImage("data/images/status/status.png", true);
    metronome = new Metronome(2, ANIM_REFRESH * 3);
    metronome->reset();
}

Menu::~Menu() {
    ResourceManager::getInstance()->free(imageMenu);
    ResourceManager::getInstance()->free(imageCoeur);
    ResourceManager::getInstance()->free(imageChiffres);
    ResourceManager::getInstance()->free(imageMedaillons);
    ResourceManager::getInstance()->free(imageStuff);
    ResourceManager::getInstance()->free(imageStatus);
    delete metronome;
}

void Menu::init() {
    i = 0;
    j = 0;
    step = MENU_OFF;
    anim = 0;
    //updateSelectedStuff();
}

void Menu::handleActions(Action* action) {
    if (action->isAction(ENTER)) {
        stop();
        return;
    }
    if (!link->isAbleToChangeStuff() || step != MENU_ON) {
        return;
    }

    bool hasMoved = false;
    if (action->isAction(PUSH_UP)) {
        if (--j < 0) j = 3;
        hasMoved = true;
    }
    if (action->isAction(PUSH_DOWN)) {
        if (++j > 3) j = 0;
        hasMoved = true;
    }
    if (action->isAction(PUSH_LEFT)) {
        if (--i < 0) i = 3;
        hasMoved = true;
    }
    if (action->isAction(PUSH_RIGHT)) {
        if (++i > 3) i = 0;
        hasMoved = true;
    }

    if (hasMoved) {
        metronome->reset();
        updateSelectedStuff();
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
}

void Menu::loop() {
    if (step == MENU_START) {
        if (++anim == 8) {
            step = MENU_ON;
            metronome->reset();
        }
    } else if (step == MENU_END) {
        if (--anim == 0) {
            step = MENU_OFF;
            MainController::getInstance()->getGameController()->setStep(GAME_MAIN);
        }
    }
    metronome->loop();
}

void Menu::draw() {

    int df = 200 - anim * 25;

    drawObjects(df);

    drawObjectName(df);

    drawHearts(df);

    drawGemms(df);

    drawMedaillons(df);

    drawStuff(df);

}

void Menu::drawObjects(int df) {

    drawCadre(16 - df, 16, 16*9, 16*9);

    TextManager::getInstance()->drawLetter('X', 16 + 4 - df, 16 + 4, 0);

    if (link->getInventory()->hasObject(ARC_FEE)) {
        link->getInventory()->draw(ARC_FEE, 32 - df, 32);
    } else if (link->getInventory()->hasObject(ARC)) {
        link->getInventory()->draw(ARC, 32 - df, 32);
    }

    if (link->getInventory()->hasObject(BOOMERANG_FEE)) {
        link->getInventory()->draw(BOOMERANG_FEE, 64 - df, 32);
    } else if (link->getInventory()->hasObject(BOOMERANG)) {
        link->getInventory()->draw(BOOMERANG, 64 - df, 32);
    }

    if (link->getInventory()->hasObject(GRAPPIN)) {
        link->getInventory()->draw(GRAPPIN, 96 - df, 32);
    }

    if (link->getInventory()->hasObject(BOMBES) /*&& link->getStatus()->getBombs()*/) {
        link->getInventory()->draw(BOMBES, 128 - df, 32);
    }

    if (link->getInventory()->hasObject(BAGUETTE_FEU)) {
        link->getInventory()->draw(BAGUETTE_FEU, 32 - df, 64);
    }

    if (link->getInventory()->hasObject(BAGUETTE_GLACE)) {
        link->getInventory()->draw(BAGUETTE_GLACE, 64 - df, 64);
    }

    if (link->getInventory()->hasObject(LANTERNE)) {
        link->getInventory()->draw(LANTERNE, 96 - df, 64);
    }

    if (link->getInventory()->hasObject(CAPE_INVISIBILITE)) {
        link->getInventory()->draw(CAPE_INVISIBILITE, 128 - df, 64);
    }

    if (link->getInventory()->hasObject(MARTEAU)) {
        link->getInventory()->draw(MARTEAU, 32 - df, 96);
    }

    if (link->getInventory()->hasObject(OCARINA)) {
        link->getInventory()->draw(OCARINA, 64 - df, 96);
    }

    if (link->getInventory()->hasObject(CANNE_SOMARIA)) {
        link->getInventory()->draw(CANNE_SOMARIA, 96 - df, 96);
    }

    if (link->getInventory()->hasObject(CANNE_BYRNA)) {
        link->getInventory()->draw(CANNE_BYRNA, 128 - df, 96);
    }

    if (link->getInventory()->hasObject(FLACON_1)) {
        link->getInventory()->draw(FLACON_1, 32 - df, 128);
    }

    if (link->getInventory()->hasObject(FLACON_2)) {
        link->getInventory()->draw(FLACON_2, 64 - df, 128);
    }

    if (link->getInventory()->hasObject(FLACON_3)) {
        link->getInventory()->draw(FLACON_3, 96 - df, 128);
    }

    if (link->getInventory()->hasObject(FLACON_4)) {
        link->getInventory()->draw(FLACON_4, 128 - df, 128);
    }

    if (df == 0 && !metronome->getValue()) {
        if (link->isAbleToChangeStuff()) {
            WindowManager::getInstance()->draw(imageMenu, 51, 3, 26, 26, 32 - 5 + i * 32, 32 - 5 + j * 32);
        } else {
            WindowManager::getInstance()->draw(imageMenu, 83, 3, 26, 26, 32 - 5 + i * 32, 32 - 5 + j * 32);
        }
    }
}

void Menu::drawObjectName(int df) {
    drawCadre(16 * 11, 16 - df, 16 * 8, 16 * 2);

    Equipment e = link->getInventory()->getCurrent();

    int dstX = 16 * 11 + 8;
    int dstY = 16 + 8 - df;
    int text = -1;
    if (link->getInventory()->hasObject(e)) {
        switch (e) {
            case ARC : text = 16; break;
            case ARC_FEE : text = 17; break;
            case BOOMERANG : text = 18; break;
            case BOOMERANG_FEE : text = 19; break;
            case GRAPPIN : text = 20; break;
            case BOMBES : text = 21; break;
            case BAGUETTE_FEU : text = 22; break;
            case BAGUETTE_GLACE : text = 23; break;
            case LANTERNE : text = 24; break;
            case CAPE_INVISIBILITE : text = 25; break;
            case MARTEAU : text = 26; break;
            case OCARINA : text = 27; break;
            case CANNE_SOMARIA : text = 28; break;
            case CANNE_BYRNA : text = 29; break;
            case FLACON_1 :
            case FLACON_2 :
            case FLACON_3 :
            case FLACON_4 :
                text = 30 + (link->getInventory()->getCurrentFlacon() % 4) + 461 * (link->getInventory()->getCurrentFlacon() / 4);
                break;
            default : break;
        }
        if (text != -1) {
            TextManager::getInstance()->getText(text)->display(dstX, dstY);
        }
    }
}

void Menu::drawHearts(int df) {
    drawCadre(16 * 16, 16 * 11 + df, 16*3, 16*3);
    WindowManager::getInstance()->draw(imageCoeur, nbQuartCoeur * 16, 0, 16, 16, 16 * 17, 16 * 12 + df);
}

void Menu::drawGemms(int df) {
    drawCadre(16 - df, 16 * 11, 16*3, 16*3);
    if (nbTotalGemme) {
        WindowManager::getInstance()->draw(imageStuff, 64, 48, 16, 16, 32 - df, 16 * 12 - 4);
        drawNombreTricolore(nbGemme, (nbGemme == 100 ? 28 : 32) - df, 16 * 13, nbGemme == 100 ? 3 : 2, allGemme);
    }
}

void Menu::drawMedaillons(int df) {
    drawCadre(16 * 5, 16 * 11 + df, 16*10, 16*3);

    if (displayMedaillons) {
        for (int i = 0; i < NB_MEDAILLONS; i++) {
            if (link->getInventory()->hasMedaillons(i)) {
                WindowManager::getInstance()->draw(imageMedaillons, i * 16, 0, 16, 16, 16 * 5 + 9 + i * 21, 16 * 12 + df);
            } else {
                WindowManager::getInstance()->draw(imageMedaillons, 16 * 7, 0, 16, 16, 16 * 5 + 9 + i * 21, 16 * 12 + df);
            }
        }
    }
}

void Menu::drawStuff(int df) {
    drawCadre(16 * 11 + df, 16 * 4, 16 * 8, 16 * 6);

    // line 1
    // force
    WindowManager::getInstance()->draw(imageStuff, 80, 40, 12, 12, 16 * 11 + 5 + df, 16 * 4 + 5);
    drawNombre(force, 16 * 11 + 15 + df, 16 * 4 + 8, force >= 10 ? 2 : 1, force >= 10);

    // defense
    WindowManager::getInstance()->draw(imageStuff, 80, 28, 12, 12, 16 * 11 + 31 + df, 16 * 4 + 5);
    drawNombre(defense, 16 * 11 + 42 + df, 16 * 4 + 8, defense >= 10 ? 2 : 1, defense >= 10);

    // time
    WindowManager::getInstance()->draw(imageStuff, 80, 16, 12, 12, 16 * 11 + 58 + df, 16 * 4 + 5);
    int time = MainController::getInstance()->getGameController()->getTime();
    int hours = time / 3600;
    int minutes = (time - hours * 3600) / 60;
    int seconds = time % 60;
    bool green = time == 359999;
    drawNombre(hours, 16 * 11 + 69 + df, 16 * 4 + 8, 2, green);
    WindowManager::getInstance()->draw(imageChiffres, 40, green ? 16 : 0, 4, 8, 16 * 11 + 69 + 15 + df, 16 * 4 + 8);
    drawNombre(minutes, 16 * 11 + 87 + df, 16 * 4 + 8, 2, green);
    WindowManager::getInstance()->draw(imageChiffres, 40, green ? 16 : 0, 4, 8, 16 * 11 + 87 + 15 + df, 16 * 4 + 8);
    drawNombre(seconds, 16 * 11 + 105 + df, 16 * 4 + 8, 2, green);

    // line 2
    int dy = 16 * 5 + 8 - 4;
    // navi
    int dx = 16 * 11 + 8 + 2-4;
    if (navi) {
        WindowManager::getInstance()->draw(imageStuff, 96, 0, 10, 9, dx + df, dy + 2);
        drawNombre(navi, 16 * 11 + 15 + df, dy + 6, 1, navi == 5);
    }
    // heal
    dx += 18;
    if (link->getInventory()->hasObject(NAVI_HEAL)) {
        WindowManager::getInstance()->draw(imageStatus, 0, 55, 16, 16, dx + df, dy);
        if (naviHeal > 1) {
            WindowManager::getInstance()->draw(imageStatus, 48 + (naviHeal - 2) * 5, 55, 5, 5, dx + 11 + df, dy + 10);
        }
    }
    // att
    dx += 18;
    if (link->getInventory()->hasObject(NAVI_ATT)) {
        WindowManager::getInstance()->draw(imageStatus, 16, 55, 16, 16, dx + df, dy);
        if (naviAtt > 1) {
            WindowManager::getInstance()->draw(imageStatus, 48 + (naviAtt - 2) * 5, 55, 5, 5, dx + 11 + df, dy + 10);
        }
    }
    // def
    dx += 18;
    if (link->getInventory()->hasObject(NAVI_DEF)) {
        WindowManager::getInstance()->draw(imageStatus, 32, 55, 16, 16, dx + df, dy);
        if (naviDef > 1) {
            WindowManager::getInstance()->draw(imageStatus, 48 + (naviDef - 2) * 5, 55, 5, 5, dx + 11 + df, dy + 10);
        }
    }
    // coquillages
    dx += 18 + 8;
    if (hasCoquillage) {
        WindowManager::getInstance()->draw(imageStuff, 64, 32, 16, 16, dx + df, dy);
        drawNombre(coquillages, dx + 16 + df, dy + 6, 2, hasCoquillage == 20);
    }

    // line 3
    dy = 16 * 6 + 8 - 2;
    // bourse
    dx = 16 * 11 + 8 - 2;
    if (maxRupees == 99) {
        WindowManager::getInstance()->draw(imageStuff, 0, 48, 16, 16, dx + df, dy);
    } else if (maxRupees == 200) {
        WindowManager::getInstance()->draw(imageStuff, 16, 48, 16, 16, dx + df, dy);
    } else if (maxRupees == 500) {
        WindowManager::getInstance()->draw(imageStuff, 32, 48, 16, 16, dx + df, dy);
    } else if (maxRupees == 999) {
        WindowManager::getInstance()->draw(imageStuff, 48, 48, 16, 16, dx + df, dy);
    }
    if (maxRupees) {
        int rupees = link->getStatus()->getVirtualRupees();
        drawNombre(rupees, dx + 16 + df, dy + 6, maxRupees == 99 ? 2 : 3, rupees == maxRupees);
    }
    // bombes
    dx += 44;
    if (maxBombes) {
        WindowManager::getInstance()->draw(imageStuff, 80, 0, 16, 16, dx + df, dy);
        if (recupBombes) WindowManager::getInstance()->draw(imageStuff, 80, 52, 9, 8, dx + 16 + df, dy - 2);
        drawNombre(bombes, dx + 16 + df, dy + 6, 2, maxBombes == bombes);
    }
    // fleches
    dx += 36;
    if (maxFleches) {
        WindowManager::getInstance()->draw(imageStuff, 64, 0, 16, 16, dx + df, dy);
        if (recupFleches) WindowManager::getInstance()->draw(imageStuff, 80, 52, 9, 8, dx + 16 + df, dy - 2);
        drawNombre(fleches, dx + 16 + df, dy + 6, 2, maxFleches == fleches);
    }

    // line 4
    dy = 16 * 7 + 8;
    // tunique
    dx = 16 * 11 + 8 + 2;
    WindowManager::getInstance()->draw(imageStuff, (tunique - 1) * 16, 0, 16, 16, dx + df, dy);
    // epee
    dx += 18;
    if (epee) WindowManager::getInstance()->draw(imageStuff, (epee - 1) * 16, 16, 16, 16, dx + df, dy);
    // bouclier
    dx += 18;
    if (bouclier) WindowManager::getInstance()->draw(imageStuff, (bouclier - 1) * 16, 32, 16, 16, dx + df, dy);
    // gant
    dx += 18;
    if (link->getInventory()->hasObject(GANTS_2)) {
        link->getInventory()->draw(GANTS_2, dx + df, dy);
    } else if (link->getInventory()->hasObject(GANTS)) {
        link->getInventory()->draw(GANTS, dx + df, dy);
    }
    // bottes
    dx += 18;
    if (link->getInventory()->hasObject(BOTTES)) {
        link->getInventory()->draw(BOTTES, dx + df, dy);
    }
    // palmes
    dx += 18;
    if (link->getInventory()->hasObject(PALMES)) {
        link->getInventory()->draw(PALMES, dx + df, dy);
    }

    // line 5
    dy = 16 * 8 + 8 + 2;
    // carte
    dx = 16 * 11 + 8 + 2;
    if (link->getInventory()->hasObject(CARTE_ILE)) {
        link->getInventory()->draw(CARTE_ILE, dx + df, dy);
    }
    // detecteur
    dx += 18;
    if (link->getInventory()->hasObject(DETECTEUR)) {
        link->getInventory()->draw(DETECTEUR, dx + df, dy);
    }
    // chant 1
    dx += 18;
    if (link->getInventory()->hasObject(CHANT_1)) {
        link->getInventory()->draw(CHANT_1, dx + df, dy);
    }
    // chant 2
    dx += 18;
    if (link->getInventory()->hasObject(CHANT_2)) {
        link->getInventory()->draw(CHANT_2, dx + df, dy);
    }
    // chant 3
    dx += 18;
    if (link->getInventory()->hasObject(CHANT_3)) {
        link->getInventory()->draw(CHANT_3, dx + df, dy);
    }
    // palmes
    dx += 18;
    if (link->getInventory()->hasObject(PERLE_LUNE)) {
        link->getInventory()->draw(PERLE_LUNE, dx + df, dy);
    }
}

void Menu::drawCadre(int x, int y, int w, int h) {

    WindowManager::getInstance()->draw(imageMenu, 0, 0, 16, 16, x, y);
    WindowManager::getInstance()->draw(imageMenu, 32, 0, 16, 16, x + w - 16, y);
    WindowManager::getInstance()->draw(imageMenu, 0, 32, 16, 16, x, y + h - 16);
    WindowManager::getInstance()->draw(imageMenu, 32, 32, 16, 16, x + w - 16, y + h - 16);

    for (int i = x + 16; i < x + w - 16; i += 16) {
        WindowManager::getInstance()->draw(imageMenu, 16, 0, 16, 16, i, y);
        WindowManager::getInstance()->draw(imageMenu, 16, 32, 16, 16, i, y + h - 16);
    }

    for (int j = y + 16; j < y + h - 16; j += 16) {
        WindowManager::getInstance()->draw(imageMenu, 0, 16, 16, 16, x, j);
        WindowManager::getInstance()->draw(imageMenu, 32, 16, 16, 16, x + w - 16, j);
    }

    for (int i = x + 16; i < x + w - 16; i += 16) {
        for (int j = y + 16; j < y + h - 16; j += 16) {
            WindowManager::getInstance()->draw(imageMenu, 16, 16, 16, 16, i, j);
        }
    }

}

void Menu::drawNombre(int val, int x, int y, int nb, bool green) {
    int val2;
    int srcX;
    int srcY;
    int dstX = x;

    //si on commence par les centaines
    if (nb >= 3) {
        val2 = val / 100;
        srcX = (val2%5)*8;
        srcY = ((int)(val2/5))*8;
        if (green) srcY += 16;
        WindowManager::getInstance()->draw(imageChiffres, srcX, srcY, 8, 8, dstX, y);
        dstX += 8;
    }

    //les dizaines
    if (nb >= 2) {
        val2 = (val%100)/10;
        srcX = (val2%5)*8;
        srcY = ((int)(val2/5))*8;
        if (green) srcY += 16;
        WindowManager::getInstance()->draw(imageChiffres, srcX, srcY, 8, 8, dstX, y);
        dstX += 8;
    }

    //unitées
    val2 = val%10;
    srcX = (val2%5)*8;
    srcY = ((int)(val2/5))*8;
    if (green) srcY += 16;
    WindowManager::getInstance()->draw(imageChiffres, srcX, srcY, 8, 8, dstX, y);
}

void Menu::drawNombreTricolore(int val, int x, int y, int nb, int color) {
    int val2;
    int srcX;
    int srcY;
    int dstX = x;

    int offsetY = 0;
    if (color == 0) offsetY = 16;
    else if (color == 1) offsetY = 32;

    //si on commence par les centaines
    if (nb >= 3) {
        val2 = val / 100;
        srcX = (val2%5)*8;
        srcY = ((int)(val2/5))*8;
        WindowManager::getInstance()->draw(imageChiffres, srcX, srcY + offsetY, 8, 8, dstX, y);
        dstX += 8;
    }

    //les dizaines
    if (nb >= 2) {
        val2 = (val%100)/10;
        srcX = (val2%5)*8;
        srcY = ((int)(val2/5))*8;
        WindowManager::getInstance()->draw(imageChiffres, srcX, srcY + offsetY, 8, 8, dstX, y);
        dstX += 8;
    }

    //unitées
    val2 = val%10;
    srcX = (val2%5)*8;
    srcY = ((int)(val2/5))*8;
    WindowManager::getInstance()->draw(imageChiffres, srcX, srcY + offsetY, 8, 8, dstX, y);
}

void Menu::start() {
    if (step == MENU_OFF) {
        link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
        AudioManager::getInstance()->playSound(TS_MENU1);
        step = MENU_START;
        anim = 0;

        nbQuartCoeur = link->getInventory()->nbQuartCoeur();
        nbTotalGemme = link->getInventory()->totalGemmeForce();
        nbGemme = nbTotalGemme ? link->getInventory()->nbGemmeForce() : 0;
        if (link->getInventory()->hasObject(DETECTEUR)) {
            allGemme = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->nbGemmeForce();
        } else {
            allGemme = 2;
        }
        displayMedaillons = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getAvancement() >= AV_MAIRE_VU;
        force = link->getForce();
        defense = link->getDefense();
        tunique = link->getTunique();
        epee = link->getEpee();
        bouclier = link->getBouclier();
        maxRupees = link->getStatus()->getMaxRupees();
        coquillages = link->getInventory()->nbCoquillages();
        hasCoquillage = link->getInventory()->totalCoquillages();

        int naviMaxMagie = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getNavi()->getMaxMagic();
        navi = (naviMaxMagie - 15) / 5;
        if (navi < 1) navi = 0;
        naviHeal = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getNavi()->getLvlHeal();
        naviAtt = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getNavi()->getLvlAtt();
        naviDef = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getNavi()->getLvlDef();

        maxFleches = link->getStatus()->getMaxArrows();
        fleches = link->getStatus()->getArrows();
        recupFleches = link->getStatus()->getRecupArrows();

        maxBombes = link->getStatus()->getMaxBombs();
        bombes = link->getStatus()->getBombs();
        recupBombes = link->getStatus()->getRecupBombs();
    }
}

void Menu::stop() {
    if (step == MENU_START || step == MENU_ON) {
        AudioManager::getInstance()->playSound(TS_MENU2);
        step = MENU_END;
    }
}

Equipment Menu::getSelectedStuff() {
    int tmp = i + j * 4;
    switch (tmp) {
        case 0 :
            if (link->getInventory()->hasObject(ARC_FEE)) return ARC_FEE;
            return ARC;
        case 1 :
            if (link->getInventory()->hasObject(BOOMERANG_FEE)) return BOOMERANG_FEE;
            return BOOMERANG;
        case 2 : return GRAPPIN;
        case 3 : return BOMBES;
        case 4 : return BAGUETTE_FEU;
        case 5 : return BAGUETTE_GLACE;
        case 6 : return LANTERNE;
        case 7 : return CAPE_INVISIBILITE;
        case 8 : return MARTEAU;
        case 9 : return OCARINA;
        case 10 : return CANNE_SOMARIA;
        case 11 : return CANNE_BYRNA;
        case 12 : return FLACON_1;
        case 13 : return FLACON_2;
        case 14 : return FLACON_3;
        case 15 : return FLACON_4;
    }
    return ARC;
}

void Menu::updateSelectedStuff() {
    MainController::getInstance()->getGameController()->getSceneController()->
        getScene()->getLink()->getInventory()->setCurrent(getSelectedStuff());
}
