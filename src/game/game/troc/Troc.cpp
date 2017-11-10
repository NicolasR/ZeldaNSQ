#include "Troc.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"
#include "../../../engine/texts/TextManager.h"

#include "../../MainController.h"


Troc::Troc() : step(MENU_TROC_OFF), anim(0), inventory(0), text(0), blocs(0), offset(0) {
    imageMenu = ResourceManager::getInstance()->loadImage("data/images/status/bord.png", true);
    imageChiffres = ResourceManager::getInstance()->loadImage("data/images/status/chiffres.png", true);
}

Troc::~Troc() {
    ResourceManager::getInstance()->free(imageMenu);
    ResourceManager::getInstance()->free(imageChiffres);
}

void Troc::handleActions(Action* action) {
    if (action->isAction(ENTER) || action->isAction(TROC_ITEMS)) {
        stop();
        return;
    }
}

void Troc::loop() {
    if (step == MENU_TROC_START) {
        if (++anim == 8) {
            step = MENU_TROC_ON;
        }
    } else if (step == MENU_TROC_END) {
        if (--anim == 0) {
            step = MENU_TROC_OFF;
            MainController::getInstance()->getGameController()->setStep(GAME_MAIN);
        }
    }
}

void Troc::draw() {
    int df = 200 - anim * 25;
    drawTitre(df);
    drawObjects(df);
}

void Troc::start() {
    if (step == MENU_TROC_OFF) {
        inventory = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink()->getInventory();
        AudioManager::getInstance()->playSound(TS_MENU1);
        step = MENU_TROC_START;
        anim = 0;

        int letterSize = TextManager::getInstance()->getWSpace();
        text = TextManager::getInstance()->getText(71);
        int size = text->getLength() * letterSize;
        blocs = (size + 8) / 16 + 1;
        offset = (((blocs * 16) - size) / 2) - 1;
    }
}

void Troc::stop() {
    if (step == MENU_TROC_START || step == MENU_TROC_ON) {
        AudioManager::getInstance()->playSound(TS_MENU2);
        step = MENU_TROC_END;
    }
}

void Troc::drawTitre(int df) {
    drawCadre(16 - df, 16, 16 * blocs, 16 * 2);
    text->display(16 + offset - df, 16 + 8);
}

void Troc::drawObjects(int df) {
    drawCadre(16, 16 * 4 + df, 16 * 18, 16 * 10);

    int dx = 16 + 8;
    int dy = 16 * 4 + 8 + df;
    int nb = inventory->getTroc(TT_SAC_RIZ);
    if (nb >= 0) {
        inventory->drawTroc(TT_SAC_RIZ, dx, dy);
        drawNombre(nb, dx, dy + 16);
        if (nb < 2) {
            TextManager::getInstance()->getText(72)->display(dx + 24, dy + 4);
        } else {
            TextManager::getInstance()->getText(73)->display(dx + 24, dy + 4);
        }
    }

    dy += 30;
    nb = inventory->getTroc(TT_SAC_OLIVES);
    if (nb >= 0) {
        inventory->drawTroc(TT_SAC_OLIVES, dx, dy);
        drawNombre(nb, dx, dy + 16);
        if (nb < 2) {
            TextManager::getInstance()->getText(74)->display(dx + 24, dy + 4);
        } else {
            TextManager::getInstance()->getText(75)->display(dx + 24, dy + 4);
        }
    }

    dy += 30;
    nb = inventory->getTroc(TT_BOCAL_EPICES);
    if (nb >= 0) {
        inventory->drawTroc(TT_BOCAL_EPICES, dx, dy);
        drawNombre(nb, dx, dy + 16);
        if (nb < 2) {
            TextManager::getInstance()->getText(76)->display(dx + 24, dy + 4);
        } else {
            TextManager::getInstance()->getText(77)->display(dx + 24, dy + 4);
        }
    }

    dy += 30;
    nb = inventory->getTroc(TT_POULET);
    if (nb >= 0) {
        inventory->drawTroc(TT_POULET, dx, dy);
        drawNombre(nb, dx, dy + 16);
        if (nb < 2) {
            TextManager::getInstance()->getText(78)->display(dx + 24, dy + 4);
        } else {
            TextManager::getInstance()->getText(79)->display(dx + 24, dy + 4);
        }
    }

    dy += 30;
    nb = inventory->getTroc(TT_PLUME);
    if (nb >= 0) {
        inventory->drawTroc(TT_PLUME, dx, dy);
        drawNombre(nb, dx, dy + 16);
        if (nb < 2) {
            TextManager::getInstance()->getText(80)->display(dx + 24, dy + 4);
        } else {
            TextManager::getInstance()->getText(81)->display(dx + 24, dy + 4);
        }
    }

    dx += 16 * 8 + 8;
    dy = 16 * 4 + 8 + df;
    nb = inventory->getTroc(TT_PERLES);
    if (nb >= 0) {
        inventory->drawTroc(TT_PERLES, dx, dy);
        drawNombre(nb, dx, dy + 16);
        if (nb < 2) {
            TextManager::getInstance()->getText(82)->display(dx + 24, dy + 4);
        } else {
            TextManager::getInstance()->getText(83)->display(dx + 24, dy + 4);
        }
    }

    dy += 30;
    nb = inventory->getTroc(TT_TAPISSERIE);
    if (nb >= 0) {
        inventory->drawTroc(TT_TAPISSERIE, dx, dy);
        drawNombre(nb, dx, dy + 16);
        if (nb < 2) {
            TextManager::getInstance()->getText(84)->display(dx + 24, dy + 4);
        } else {
            TextManager::getInstance()->getText(85)->display(dx + 24, dy + 4);
        }
    }

    dy += 30;
    nb = inventory->getTroc(TT_AMULETTE);
    if (nb >= 0) {
        inventory->drawTroc(TT_AMULETTE, dx, dy);
        drawNombre(nb, dx, dy + 16);
        if (nb < 2) {
            TextManager::getInstance()->getText(86)->display(dx + 24, dy + 4);
        } else {
            TextManager::getInstance()->getText(87)->display(dx + 24, dy + 4);
        }
    }

    dy += 30;
    nb = inventory->getTroc(TT_PARFUM);
    if (nb >= 0) {
        inventory->drawTroc(TT_PARFUM, dx, dy);
        drawNombre(nb, dx, dy + 16);
        if (nb < 2) {
            TextManager::getInstance()->getText(88)->display(dx + 24, dy + 4);
        } else {
            TextManager::getInstance()->getText(89)->display(dx + 24, dy + 4);
        }
    }

    dy += 30;
    nb = inventory->getTroc(TT_ROC_GIVRE);
    if (nb >= 0) {
        inventory->drawTroc(TT_ROC_GIVRE, dx, dy);
        drawNombre(nb, dx, dy + 16);
        if (nb < 2) {
            TextManager::getInstance()->getText(562)->display(dx + 24, dy + 4);
        } else {
            TextManager::getInstance()->getText(563)->display(dx + 24, dy + 4);
        }
    }
}

void Troc::drawCadre(int x, int y, int w, int h) {

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

void Troc::drawNombre(int val, int x, int y) {
    int val2;
    int srcX;
    int srcY;
    int dstX = x;

    //les dizaines
    val2 = (val%100)/10;
    srcX = (val2%5)*8;
    srcY = ((int)(val2/5))*8;
    if (val == 99) srcY += 16;
    WindowManager::getInstance()->draw(imageChiffres, srcX, srcY, 8, 8, dstX, y);
    dstX += 8;

    //unitées
    val2 = val%10;
    srcX = (val2%5)*8;
    srcY = ((int)(val2/5))*8;
    if (val == 99) srcY += 16;
    WindowManager::getInstance()->draw(imageChiffres, srcX, srcY, 8, 8, dstX, y);
}
