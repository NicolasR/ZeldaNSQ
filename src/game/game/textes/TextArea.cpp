#include "TextArea.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"
#include "../../../engine/texts/TextManager.h"

#include "../../MainController.h"

#include "../../content/link/Link.h"

TextArea::TextArea() : step(TEXT_OFF), anim(0), animMax(0), vanim(40), ready(false), text(0), id(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/texts/cadre.png", true);
    imageCoeur = ResourceManager::getInstance()->loadImage("data/images/status/coeur.png", true);
}

TextArea::~TextArea() {
    ResourceManager::getInstance()->free(image);
    ResourceManager::getInstance()->free(imageCoeur);
    delete text;
}

void TextArea::handleActions(Action* action) {
    if (action->isAction(ENTER)) {
        if (ready) {
            if (text->hasNext()) {
                text->next();
                start();
                AudioManager::getInstance()->playSound(TS_TEXTNEXT);
            } else if (hasLogicalNext()) {
                start();
                AudioManager::getInstance()->playSound(TS_TEXTNEXT);
            } else {
                stop();
                AudioManager::getInstance()->playSound(TS_TEXTEND);
            }
        } else {
            anim = animMax;
            ready = true;
        }
        return;
    }
    if (ready) {
        if (action->isAction(PUSH_UP)) {
            int newId = -1;
            switch (id) {
                case 13 : case 202 : case 239 : case 242 : case 245 : case 397 : case 499 : case 519 :
                case 538 :
                    newId = id + 2;
                    break;
                case 14 : case 15 : case 94 : case 108 : case 110 : case 112 : case 114 : case 116 :
                case 118 : case 120 : case 127 : case 129 : case 131 : case 167 : case 169 : case 171 :
                case 173 : case 175 : case 177 : case 179 : case 181 : case 183 : case 185 : case 187 :
                case 189 : case 201 : case 203 : case 204 : case 206 : case 207 : case 208 : case 218 :
                case 226 : case 229 : case 234 : case 236 : case 238 : case 240 : case 241 : case 243 :
                case 244 : case 246 : case 247 : case 249 : case 250 : case 251 : case 264 : case 267 :
                case 269 : case 271 : case 273 : case 275 : case 277 : case 279 : case 281 : case 329 :
                case 331 : case 342 : case 345 : case 347 : case 351 : case 354 : case 357 : case 362 :
                case 365 : case 369 : case 374 : case 377 : case 379 : case 381 : case 383 : case 385 :
                case 387 : case 389 : case 393 : case 396 : case 398 : case 399 : case 441 : case 448 :
                case 450 : case 452 : case 454 : case 457 : case 460 : case 467 : case 482 : case 498 :
                case 500 : case 501 : case 515 : case 517 : case 520 : case 521 : case 523 : case 525 :
                case 527 : case 537 : case 539 : case 540 : case 543 : case 546 : case 548 : case 551 :
                case 553 : case 555 : case 557 : case 568 : case 584 : case 602 : case 763 :
                    newId = id - 1;
                    break;
                case 93 : case 107 : case 109 : case 111 : case 113 : case 115 : case 117 : case 119 :
                case 126 : case 128 : case 130 : case 166 : case 168 : case 170 : case 172 : case 174 :
                case 176 : case 178 : case 180 : case 182 : case 184 : case 186 : case 188 : case 200 :
                case 217 : case 225 : case 228 : case 233 : case 235 : case 237 : case 263 : case 266 :
                case 268 : case 270 : case 272 : case 274 : case 276 : case 278 : case 280 : case 328 :
                case 330 : case 341 : case 344 : case 346 : case 350 : case 353 : case 356 : case 361 :
                case 364 : case 368 : case 373 : case 376 : case 378 : case 380 : case 382 : case 384 :
                case 386 : case 388 : case 392 : case 395 : case 440 : case 447 : case 449 : case 451 :
                case 453 : case 456 : case 459 : case 466 : case 481 : case 497 : case 514 : case 516 :
                case 522 : case 524 : case 526 : case 536 : case 542 : case 545 : case 547 : case 550 :
                case 552 : case 554 : case 556 : case 567 : case 583 : case 601 : case 762 :
                    newId = id + 1;
                    break;
                case 205 : case 248 :
                    newId = id + 3;
                    break;
                default : break;
            }
            if (newId != -1) {
                setTextId(newId);
                AudioManager::getInstance()->playSound(TS_MENU3);
            }
            return;
        }
        if (action->isAction(PUSH_DOWN)) {
            int newId = -1;
            switch (id) {
                case 13 : case 14 : case 93 : case 107 : case 109 : case 111 : case 113 : case 115 :
                case 117 : case 119 : case 126 : case 128 : case 130 : case 166 : case 168 : case 170 :
                case 172 : case 174 : case 176 : case 178 : case 180 : case 182 : case 184 : case 186 :
                case 188 : case 200 : case 202 : case 203 : case 205 : case 206 : case 207 : case 217 :
                case 225 : case 228 : case 233 : case 235 : case 237 : case 239 : case 240 : case 242 :
                case 243 : case 245 : case 246 : case 248 : case 249 : case 250 : case 263 : case 266 :
                case 268 : case 270 : case 272 : case 274 : case 276 : case 278 : case 280 : case 328 :
                case 330 : case 341 : case 344 : case 346 : case 350 : case 353 : case 356 : case 361 :
                case 364 : case 368 : case 373 : case 376 : case 378 : case 380 : case 382 : case 384 :
                case 386 : case 388 : case 392 : case 395 : case 397 : case 398 : case 440 : case 447 :
                case 449 : case 451 : case 453 : case 456 : case 459 : case 466 : case 481 : case 497 :
                case 499 : case 500 : case 514 : case 516 : case 519 : case 520 : case 522 : case 524 :
                case 526 : case 536 : case 538 : case 539 : case 542 : case 545 : case 547 : case 550 :
                case 552 : case 554 : case 556 : case 567 : case 583 : case 601 : case 762 :
                    newId = id + 1;
                    break;
                case 15 : case 204 : case 241 : case 244 : case 247 : case 399 : case 501 : case 521 :
                case 540 :
                    newId = id - 2;
                    break;
                case 94 : case 108 : case 110 : case 112 : case 114 : case 116 : case 118 : case 120 :
                case 127 : case 129 : case 131 : case 167 : case 169 : case 171 : case 173 : case 175 :
                case 177 : case 179 : case 181 : case 183 : case 185 : case 187 : case 189 : case 201 :
                case 218 : case 226 : case 229 : case 234 : case 236 : case 238 : case 264 : case 267 :
                case 269 : case 271 : case 273 : case 275 : case 277 : case 279 : case 281 : case 329 :
                case 331 : case 342 : case 345 : case 347 : case 351 : case 354 : case 357 : case 362 :
                case 365 : case 369 : case 374 : case 377 : case 379 : case 381 : case 383 : case 385 :
                case 387 : case 389 : case 393 : case 396 : case 441 : case 448 : case 450 : case 452 :
                case 454 : case 457 : case 460 : case 467 : case 482 : case 498 : case 515 : case 517 :
                case 523 : case 525 : case 527 : case 537 : case 543 : case 546 : case 548 : case 551 :
                case 553 : case 555 : case 557 : case 568 : case 584 : case 602 : case 763 :
                    newId = id - 1;
                    break;
                case 208 : case 251 :
                    newId = id - 3;
                    break;
                default : break;
            }
            if (newId != -1) {
                setTextId(newId);
                AudioManager::getInstance()->playSound(TS_MENU3);
            }
            return;
        }
        if (action->isAction(PUSH_LEFT)) {
            return;
        }
        if (action->isAction(PUSH_RIGHT)) {
            return;
        }
    }
}

void TextArea::loop() {
    if (!ready) {
        if (chrono.getElapsedTime() >= vanim) {
            do {
                ready = (++anim == animMax);
            } while (!ready && text->charAtInBox(anim) == ' ');
            AudioManager::getInstance()->playSound(TS_TEXT, 1);
            chrono.reset();
        }
    }
}

void TextArea::draw() {
    WindowManager::getInstance()->draw(image, 0, 0, 8, 8, 8, 16 * 9 + 8);
    WindowManager::getInstance()->draw(image, 24, 0, 8, 8, 16 * 19, 16 * 9 + 8);
    WindowManager::getInstance()->draw(image, 0, 24, 8, 8, 8, 16 * 14);
    WindowManager::getInstance()->draw(image, 24, 24, 8, 8, 16 * 19, 16 * 14);

    for (int i = 16; i < 16 * 19; i += 16) {
        WindowManager::getInstance()->draw(image, 8, 0, 16, 8, i, 16 * 9 + 8);
        WindowManager::getInstance()->draw(image, 8, 24, 16, 8, i, 16 * 14);
    }

    for (int j = 16 * 10; j < 16 * 14; j += 16) {
        WindowManager::getInstance()->draw(image, 0, 8, 8, 16, 8, j);
        WindowManager::getInstance()->draw(image, 24, 8, 8, 16, 16 * 19, j);
    }

    text->displayBox(16, 16 * 10, anim);

    drawAdditionalContent();
}

void TextArea::start() {
    chrono.reset();
    anim = 0;
    animMax = text->getLengthInBox();
    ready = false;
}

void TextArea::stop() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Map* map = scene->getMap();
    Link* link = scene->getLink();
    Navi* navi = scene->getNavi();
    if (link->getAnimation() == TROUVE_SIMPLE || link->getAnimation() == TROUVE_DOUBLE) {
        link->setAnimation(IDLE);
    }
    int tmp = 0;

    switch (id) {
        case 14 :
            MainController::getInstance()->getGameController()->saveData();
        case 15 : case 645 :
            AudioManager::getInstance()->stopMusic();
            MainController::getInstance()->setStep(LOGO);
            return;
        case 56 :
            AudioManager::getInstance()->playMusic(3);
            break;
        case 63 :
            navi->setMaxMagic(20);
            navi->setMagic(20);
            link->getInventory()->setObject(NAVI_HEAL);
            break;
        case 68 :
            link->trouveObjet(TI_COQUILLAGE, 0);
            return;
        case 93 :
            link->getStatus()->updateRupees(-10);
            link->trouveObjet(TI_SAC_RIZ);
            return;
        case 107 :
            link->getStatus()->updateRupees(-15);
            link->trouveObjet(TI_SAC_OLIVES);
            return;
        case 109 :
            link->getInventory()->removeTroc(TT_BOCAL_EPICES, 2);
            link->getInventory()->removeTroc(TT_SAC_RIZ, 1);
            link->trouveObjet(TI_POULET);
            return;
        case 111 :
            link->getInventory()->removeTroc(TT_PERLES, 1);
            link->getInventory()->removeTroc(TT_PLUME, 1);
            link->trouveObjet(TI_TAPISSERIE);
            return;
        case 113 :
            link->getInventory()->removeTroc(TT_SAC_RIZ, 1);
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 2);
            link->trouveObjet(TI_BOURSE_1);
            map->setSol(12 * 16, 13 * 16, 572);
            return;
        case 115 :
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 3);
            link->getStatus()->updateRupees(-50);
            link->trouveObjet(TI_BOURSE_2);
            map->setSol(12 * 16, 13 * 16, 573);
            return;
        case 117 :
            link->getInventory()->removeTroc(TT_PLUME, 1);
            link->getStatus()->updateRupees(-100);
            link->trouveObjet(TI_BOURSE_3);
            map->setSol(12 * 16, 13 * 16, 574);
            return;
        case 119 :
            link->getInventory()->removeTroc(TT_PARFUM, 1);
            link->getStatus()->updateRupees(-250);
            link->trouveObjet(TI_BOURSE_4);
            map->setSol(12 * 16, 13 * 16, 576);
            return;
        case 126 :
            link->getInventory()->removeTroc(TT_POULET, 1);
            link->getInventory()->removeTroc(TT_SAC_RIZ, 2);
            link->trouveObjet(TI_BOUCLIER_1);
            map->setSol(25 * 16, 13 * 16, 569);
            return;
        case 128 :
            link->getInventory()->removeTroc(TT_POULET, 2);
            link->getInventory()->removeTroc(TT_BOCAL_EPICES, 2);
            link->trouveObjet(TI_BOUCLIER_2);
            map->setSol(25 * 16, 13 * 16, 570);
            return;
        case 130 :
            link->getInventory()->removeTroc(TT_AMULETTE, 1);
            link->getInventory()->removeTroc(TT_TAPISSERIE, 1);
            link->trouveObjet(TI_BOUCLIER_3);
            map->setSol(25 * 16, 13 * 16, 576);
            return;
        case 166 :
            link->getStatus()->updateRupees(-20);
            link->trouveObjet(TI_BOCAL_EPICES);
            return;
        case 168 :
            link->getInventory()->removeTroc(TT_SAC_RIZ, 1);
            link->getStatus()->updateRupees(-2);
            link->trouveObjet(TI_SAC_OLIVES);
            return;
        case 170 :
            link->getInventory()->removeTroc(TT_SAC_RIZ, 3);
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 1);
            link->trouveObjet(TI_PLUME);
            return;
        case 172 :
            link->getInventory()->removeTroc(TT_POULET, 1);
            link->getInventory()->removeTroc(TT_PERLES, 1);
            link->trouveObjet(TI_AMULETTE);
            return;
        case 174 :
            link->getInventory()->removeTroc(TT_PLUME, 1);
            link->getInventory()->removeTroc(TT_BOCAL_EPICES, 2);
            link->trouveObjet(TI_FLACON_1);
            return;
        case 176 :
            link->getInventory()->removeTroc(TT_PERLES, 1);
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 5);
            link->trouveObjet(TI_FLACON_2);
            map->setSol(13 * 16, 8 * 16, 757);
            return;
        case 178 :
            link->getInventory()->removeTroc(TT_SAC_RIZ, 2);
            link->getInventory()->removeTroc(TT_BOCAL_EPICES, 2);
            link->trouveObjet(TI_GEMME_FORCE, 25);
            return;
        case 180 :
            link->getInventory()->removeTroc(TT_SAC_RIZ, 3);
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 3);
            link->trouveObjet(TI_GEMME_FORCE, 26);
            return;
        case 182 :
            link->getInventory()->removeTroc(TT_POULET, 1);
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 3);
            link->trouveObjet(TI_GEMME_FORCE, 27);
            return;
        case 184 :
            link->getInventory()->removeTroc(TT_PERLES, 1);
            link->getInventory()->removeTroc(TT_BOCAL_EPICES, 3);
            link->trouveObjet(TI_GEMME_FORCE, 28);
            return;
        case 186 :
            link->getInventory()->removeTroc(TT_PARFUM, 1);
            link->getInventory()->removeTroc(TT_PERLES, 1);
            link->trouveObjet(TI_GEMME_FORCE, 29);
            map->setSol(22 * 16, 8 * 16, 785);
            return;
        case 188 :
            link->getInventory()->removeTroc(TT_PARFUM, 1);
            link->getInventory()->removeTroc(TT_TAPISSERIE, 1);
            link->trouveObjet(TI_COQUILLAGE, 1);
            map->setSol(22 * 16, 8 * 16, 757);
            return;
        case 199 : case 201 : case 204 : case 208 :
            link->setAnimation(IDLE);
            break;
        case 200 : case 202 : case 205 :
            link->setAnimation(JOUE_OCARINA);
            MainController::getInstance()->getGameController()->getOcarinaController()->setSong(0);
            MainController::getInstance()->getGameController()->setStep(GAME_OCARINA);
            return;
        case 203 : case 206 :
            link->setAnimation(JOUE_OCARINA);
            MainController::getInstance()->getGameController()->getOcarinaController()->setSong(1);
            MainController::getInstance()->getGameController()->setStep(GAME_OCARINA);
            return;
        case 207 :
            link->setAnimation(JOUE_OCARINA);
            MainController::getInstance()->getGameController()->getOcarinaController()->setSong(2);
            MainController::getInstance()->getGameController()->setStep(GAME_OCARINA);
            return;
        case 209 :
            if (map->getId() == 29) {
                MainController::getInstance()->getGameController()->getOcarinaController()->setSong(1);
            } else {
                MainController::getInstance()->getGameController()->getOcarinaController()->setSong(0);
            }
            link->setAnimation(JOUE_OCARINA);
            MainController::getInstance()->getGameController()->setStep(GAME_OCARINA);
            return;
        case 210 :
            scene->getAnimationInGame()->startAnim(9);
            break;
        case 211 : case 214 :
            map->removeStele();
            break;
        case 217 :
            link->getInventory()->removeGemme(15);
            link->trouveObjet(TI_EPEE_2);
            return;
        case 225 :
            link->getInventory()->removeGemme(10);
            link->trouveObjet(TI_EPEE_3);
            return;
        case 228 :
            link->getInventory()->removeGemme(15);
            scene->getAnimationInGame()->startAnim(11);
            break;
        case 263 :
            link->getInventory()->removeTroc(TT_TAPISSERIE, 1);
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 5);
            link->trouveObjet(TI_RECETTE_POTION_JAUNE, 1);
            map->setSol(13 * 16, 20 * 16, 932);
            return;
        case 266 :
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 2);
            link->getInventory()->removeTroc(TT_PLUME, 1);
            link->trouveObjet(TI_QUART_COEUR, 8);
            return;
        case 268 :
            link->getInventory()->removeTroc(TT_SAC_RIZ, 2);
            link->getInventory()->removeTroc(TT_AMULETTE, 1);
            link->trouveObjet(TI_QUART_COEUR, 9);
            return;
        case 270 :
            link->getInventory()->removeTroc(TT_PARFUM, 1);
            link->getInventory()->removeTroc(TT_POULET, 1);
            link->trouveObjet(TI_QUART_COEUR, 10);
            return;
        case 272 :
            link->getInventory()->removeTroc(TT_AMULETTE, 1);
            link->getInventory()->removeTroc(TT_TAPISSERIE, 1);
            link->trouveObjet(TI_QUART_COEUR, 11);
            map->setSol(29 * 16, 20 * 16, 932);
            return;
        case 274 :
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 1);
            link->getStatus()->updateRupees(-2);
            link->trouveObjet(TI_SAC_RIZ);
            return;
        case 276 :
            link->getInventory()->removeTroc(TT_SAC_RIZ, 1);
            link->getStatus()->updateRupees(-5);
            link->trouveObjet(TI_BOCAL_EPICES);
            return;
        case 278 :
            link->getInventory()->removeTroc(TT_BOCAL_EPICES, 2);
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 1);
            link->trouveObjet(TI_PERLES);
            return;
        case 280 :
            link->getInventory()->removeTroc(TT_POULET, 1);
            link->getInventory()->removeTroc(TT_PLUME, 1);
            link->trouveObjet(TI_PARFUM);
            return;
        case 299 :
            link->trouveObjet(TI_GANT2);
            return;
        case 300 :
            link->trouveObjet(TI_ARC_FEE);
            link->getInventory()->setCurrent(ARC_FEE);
            return;
        case 301 :
            link->trouveObjet(TI_BOOMERANG_FEE);
            link->getInventory()->setCurrent(BOOMERANG_FEE);
            return;
        case 310 :
            if (map->getId() == 17) scene->setFees(2, 2);
            if (map->getId() == 19) scene->setFees(4, 2);
            if (map->getId() == 25) scene->setFees(6, 2);
            if (map->getId() == 28) scene->setFees(7, 2);
            navi->setMaxMagic(navi->getMaxMagic() + 5);
            navi->setMagic(navi->getMaxMagic());
            AudioManager::getInstance()->playSound(TS_HAPPY, 2);
            if (navi->getMaxMagic() == 25) {
                MainController::getInstance()->getGameController()->displayText(312);
                link->getInventory()->setObject(NAVI_ATT);
                return;
            } else if (navi->getMaxMagic() == 30) {
                MainController::getInstance()->getGameController()->displayText(313);
                link->getInventory()->setObject(NAVI_DEF);
                return;
            } else if (navi->getMaxMagic() == 35) {
                MainController::getInstance()->getGameController()->displayText(314);
                return;
            } else if (navi->getMaxMagic() == 40) {
                MainController::getInstance()->getGameController()->displayText(315);
                return;
            }
            break;
        case 316 :
            AudioManager::getInstance()->playSound(TS_HAPPY, 2);
            link->getStatus()->setMaxMagic(64);
            link->getStatus()->setMagic(64);
            MainController::getInstance()->getGameController()->displayText(317);
            return;
        case 318 : case 322 :
            map->testAnimRoom();
            break;
        case 321 :
            link->trouveObjet(TI_PALMES);
            return;
        case 328 :
            link->getStatus()->updateRupees(-100);
            AudioManager::getInstance()->playSound(TS_HAPPY, 2);

            if (link->getStatus()->getMaxBombs() < 30 &&
                (!link->getInventory()->hasObject(ARC)
                || link->getStatus()->getMaxArrows() == 70
                || (int)((float)rand() / RAND_MAX * 2))) {
                link->getStatus()->setMaxBombs(link->getStatus()->getMaxBombs() + 2);
                link->getStatus()->setBombs(link->getStatus()->getMaxBombs());
                MainController::getInstance()->getGameController()->displayText(333);
            } else {
                link->getStatus()->setMaxArrows(link->getStatus()->getMaxArrows() + 5);
                link->getStatus()->setArrows(link->getStatus()->getMaxArrows());
                MainController::getInstance()->getGameController()->displayText(332);
            }
            return;
        case 330 :
            link->getStatus()->updateRupees(-200);
            AudioManager::getInstance()->playSound(TS_HAPPY, 2);
            if (!link->getStatus()->getRecupBombs() && (link->getStatus()->getRecupArrows() || (int)((float)rand() / RAND_MAX * 2))) {
                link->getStatus()->setRecupBombs(1);
                MainController::getInstance()->getGameController()->displayText(335);
            } else {
                link->getStatus()->setRecupArrows(1);
                MainController::getInstance()->getGameController()->displayText(334);
            }
            return;
        case 341 :
            link->getInventory()->removeCoquillages(1);
            link->trouveObjet(TI_CARTE_ILE);
            map->setSol(5 * 16, 5 * 16, 1366);
            return;
        case 344 :
            link->getInventory()->removeCoquillages(2);
            if (!link->getInventory()->hasCoeur(3)) {
                link->trouveObjet(TI_COEUR, 3);
            } else {
                link->trouveObjet(TI_COEUR, 4);
                map->setSol(7 * 16, 5 * 16, 1366);
            }
            return;
        case 346 :
            link->getInventory()->removeCoquillages(2);
            link->trouveObjet(TI_DETECTEUR);
            map->setSol(9 * 16, 5 * 16, 1366);
            return;
        case 350 :
            link->getInventory()->removeCoquillages(4);
            link->trouveObjet(TI_TUNIQUE_BLEUE);
            map->setSol(11 * 16, 5 * 16, 1370);
            return;
        case 352 :
            link->setTunique(2);
            break;
        case 353 :
            link->getInventory()->removeCoquillages(4);
            link->trouveObjet(TI_TUNIQUE_ROUGE);
            map->setSol(11 * 16, 5 * 16, 1366);
            return;
        case 355 :
            link->setTunique(3);
            break;
        case 356 :
            link->getInventory()->removeCoquillages(5);
            link->trouveObjet(TI_CANNE_BYRNA);
            map->setSol(13 * 16, 5 * 16, 1366);
            return;
        case 361 :
            link->getStatus()->updateRupees(-250);
            link->trouveObjet(TI_BOTTES);
            map->setSol(6 * 16, 5 * 16, 1366);
            return;
        case 364 :
            link->getStatus()->updateRupees(-5);
            link->getStatus()->setBombs(link->getStatus()->getBombs() + 10);
            break;
        case 368 :
            link->getStatus()->updateRupees(-5);
            link->getStatus()->setArrows(link->getStatus()->getArrows() + 10);
            break;
        case 373 :
            link->getStatus()->updateRupees(-1);
            link->getStatus()->updateLife(10);
            break;
        case 376 :
            link->getStatus()->updateRupees(-2);
            AudioManager::getInstance()->playSound(TS_ITEM);
            link->getStatus()->updateMagic(link->getStatus()->getMaxMagic() / 8);
            break;
        case 378 :
            link->getStatus()->updateRupees(-4);
            AudioManager::getInstance()->playSound(TS_MAGICCHARGE);
            link->getStatus()->updateMagic(link->getStatus()->getMaxMagic() / 4);
            break;
        case 380 :
            link->getStatus()->updateRupees(-10);
            map->closeChests(5, 83, true);
            break;
        case 382 :
            link->getStatus()->updateRupees(-10);
            map->closeChests(4, 84, true);
            break;
        case 384 :
            link->getStatus()->updateRupees(-10);
            map->closeChests(3, 85, true);
            break;
        case 386 :
            link->getStatus()->updateRupees(-10);
            map->closeChests(2, 86, true);
            break;
        case 388 :
            link->getStatus()->updateRupees(-10);
            map->closeChests(1, 87, true);
            break;
        case 392 :
            link->getStatus()->updateRupees(-20);
            map->closeChests(1, 2, false);
            break;
        case 426 :
            link->trouveObjet(TI_SAC_TROC);
            scene->setAvancement(AV_MAIRE_VU);
            return;
        case 428 :
            scene->setAvancement(AV_MAIRE_OK);
            break;
        case 443 :
            scene->getPrimes()->start();
            break;
        case 446 :
            if (link->getInventory()->hasCoquillages(8)) {
                link->trouveObjet(TI_COQUILLAGE, 9);
            } else if (link->getInventory()->hasCoquillages(7)) {
                link->trouveObjet(TI_COQUILLAGE, 8);
            } else if (link->getInventory()->hasCoquillages(6)) {
                link->trouveObjet(TI_COQUILLAGE, 7);
            } else if (link->getInventory()->hasCoquillages(5)) {
                link->trouveObjet(TI_COQUILLAGE, 6);
            } else if (link->getInventory()->hasCoquillages(4)) {
                link->trouveObjet(TI_COQUILLAGE, 5);
            } else if (link->getInventory()->hasCoquillages(3)) {
                link->trouveObjet(TI_COQUILLAGE, 4);
            } else {
                link->trouveObjet(TI_COQUILLAGE, 3);
            }
            scene->getPrimes()->stop(false);
            return;
        case 456 :
            link->getStatus()->updateRupees(-50);
            link->trouveObjet(TI_SAC_BOMBES);
            map->setSol(6 * 16, 5 * 16, 1366);
            return;
        case 459 :
            link->getStatus()->updateRupees(-100);
            link->trouveObjet(TI_QUART_COEUR, 15);
            map->setSol(5 * 16, 5 * 16, 1366);
            return;
        case 468 :
            link->getStatus()->updateRupees(-10);
            map->startCibles();
            break;
        case 470 : case 471 : case 472 : case 473 :
            map->endCibles();
            break;
        case 474 :
            link->trouveObjet(TI_GEMME_FORCE, 89);
            map->endCibles();
            return;
        case 475 :
            link->trouveObjet(TI_COQUILLAGE, 10);
            map->endCibles();
            return;
        case 507 : case 508 : case 509 : case 510 : case 511 : case 512 : case 513 :
            link->trouveObjet(TI_COQUILLAGE, 11 + id - 507); // 11 -> 17
            map->endCibles();
            return;
        case 522 :
            link->getInventory()->removeTroc(TT_BOCAL_EPICES, 2);
            link->getInventory()->removeTroc(TT_SAC_RIZ, 1);
            link->trouveObjet(TI_POTION_ROUGE);
            scene->setCoffre(0, 12, 1);
            return;
        case 524 :
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 2);
            link->getInventory()->removeTroc(TT_SAC_RIZ, 1);
            link->trouveObjet(TI_POTION_VERTE);
            scene->setCoffre(0, 13, 1);
            return;
        case 526 :
            link->getInventory()->removeTroc(TT_BOCAL_EPICES, 1);
            link->getInventory()->removeTroc(TT_SAC_OLIVES, 1);
            link->trouveObjet(TI_POTION_JAUNE);
            scene->setCoffre(0, 14, 1);
            return;
        case 536 :
            tmp = 0;
            for (int i = 0; i < NB_FLACON; i++) {
                if ((link->getInventory()->getFlacon(i) == 1
                    || link->getInventory()->getFlacon(i) == 2
                    || link->getInventory()->getFlacon(i) == 4)
                    && (tmp == 0 || tmp != link->getInventory()->getFlacon(i))) {
                    if (tmp == 0) {
                        tmp = link->getInventory()->getFlacon(i);
                    } else {
                        if ((tmp == 1 && link->getInventory()->getFlacon(i) == 2) || (tmp == 2 && link->getInventory()->getFlacon(i) == 1) ) {
                            link->trouveObjet(TI_POTION_BLEUE);
                            scene->setCoffre(0, 15, 1);
                            return;
                        }
                        if ((tmp == 1 && link->getInventory()->getFlacon(i) == 4) || (tmp == 4 && link->getInventory()->getFlacon(i) == 1) ) {
                            link->trouveObjet(TI_POTION_ORANGE);
                            scene->setCoffre(0, 16, 1);
                            return;
                        }
                        if ((tmp == 2 && link->getInventory()->getFlacon(i) == 4) || (tmp == 4 && link->getInventory()->getFlacon(i) == 2) ) {
                            link->trouveObjet(TI_POTION_VIOLETTE);
                            scene->setCoffre(0, 17, 1);
                            return;
                        }
                    }
                }
            }
            break;
        case 538 :
            link->trouveObjet(TI_POTION_BLEUE);
            scene->setCoffre(0, 15, 1);
            return;
        case 539 :
            link->trouveObjet(TI_POTION_ORANGE);
            scene->setCoffre(0, 16, 1);
            return;
        case 540 :
            link->trouveObjet(TI_POTION_VIOLETTE);
            scene->setCoffre(0, 17, 1);
            return;
        case 542 :
            link->trouveObjet(TI_POTION_GRISE);
            scene->setCoffre(0, 18, 1);
            return;
        case 545 :
            link->getStatus()->updateRupees(-200);
            link->trouveObjet(TI_COQUILLAGE, 18);
            map->setSol(6 * 16, 5 * 16, 1366);
            return;
        case 547 :
            link->getStatus()->updateRupees(-100);
            link->trouveObjet(TI_FLACON_3);
            map->setSol(5 * 16, 5 * 16, 1366);
            return;
        case 558 :
            link->getInventory()->removeTroc(TT_TAPISSERIE, 1);
            link->getStatus()->updateRupees(100);
            break;
        case 559 :
            link->getInventory()->removeTroc(TT_ROC_GIVRE, 1);
            link->getStatus()->updateRupees(350);
            break;
        case 564 :
            link->getInventory()->removeTroc(TT_AMULETTE, 1);
            link->getStatus()->updateRupees(100);
            break;
        case 565 :
            link->getInventory()->removeTroc(TT_PARFUM, 1);
            link->getStatus()->updateRupees(100);
            break;
        case 569 :
            link->getInventory()->removeTroc(TT_ROC_GIVRE, 1);
            link->trouveObjet(TI_COQUILLAGE, 19);
            return;
        case 576 :
            MainController::getInstance()->getGameController()->getOcarinaController()->setSong(2);
            MainController::getInstance()->getGameController()->setStep(GAME_OCARINA);
            return;
        case 583 :
            link->getInventory()->removeTroc(TT_TAPISSERIE, 1);
            link->getInventory()->removeTroc(TT_AMULETTE, 1);
            link->getInventory()->removeTroc(TT_PARFUM, 1);
            link->trouveObjet(TI_ROC_GIVRE);
            return;
        case 617 :
            if (scene->getAvancement() < AV_TUTO_LAPIN_DONE) {
                scene->getAnimationInGame()->startAnim(36);
            } else if (scene->getAvancement() < AV_TUTO_PERLE_LUNE && link->getInventory()->hasObject(PERLE_LUNE)) {
                scene->getAnimationInGame()->startAnim(37);
            }
            break;
        case 620 :
            if (scene->getAvancement() < AV_FINAL_TUTO_RDC_DONE) {
                scene->getAnimationInGame()->startAnim(46);
            }
            break;
        case 642 :
            link->trouveObjet(TI_EPEE_5);
            return;
        case 646 :
            scene->getAnimationInGame()->startAnim(45);
            break;
        case 654 :
            MainController::getInstance()->getGameController()->saveData(true);
            MainController::getInstance()->setStep(ENDING);
            return;
        case 762 :
            AudioManager::getInstance()->playSound(TS_TELEPORT);
            if (map->getId() == 58) MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(2, 105 * 16 + 8, 8 * 16 + 8, S, false, true);
            else if (map->getId() == 59) MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(6, 19 * 16, 16 * 16 + 8, S, false, true);
            else if (map->getId() == 60) MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(11, 20 * 16 + 8, 28 * 16 + 8, S, false, true);
            else if (map->getId() == 61) MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(4, 34 * 16 + 8, 22 * 16 + 8, S, false, true);
            else if (map->getId() == 62) MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(14, 26 * 16, 26 * 16 + 8, S, false, true);
            else if (map->getId() == 63) MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(13, 10 * 16 + 8, 12 * 16, S, false, true);
            else if (map->getId() == 64) MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(14, 133 * 16 + 8, 27 * 16 + 8, S, false, true);
            else if (map->getId() == 65) MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(9, 39 * 16 + 8, 28 * 16, S, false, true);
            MainController::getInstance()->getGameController()->setStep(GAME_TELEPORT);
            return;
        default : break;
    }

    MainController::getInstance()->getGameController()->setStep(GAME_MAIN);
}

void TextArea::setTextId(int textId) {
    delete text;
    text = TextManager::getInstance()->getText(textId);
    text->setBox(16 * 18, 16 * 4);
    id = textId;
}

bool TextArea::hasLogicalNext() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    Navi* navi = scene->getNavi();
    int tmp = 0;
    int next = 0;
    switch (id) {
        case 10 :
            next = link->getInventory()->nbQuartCoeur() ? 11 : 12;
            break;
        case 14 :
            if (scene->getMap()->getId() == 66) {
                next = 644;
                break;
            }
            return false;
        case 37 : case 39 : case 54 : case 55 : case 136 : case 144 : case 149 : case 213 : case 219 :
        case 282 : case 284 : case 293 : case 298 : case 319 : case 320 : case 348 : case 400 : case 401 :
        case 402 : case 403 : case 404 : case 406 : case 408 : case 411 : case 413 : case 414 : case 415 :
        case 416 : case 417 : case 418 : case 419 : case 420 : case 421 : case 422 : case 423 : case 424 :
        case 431 : case 438 : case 439 : case 442 : case 480 : case 483 : case 484 : case 489 : case 495 :
        case 496 : case 575 : case 579 : case 582 : case 625 : case 632 : case 633 : case 634 : case 635 :
        case 636 : case 637 : case 638 : case 639 : case 640 : case 641 : case 648 : case 649 : case 650 :
            next = id + 1;
            break;
        case 60 : case 61 : case 62 :
            next = 63;
            break;
        case 93 :
            if (link->getStatus()->getVirtualRupees() >= 10) {
                if (link->getInventory()->getTroc(TT_SAC_RIZ) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 94 : case 108 : case 110 : case 112 : case 114 : case 116 : case 118 : case 120 :
        case 127 : case 129 : case 131 : case 167 : case 169 : case 171 : case 173 : case 175 :
        case 177 : case 179 : case 181 : case 183 : case 185 : case 187 : case 189 : case 264 :
        case 267 : case 269 : case 271 : case 273 : case 275 : case 277 : case 279 : case 281 :
        case 381 : case 383 : case 385 : case 387 : case 389 : case 393 : case 441 : case 448 :
        case 450 : case 452 : case 454 : case 467 : case 515 : case 523 : case 525 : case 527 :
        case 537 : case 543 : case 551 : case 553 : case 555 : case 557 : case 584 :
            // "reviens si tu change d'avis"
            next = 95;
            break;
        case 107 :
            if (link->getStatus()->getVirtualRupees() >= 15) {
                if (link->getInventory()->getTroc(TT_SAC_OLIVES) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 109 :
            if (link->getInventory()->getTroc(TT_BOCAL_EPICES) >= 2
                && link->getInventory()->getTroc(TT_SAC_RIZ) >= 1) {
                if (link->getInventory()->getTroc(TT_POULET) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 111 :
            if (link->getInventory()->getTroc(TT_PERLES) >= 1
                && link->getInventory()->getTroc(TT_PLUME) >= 1) {
                if (link->getInventory()->getTroc(TT_TAPISSERIE) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 113 :
            if (link->getInventory()->getTroc(TT_SAC_RIZ) >= 1
                && link->getInventory()->getTroc(TT_SAC_OLIVES) >= 2) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 115 :
            if (link->getStatus()->getVirtualRupees() >= 50
                && link->getInventory()->getTroc(TT_SAC_OLIVES) >= 3) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 117 :
            if (link->getStatus()->getVirtualRupees() >= 100
                && link->getInventory()->getTroc(TT_PLUME) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 119 :
            if (link->getStatus()->getVirtualRupees() >= 250
                && link->getInventory()->getTroc(TT_PARFUM) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 126 :
            if (link->getInventory()->getTroc(TT_SAC_RIZ) >= 2
                && link->getInventory()->getTroc(TT_POULET) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 128 :
            if (link->getInventory()->getTroc(TT_BOCAL_EPICES) >= 2
                && link->getInventory()->getTroc(TT_POULET) >= 2) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 130 :
            if (link->getInventory()->getTroc(TT_AMULETTE) >= 1
                && link->getInventory()->getTroc(TT_TAPISSERIE) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 146 : case 147 : case 148 :
            next = 149;
            break;
        case 166 :
            if (link->getStatus()->getVirtualRupees() >= 20) {
                if (link->getInventory()->getTroc(TT_BOCAL_EPICES) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 168 :
            if (link->getStatus()->getVirtualRupees() >= 2
                && link->getInventory()->getTroc(TT_SAC_RIZ) >= 1) {
                if (link->getInventory()->getTroc(TT_SAC_OLIVES) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 170 :
            if (link->getInventory()->getTroc(TT_SAC_RIZ) >= 3
                && link->getInventory()->getTroc(TT_SAC_OLIVES) >= 1) {
                if (link->getInventory()->getTroc(TT_PLUME) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 172 :
            if (link->getInventory()->getTroc(TT_POULET) >= 1
                && link->getInventory()->getTroc(TT_PERLES) >= 1) {
                if (link->getInventory()->getTroc(TT_AMULETTE) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 174 :
            if (link->getInventory()->getTroc(TT_PLUME) >= 1
                && link->getInventory()->getTroc(TT_BOCAL_EPICES) >= 2) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 176 :
            if (link->getInventory()->getTroc(TT_PERLES) >= 1
                && link->getInventory()->getTroc(TT_SAC_OLIVES) >= 5) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 178 :
            if (link->getInventory()->getTroc(TT_SAC_RIZ) >= 2
                && link->getInventory()->getTroc(TT_BOCAL_EPICES) >= 2) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 180 :
            if (link->getInventory()->getTroc(TT_SAC_RIZ) >= 3
                && link->getInventory()->getTroc(TT_SAC_OLIVES) >= 3) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 182 :
            if (link->getInventory()->getTroc(TT_POULET) >= 1
                && link->getInventory()->getTroc(TT_SAC_OLIVES) >= 3) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 184 :
            if (link->getInventory()->getTroc(TT_PERLES) >= 1
                && link->getInventory()->getTroc(TT_BOCAL_EPICES) >= 3) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 186 :
            if (link->getInventory()->getTroc(TT_PARFUM) >= 1
                && link->getInventory()->getTroc(TT_PERLES) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 188 :
            if (link->getInventory()->getTroc(TT_PARFUM) >= 1
                && link->getInventory()->getTroc(TT_TAPISSERIE) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 215 :
            if (link->getInventory()->nbGemmeForce() < 15) {
                next = 216;
            } else {
                next = 217;
            }
            break;
        case 232 :
            if (navi != 0) {
                int lvl = (navi->getMaxMagic() - 15) / 5;
                if (lvl < 1) lvl = 1;
                bool heal = navi->getLvlHeal() < lvl;
                bool att = lvl > 1 && navi->getLvlAtt() < lvl;
                bool def = lvl > 2 && navi->getLvlDef() < lvl;

                if (heal && !att && !def) {
                    next = 233;
                } else if (!heal && att && !def) {
                    next = 235;
                } else if (!heal && !att && def) {
                    next = 237;
                } else if (heal && att && !def) {
                    next = 239;
                } else if (heal && !att && def) {
                    next = 242;
                } else if (!heal && att && def) {
                    next = 245;
                } else if (heal && att && def) {
                    next = 248;
                }
            }
            break;
        case 233 : case 239 : case 242 : case 248 :
            AudioManager::getInstance()->playSound(TS_HAPPY, 2);
            link->getInventory()->removeGemme(5);
            navi->setLvlHeal(navi->getLvlHeal() + 1);
            next = 252;
            break;
        case 235 : case 240 : case 245 : case 249 :
            AudioManager::getInstance()->playSound(TS_HAPPY, 2);
            link->getInventory()->removeGemme(5);
            navi->setLvlAtt(navi->getLvlAtt() + 1);
            next = 253;
            break;
        case 237 : case 243 : case 246 : case 250 :
            AudioManager::getInstance()->playSound(TS_HAPPY, 2);
            link->getInventory()->removeGemme(5);
            navi->setLvlDef(navi->getLvlDef() + 1);
            next = 254;
            break;
        case 263 :
            if (link->getInventory()->getTroc(TT_SAC_OLIVES) >= 5
                && link->getInventory()->getTroc(TT_TAPISSERIE) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 266 :
            if (link->getInventory()->getTroc(TT_SAC_OLIVES) >= 2
                && link->getInventory()->getTroc(TT_PLUME) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 268 :
            if (link->getInventory()->getTroc(TT_SAC_RIZ) >= 2
                && link->getInventory()->getTroc(TT_AMULETTE) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 270 :
            if (link->getInventory()->getTroc(TT_PARFUM) >= 1
                && link->getInventory()->getTroc(TT_POULET) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 272 :
            if (link->getInventory()->getTroc(TT_AMULETTE) >= 1
                && link->getInventory()->getTroc(TT_TAPISSERIE) >= 1) {
                return false;
            } else {
                next = 96;
            }
            break;
        case 274 :
            if (link->getStatus()->getVirtualRupees() >= 2
                && link->getInventory()->getTroc(TT_SAC_OLIVES) >= 1) {
                if (link->getInventory()->getTroc(TT_SAC_RIZ) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 276 :
            if (link->getStatus()->getVirtualRupees() >= 5
                && link->getInventory()->getTroc(TT_SAC_RIZ) >= 1) {
                if (link->getInventory()->getTroc(TT_BOCAL_EPICES) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 278 :
            if (link->getInventory()->getTroc(TT_BOCAL_EPICES) >= 2
                && link->getInventory()->getTroc(TT_SAC_OLIVES) >= 1) {
                if (link->getInventory()->getTroc(TT_PERLES) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 280 :
            if (link->getInventory()->getTroc(TT_POULET) >= 1
                && link->getInventory()->getTroc(TT_PLUME) >= 1) {
                if (link->getInventory()->getTroc(TT_PARFUM) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 326 :
            next = 328;
            break;
        case 328 :
            if (link->getStatus()->getVirtualRupees() < 100) {
                next = 336;
                break;
            }
            return false;
        case 329 : case 331 :
            next = 337;
            break;
        case 330 :
            if (link->getStatus()->getVirtualRupees() < 200) {
                next = 336;
                break;
            }
            return false;
        case 341 :
            if (link->getInventory()->nbCoquillages() < 1) {
                next = 96;
                break;
            }
            return false;
        case 344 : case 346 :
            if (link->getInventory()->nbCoquillages() < 2) {
                next = 96;
                break;
            }
            return false;
        case 350 : case 353 :
            if (link->getInventory()->nbCoquillages() < 4) {
                next = 96;
                break;
            }
            return false;
        case 356 :
            if (link->getInventory()->nbCoquillages() < 5) {
                next = 96;
                break;
            }
            return false;
        case 361 :
            if (link->getStatus()->getVirtualRupees() < 250) {
                next = 96;
                break;
            }
            return false;
        case 364 : case 368 :
            if (link->getStatus()->getVirtualRupees() < 5) {
                next = 96;
                break;
            }
            return false;
        case 373 :
            if (link->getStatus()->getVirtualRupees() < 1) {
                next = 96;
                break;
            }
            return false;
        case 376 :
            if (link->getStatus()->getVirtualRupees() < 2) {
                next = 96;
                break;
            }
            return false;
        case 378 :
            if (link->getStatus()->getVirtualRupees() < 4) {
                next = 96;
                break;
            }
            return false;
        case 380 : case 382 : case 384 : case 386 : case 388 :
            if (link->getStatus()->getVirtualRupees() < 10) {
                next = 96;
                break;
            }
            return false;
        case 392 :
            if (link->getStatus()->getVirtualRupees() < 20) {
                next = 96;
                break;
            }
            return false;
        case 395 :
            next = 397;
            break;
        case 397 :
            next = 400;
            break;
        case 398 :
            next = 406;
            break;
        case 399 :
            next = 408;
            break;
        case 425 :
            if (!link->getInventory()->hasObject(SAC_TROC)) {
                next = 426;
                break;
            }
            return false;
        case 440 : case 447 : case 449 : case 451 : case 453 :
            if (link->getInventory()->hasCoquillages(8)) {
                scene->getPrimes()->init(42);
            } else if (link->getInventory()->hasCoquillages(7)) {
                scene->getPrimes()->init(15);
            } else if (link->getInventory()->hasCoquillages(6)) {
                scene->getPrimes()->init(14);
            } else if (link->getInventory()->hasCoquillages(5)) {
                scene->getPrimes()->init(18);
            } else if (link->getInventory()->hasCoquillages(4)) {
                scene->getPrimes()->init(12);
            } else if (link->getInventory()->hasCoquillages(3)) {
                scene->getPrimes()->init(23);
            } else {
                scene->getPrimes()->init(53);
            }
            next = 442;
            break;
        case 456 :
            if (link->getStatus()->getVirtualRupees() < 50) {
                next = 96;
                break;
            }
            return false;
        case 459 :
            if (link->getStatus()->getVirtualRupees() < 100) {
                next = 96;
                break;
            }
            return false;
        case 461 : case 462 : case 463 : case 464 :
            next = 466;
            break;
        case 466 :
            if (link->getStatus()->getVirtualRupees() < 10) {
                next = 96;
                break;
            }
            next = 468;
            break;
        case 473 :
            if (!link->getInventory()->hasCoquillages(10)) {
                next = 475;
                break;
            }
        case 472 :
            if (!link->getInventory()->hasGemmeForce(89)) {
                next = 474;
                break;
            }
            return false;
        case 481 :
            next = 483;
            break;
        case 482 :
            next = 487;
            break;
        case 497 :
            next = 499;
            break;
        case 498 :
            next = 506;
            break;
        case 499 :
            next = 502;
            break;
        case 500 :
            next = 503;
            break;
        case 501 :
            next = 504;
            break;
        case 514 :
            next = scene->getCoffre(0, 19) ? 519 : 516;
            break;
        case 516 : case 519 :
            next = 522;
            break;
        case 517 : case 520 :
            next = 524;
            break;
        case 518 :
            if ( (!link->getInventory()->hasObject(FLACON_1) || link->getInventory()->getFlacon(0))
                && (!link->getInventory()->hasObject(FLACON_2) || link->getInventory()->getFlacon(1))
                && (!link->getInventory()->hasObject(FLACON_3) || link->getInventory()->getFlacon(2))
                && (!link->getInventory()->hasObject(FLACON_4) || link->getInventory()->getFlacon(3)) ) {
                next = 528;
            } else {
                next = 519;
            }
            break;
        case 521 :
            next = 526;
            break;
        case 522 :
            if (link->getInventory()->getTroc(TT_BOCAL_EPICES) < 2
                || link->getInventory()->getTroc(TT_SAC_RIZ) < 1) {
                next = 96;
                break;
            }
            return false;
        case 524 :
            if (link->getInventory()->getTroc(TT_SAC_OLIVES) < 2
                || link->getInventory()->getTroc(TT_SAC_RIZ) < 1) {
                next = 96;
                break;
            }
            return false;
        case 526 :
            if (link->getInventory()->getTroc(TT_BOCAL_EPICES) < 1
                || link->getInventory()->getTroc(TT_SAC_OLIVES) < 1) {
                next = 96;
                break;
            }
            return false;
        case 536 :
            if (!link->getInventory()->hasObject(FLACON_1)
                || !link->getInventory()->hasObject(FLACON_2)
                || !link->getInventory()->hasObject(FLACON_3)
                || !link->getInventory()->hasObject(FLACON_4)) {
                return false;
            }
            tmp = -1;
            for (int i = 0; i < NB_FLACON; i++) {
                if (link->getInventory()->getFlacon(i) == 1) {
                    tmp = i;
                    break;
                }
            }
            if (tmp == -1) return false;
            tmp = -1;
            for (int i = 0; i < NB_FLACON; i++) {
                if (link->getInventory()->getFlacon(i) == 2) {
                    tmp = i;
                    break;
                }
            }
            if (tmp == -1) return false;
            tmp = -1;
            for (int i = 0; i < NB_FLACON; i++) {
                if (link->getInventory()->getFlacon(i) == 4) {
                    tmp = i;
                    break;
                }
            }
            if (tmp == -1) return false;
            next = 538;
            break;
        case 545 :
            if (link->getStatus()->getVirtualRupees() < 200) {
                next = 96;
                break;
            }
            return false;
        case 547 :
            if (link->getStatus()->getVirtualRupees() < 100) {
                next = 96;
                break;
            }
            return false;
        case 550 :
            if (link->getInventory()->getTroc(TT_TAPISSERIE) < 1) {
                next = 560;
                break;
            }
            if (link->getStatus()->getVirtualRupees() == link->getStatus()->getMaxRupees()) {
                next = 566;
                break;
            }
            next = 558;
            break;
        case 552 :
            if (link->getInventory()->getTroc(TT_AMULETTE) < 1) {
                next = 560;
                break;
            }
            if (link->getStatus()->getVirtualRupees() == link->getStatus()->getMaxRupees()) {
                next = 566;
                break;
            }
            next = 564;
            break;
        case 554 :
            if (link->getInventory()->getTroc(TT_PARFUM) < 1) {
                next = 560;
                break;
            }
            if (link->getStatus()->getVirtualRupees() == link->getStatus()->getMaxRupees()) {
                next = 566;
                break;
            }
            next = 565;
            break;
        case 556 :
            if (link->getInventory()->getTroc(TT_ROC_GIVRE) < 1) {
                next = 560;
                break;
            }
            if (link->getStatus()->getVirtualRupees() == link->getStatus()->getMaxRupees()) {
                next = 566;
                break;
            }
            next = 559;
            break;
        case 567 :
            if (link->getInventory()->getTroc(TT_ROC_GIVRE) < 1) {
                next = 570;
                break;
            }
            next = 569;
            break;
        case 568 :
            next = 572;
            break;
        case 583 :
            if (link->getInventory()->getTroc(TT_TAPISSERIE) >= 1
                && link->getInventory()->getTroc(TT_AMULETTE) >= 1
                && link->getInventory()->getTroc(TT_PARFUM) >= 1) {
                if (link->getInventory()->getTroc(TT_ROC_GIVRE) == 99) {
                    next = 106;
                    break;
                }
                return false;
            } else {
                next = 96;
            }
            break;
        case 590 : case 591 : case 592 : case 593 : case 594 : case 595 : case 596 :
            tmp = link->getInventory()->nbMedaillons();
            if (tmp == 7) next = 600;
            else if (tmp == 6) next = 599;
            else if (tmp == 5) next = 598;
            else next = 597;
            break;
        case 597 : case 598 : case 599 : case 600 : case 223 :
            next = 601;
            break;
        case 601 :
            MainController::getInstance()->getGameController()->saveData();
        case 602 :
            next = 762;
            break;
        default : return false;
    }
    setTextId(next);
    return true;
}

void TextArea::drawAdditionalContent() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    int tmp;
    switch (id) {
        case 10 :
            tmp = link->getInventory()->nbQuartCoeur();
            if (tmp == 0) tmp = 4;
            WindowManager::getInstance()->draw(imageCoeur, 16 * tmp, 0, 16, 16, 16 * 9 + 8, 16 * 12);
            break;
        case 442 :
        case 444 :
            if (ready) {
                scene->getPrimes()->draw(10*16, 16*12);
            }
            break;
        default : break;
    }
}
