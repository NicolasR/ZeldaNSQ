#include "Hud.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/texts/TextManager.h"

#include "../../MainController.h"
#include "Primes.h"

Hud::Hud(Status* status, Inventory* inventory, Navi* navi, bool isFrench) : status(status), inventory(inventory), navi(navi), french(isFrench),
animNavi(0), animBoss(0), bossLife(0), bossMaxLife(0), removeBoss(false), etage(0), displayEtg(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/status/status.png", true);
    chiffres = ResourceManager::getInstance()->loadImage("data/images/status/chiffres.png", true);
    level = ResourceManager::getInstance()->loadImage("data/images/status/level.png", true);
}

Hud::~Hud() {
    ResourceManager::getInstance()->free(image);
    ResourceManager::getInstance()->free(chiffres);
    ResourceManager::getInstance()->free(level);
}

void Hud::loop() {
    if (inventory->hasObject(NAVI_HEAL) || inventory->hasObject(NAVI_ATT) || inventory->hasObject(NAVI_DEF)) {
        if (!navi->isBoxOnLink() && animNavi < 56) {
            animNavi += 8;
        }
        if (navi->isBoxOnLink() && animNavi > 0) {
            animNavi -= 8;
        }
    }
    if (animBoss > 0 && (bossLife <= 0 || removeBoss))  {
        animBoss--;
    } else if (animBoss > 0 && animBoss < 50) {
        animBoss++;
    }

    if (displayEtg) displayEtg--;
}

void Hud::draw() {
    drawLife();
    drawMagic();
    drawItems();
    drawObject();
    drawSpells();
    drawBonus();
    drawBoss();
    drawTime();
    drawEtage();
}

void Hud::reset() {
    removeBoss = true;
}

void Hud::displayEtage() {
    displayEtg = 128;
    etage = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap()->getEtage();
}

void Hud::drawEtage() {
    if (displayEtg) {
        WindowManager::getInstance()->draw(level, french ? 0 : 32, 32 - etage * 16, 32, 16, 288, 0);
    }
}

void Hud::drawTime() {
    Primes* primes = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getPrimes();
    if (primes->isRunning() && !animBoss) {

        WindowManager::getInstance()->draw(image, 92, 55, 12, 10, 137, 216+1);
        int time = primes->getTime();
        if (time < 0) {
            time = 0;
        }
        int minutes = time / 60;
        int seconds = time % 60;
        drawNombre(minutes, -1, 150, 216+2, 2);
        WindowManager::getInstance()->draw(chiffres, 40, 0, 4, 8, 165, 216+2);
        drawNombre(seconds, -1, 168, 216+2, 2);
    }

}

void Hud::setBossLife(int life, int maxLife) {
    bossLife = life;
    bossMaxLife = maxLife;
    removeBoss = false;
    if (animBoss == 0) {
        animBoss = 1;
    }
}

void Hud::drawBoss() {
    int dx = (animNavi > 0 && inventory->hasObject(NAVI_ULT)) ? animNavi / 2 : 0;
    if (animBoss) {
        int dstY = 220 + 50 - animBoss;
        WindowManager::getInstance()->draw(image, 100, 45, 100, 10, 110 + dx, dstY);
        int srcW = (bossLife * 90) / bossMaxLife;
        WindowManager::getInstance()->draw(image, 95 - srcW, 49, srcW, 2, 115 + dx, dstY + 4);
    }
}

void Hud::drawBonus() {
    int dstX = 320 - 24;
    int dstY = 240 - 24;
    if (navi->getBonusDef()) {
        WindowManager::getInstance()->draw(image, 80, 55, 12, 12, dstX + 2, dstY);
        WindowManager::getInstance()->draw(image, 68, 67, 16, 3, dstX, dstY + 13);
        int value = (navi->getTmpDef() * 14) / navi->getMaxTmpDef();
        if (value < 14) value ++;
        WindowManager::getInstance()->draw(image, 49, 68, value, 1, dstX + 1, dstY + 14);
        dstX -= 24;
    }
    if (navi->getBonusForce()) {
        WindowManager::getInstance()->draw(image, 68, 55, 12, 12, dstX + 2, dstY);
        WindowManager::getInstance()->draw(image, 68, 67, 16, 3, dstX, dstY + 13);
        int value = (navi->getTmpForce() * 14) / navi->getMaxTmpForce();
        if (value < 14) value ++;
        WindowManager::getInstance()->draw(image, 49, 68, value, 1, dstX + 1, dstY + 14);
        dstX -= 24;
    }
    if (status->getGel()) {
        WindowManager::getInstance()->draw(image, 104, 55, 12, 12, dstX + 2, dstY);
        WindowManager::getInstance()->draw(image, 68, 67, 16, 3, dstX, dstY + 13);
        int value = (status->getGel() * 14) / status->getMaxGel();
        if (value < 14) value ++;
        WindowManager::getInstance()->draw(image, 49, 65, value, 1, dstX + 1, dstY + 14);
        dstX -= 24;
    }
}

void Hud::drawSpells() {
    if (animNavi > 0) {
        bool allSpells = inventory->hasObject(NAVI_ULT);

        WindowManager::getInstance()->draw(image, 100, 45, 80, 4, 10, 240 + 10 - animNavi);
        if (allSpells) WindowManager::getInstance()->draw(image, 105, 45, 90, 4, 30, 240 + 10 - animNavi);
        WindowManager::getInstance()->draw(image, 192, 45, 8, 4, allSpells ? 90 + 30 : 90, 240 + 10 - animNavi);

        WindowManager::getInstance()->draw(image, 212, 8, 4, 28, 10, 240 + 10 - animNavi + 4);
        WindowManager::getInstance()->draw(image, 218, 8, 4, 28, allSpells ? 94 + 30 : 94, 240 + 10 - animNavi + 4);


        WindowManager::getInstance()->draw(image, 100, 51, 80, 4, 10, 240 + 10 - animNavi + 32);
        if (allSpells) WindowManager::getInstance()->draw(image, 105, 51, 90, 4, 30, 240 + 10 - animNavi + 32);
        WindowManager::getInstance()->draw(image, 192, 51, 8, 4, allSpells ? 90 + 30 : 90, 240 + 10 - animNavi + 32);

        for (int i = 0; i < (allSpells ? 8 + 3 : 8); i++) {
            WindowManager::getInstance()->draw(image, 3, 8, 10, 28, 14 + 10 * i, 240 + 10 - animNavi + 4);
        }


        if (inventory->hasObject(NAVI_HEAL)) {
            TextManager::getInstance()->drawLetter('W', 14, 240 + 10 - animNavi + 4);
            WindowManager::getInstance()->draw(image, 0, 55, 16, 16, 22, 240 + 10 - animNavi + 8);
            if (navi->getLvlHeal() > 1) {
                WindowManager::getInstance()->draw(image, 48 + (navi->getLvlHeal() - 2) * 5, 55, 5, 5, 33, 240 + 20 - animNavi + 8);
            }
            drawJaugeMultiStyle(22, 240 + 10 - animNavi + 8 + 16, navi->getMagic(), 20);
        }

        if (inventory->hasObject(NAVI_ATT)) {
            TextManager::getInstance()->drawLetter('X', 42, 240 + 10 - animNavi + 4);
            WindowManager::getInstance()->draw(image, 16, 55, 16, 16, 50, 240 + 10 - animNavi + 8);
            if (navi->getLvlAtt() > 1) {
                WindowManager::getInstance()->draw(image, 48 + (navi->getLvlAtt() - 2) * 5, 55, 5, 5, 61, 240 + 20 - animNavi + 8);
            }
            drawJaugeMultiStyle(50, 240 + 10 - animNavi + 8 + 16, navi->getMagic(), 10);
        }

        if (inventory->hasObject(NAVI_DEF)) {
            TextManager::getInstance()->drawLetter('C', 70, 240 + 10 - animNavi + 4);
            WindowManager::getInstance()->draw(image, 32, 55, 16, 16, 78, 240 + 10 - animNavi + 8);
            if (navi->getLvlDef() > 1) {
                WindowManager::getInstance()->draw(image, 48 + (navi->getLvlDef() - 2) * 5, 55, 5, 5, 89, 240 + 20 - animNavi + 8);
            }
            drawJaugeMultiStyle(78, 240 + 10 - animNavi + 8 + 16, navi->getMagic(), 15);
        }

        if (allSpells) {
            TextManager::getInstance()->drawLetter('V', 98, 240 + 10 - animNavi + 4);
            WindowManager::getInstance()->draw(image, 116, 55, 16, 16, 106, 240 + 10 - animNavi + 8);
            drawJaugeMultiStyle(106, 240 + 10 - animNavi + 8 + 16, navi->getMagic(), 40);
        }
    }
}

void Hud::drawLife() {

    //life
    int srcY = french ? 17 : 0;
    int srcH = french ? 8 : 7;
    WindowManager::getInstance()->draw(image, 158, srcY, 44, srcH, 250, 10);

    //hearts
    int srcX = 0;
    int life = status->getLife();
    for(int i = 0; i < status->getMaxLife() / 2; i++) {
        if (life - (i * 2) > 1) srcX = 141;
        if (life - (i * 2) == 1) srcX = 149;
        if (life - (i * 2) < 1) srcX = 157;

        if (status->isOniLink()) {
            WindowManager::getInstance()->draw(image, srcX, srcX == 157 ? 9 : 18, 7, 7, 233 + ((i % 10) * 8), 19 + 8 * ((int)(i / 10)));
        } else if (status->getOniLife() - (i * 2) >= 1 && status->getVirtualLife() > 0) {
            int oni = status->getOniLife();
            if (oni - (i * 2) == 1 && oni != life) {
                WindowManager::getInstance()->draw(image, srcX, 9, 7, 7, 233 + ((i % 10) * 8), 19 + 8 * ((int)(i / 10)));
                WindowManager::getInstance()->draw(image, srcX, 18, 4, 7, 233 + ((i % 10) * 8), 19 + 8 * ((int)(i / 10)));
            } else {
                WindowManager::getInstance()->draw(image, srcX, 18, 7, 7, 233 + ((i % 10) * 8), 19 + 8 * ((int)(i / 10)));
            }
        } else {
            WindowManager::getInstance()->draw(image, srcX, 9, 7, 7, 233 + ((i % 10) * 8), 19 + 8 * ((int)(i / 10)));
        }
    }
}

void Hud::drawMagic() {

    //jauge navi
    int dstX = 10;

    if (navi->getMaxMagic()) {
        WindowManager::getInstance()->draw(image, 212, 0, 10, 45, dstX, 10);
        int h = (navi->getMagic() * 32) / navi->getMaxMagic();
        WindowManager::getInstance()->draw(image, 202, 8, 8, h, dstX, 50 - h);
        dstX += 10;
    }

    //jauge magie
    if (status->getMaxMagic() > 0) {
        WindowManager::getInstance()->draw(image, 0, 0, 16, 45, dstX, 10);

        //demi magie
        if (status->getMaxMagic() > 32) {
            WindowManager::getInstance()->draw(image, 186, 37, 16, 8, dstX, 10);
        }

        int ratio = status->getMagic() / (status->getMaxMagic() / 32);
        WindowManager::getInstance()->draw(image, 124, 8, 8, ratio, dstX + 4, 50 - ratio);
    }
}

void Hud::drawItems() {

    //rupees
    if (status->getMaxRupees() > 0) {
        WindowManager::getInstance()->draw(image, 52, 0, 8, 8, 129, 10);
        if (status->getMaxRupees() > 99) {
            drawNombre(status->getRupees(), status->getMaxRupees(), 121, 20, 3);
        } else {
            drawNombre(status->getRupees(), status->getMaxRupees(), 125, 20, 2);
        }
    }

    //bombs
    if (status->getMaxBombs() > 0) {
        WindowManager::getInstance()->draw(image, 80, 0, 8, 8, 157, 10);
        drawNombre(status->getBombs(), status->getMaxBombs(), 153, 20, 2);
    }

    //arrows
    if (status->getMaxArrows() > 0) {
        int srcY = inventory->hasObject(ARC_FEE) ? 21 : 0;
        WindowManager::getInstance()->draw(image, 101, srcY, 14, 8, 178, 10);
        drawNombre(status->getArrows(), status->getMaxArrows(), 177, 20, 2);
    }

    //keys
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    if (scene->getMap()->getId() >= 58) {
        int nbKeys = scene->getCoffre(scene->getMap()->getId() - 57, 3);
        WindowManager::getInstance()->draw(image, 50, 28, 8, 8, 89, 10);
        drawNombre(nbKeys, 99, nbKeys > 9 ? 85 : 89, 20, nbKeys > 9 ? 2 : 1);
    }
}

void Hud::drawObject() {
    if (inventory->hasObject()) {

        int dstX = 10;
        if (navi->getMaxMagic()) dstX += 10;
        if (status->getMaxMagic() > 0) dstX += 17;

        WindowManager::getInstance()->draw(image, 17, 5, 22, 22, dstX, 15);

        Equipment e = inventory->getCurrent();
        if (inventory->hasObject(e)) {
            if (e == BOMBES && !status->getBombs()) {
                return;
            }
            inventory->draw(e, dstX + 3, 18);
        }
    }
}

void Hud::drawNombre(int val, int max, int x, int y, int nb) {
    int val2;
    int srcX;
    int srcY;
    int dstX = x;

    //si on commence par les centaines
    if (nb >= 3) {
        val2 = val / 100;
        srcX = (val2%5)*8;
        srcY = ((int)(val2/5))*8;
        if (val == max) srcY += 16;
        WindowManager::getInstance()->draw(chiffres, srcX, srcY, 8, 8, dstX, y);
        dstX += 8;
    }

    //les dizaines
    if (nb >= 2) {
        val2 = (val%100)/10;
        srcX = (val2%5)*8;
        srcY = ((int)(val2/5))*8;
        if (val == max) srcY += 16;
        WindowManager::getInstance()->draw(chiffres, srcX, srcY, 8, 8, dstX, y);
        dstX += 8;
    }

    //unitées
    val2 = val%10;
    srcX = (val2%5)*8;
    srcY = ((int)(val2/5))*8;
    if (val == max) srcY += 16;
    WindowManager::getInstance()->draw(chiffres, srcX, srcY, 8, 8, dstX, y);

}

void Hud::drawJaugeMultiStyle(int x, int y, int current, int max) {
    if (current > max) current = max;
    int style = 0;
    if (current == max) style = 2;
    else if (current > max / 2) style = 1;
    WindowManager::getInstance()->draw(image, 48, 61, 16, 3, x, y);
    WindowManager::getInstance()->draw(image, 49, 65 + style, (current * 14) / max, 1, x + 1, y + 1);
}
