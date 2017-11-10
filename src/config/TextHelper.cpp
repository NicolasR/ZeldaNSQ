#include "TextHelper.h"

#include "../engine/resources/ResourceManager.h"

#include "../game/MainController.h"

TextHelper::TextHelper() {
    setWSpace(6);
    setWSize(8);
    setHSize(16);
    image = ResourceManager::getInstance()->loadImage("data/images/texts/font.png", true);
    setImage(image);
}

TextHelper::~TextHelper() {
    ResourceManager::getInstance()->free(image);
}

void TextHelper::setLanguage(int i) {
    switch (i) {
        case 0 : setTextfile("data/texts/french.txt"); break;
        case 1 : setTextfile("data/texts/english.txt"); break;
        default : setTextfile("data/texts/english.txt"); break;
    }
}

string TextHelper::getVariableValue(int textId, int varId) {
    ostringstream os;
    int tmp;
    switch (textId) {
        case 11 :
            tmp = 4 - MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->getLink()->getInventory()->nbQuartCoeur();
            os<<tmp;
            return os.str();
        case 443 :
            tmp = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 3 : 5;
            os<<tmp;
            return os.str();
        case 464 : case 486 :
            os << MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->getCoffre(0, 11);
            return os.str();
        case 470 : case 471 : case 472 : case 473 :
            os << MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->getMap()->getGameScore();
            return os.str();
        case 597 :
            os << (7 - MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->getLink()->getInventory()->nbMedaillons());
            return os.str();
        case 659 : case 766 :
            os << MainController::getInstance()->getGameController()->getSave()->getDeaths();
            return os.str();
        case 660 :
            tmp = 0;
            for (int i = 0; i < NB_QUART_COEUR; i++) {
                if (MainController::getInstance()->getGameController()->getSave()->getQuartCoeur(i) != 0) {
                    tmp++;
                }
            }
            os << tmp;
            return os.str();
        case 661 :
            tmp = 0;
            for (int i = 0; i < NB_GEMME_FORCE; i++) {
                if (MainController::getInstance()->getGameController()->getSave()->getGemmeForce(i) != 0) {
                    tmp++;
                }
            }
            os << tmp;
            return os.str();
        case 662 :
            tmp = 0;
            for (int i = 0; i < NB_COQUILLAGES; i++) {
                if (MainController::getInstance()->getGameController()->getSave()->getCoquillages(i) != 0) {
                    tmp++;
                }
            }
            os << tmp;
            return os.str();
        case 663 :
            tmp = 0;
            for (int i = 0; i < NB_TROC; i++) {
                if (MainController::getInstance()->getGameController()->getSave()->getTroc(i) != -1) {
                    tmp++;
                }
            }
            os << tmp;
            return os.str();
        case 664 :
            os << ((MainController::getInstance()->getGameController()->getSave()->getNaviMaxMagic() - 15) / 5);
            return os.str();
        case 665 :

            tmp = 0;
            for (int i = 0; i < NB_EQPMT - 1; i++) {
                if (MainController::getInstance()->getGameController()->getSave()->getEquipment(i) != 0) {
                    tmp++;
                }
            }
            tmp += MainController::getInstance()->getGameController()->getSave()->getBouclier();
            tmp += MainController::getInstance()->getGameController()->getSave()->getTunique() - 1;
            tmp += MainController::getInstance()->getGameController()->getSave()->getEpee();
            if (MainController::getInstance()->getGameController()->getSave()->getEpee() == 5) tmp--;
            if (MainController::getInstance()->getGameController()->getSave()->getMaxMagic() > 32) tmp++;
            tmp += (MainController::getInstance()->getGameController()->getSave()->getMaxArrows() - 30) / 5;
            tmp += (MainController::getInstance()->getGameController()->getSave()->getMaxBombs() - 10) / 2;
            if (MainController::getInstance()->getGameController()->getSave()->getMaxRupees() > 0) tmp++;
            if (MainController::getInstance()->getGameController()->getSave()->getMaxRupees() > 99) tmp++;
            if (MainController::getInstance()->getGameController()->getSave()->getMaxRupees() > 250) tmp++;
            if (MainController::getInstance()->getGameController()->getSave()->getMaxRupees() > 500) tmp++;

            for (int i = 0; i < NB_MEDAILLONS; i++) {
                if (MainController::getInstance()->getGameController()->getSave()->getMedaillons(i) != 0) {
                    tmp++;
                }
            }

            for (int j = 1; j < NB_DONJON; j++) {
                for (int i = 0; i < 3; i++) {
                    if (MainController::getInstance()->getGameController()->getSave()->getCoffre(j, i) != 0) tmp++;
                }
            }

            os << tmp;
            return os.str();
        case 666 :
            os << MainController::getInstance()->getGameController()->getSave()->getCompletion();
            return os.str();
    }
    return "";
}

string TextHelper::getCommonValue(string tag) {
    if (tag == "line") {
        return "                              ";
    }
    return "";
}
