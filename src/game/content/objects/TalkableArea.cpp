#include "TalkableArea.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../MainController.h"
#include "../../game/scene/Scene.h"

TalkableArea::TalkableArea(int i, int j, int txt) : texte(txt) {
    x = i;
    y = j;

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);
}

TalkableArea::~TalkableArea() {
}


void TalkableArea::loop() {}

void TalkableArea::draw(int offsetX, int offsetY) {}

bool TalkableArea::action(Direction dir) {
    MainController::getInstance()->getGameController()->displayText(getDynamicText());
    return true;
}

int TalkableArea::getDynamicText() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    //Navi* navi = scene->getNavi();
    switch (texte) {
        case 341 :
            if (link->getInventory()->hasObject(CARTE_ILE)) {
                return 340;
            }
            break;
        case 344 :
            if (link->getInventory()->hasCoeur(4)) {
                return 340;
            }
            break;
        case 346 :
            if (link->getInventory()->hasObject(DETECTEUR)) {
                return 340;
            }
            break;
        case 350 :
            if (link->getTunique() == 3) {
                return 340;
            }
            if (link->getTunique() == 2) {
                return 353;
            }
            break;
        case 356 :
            if (link->getInventory()->hasObject(CANNE_BYRNA)) {
                return 340;
            }
            break;
        case 361 :
            if (link->getInventory()->hasObject(BOTTES)) {
                return 340;
            }
            break;
        case 364 :
            if (!link->getStatus()->getMaxBombs()) {
                return 367;
            }
            if (link->getStatus()->getBombs() == link->getStatus()->getMaxBombs()) {
                return 366;
            }
            break;
        case 368 :
            if (!link->getStatus()->getMaxArrows()) {
                return 371;
            }
            if (link->getStatus()->getArrows() == link->getStatus()->getMaxArrows()) {
                return 370;
            }
            break;
        case 373 :
            if (link->getStatus()->getVirtualLife() == link->getStatus()->getMaxLife()) {
                return 375;
            }
            break;
        case 376 : case 378 :
            if (link->getStatus()->getVirtualMagic() == link->getStatus()->getMaxMagic() || !link->getStatus()->getMaxMagic()) {
                return 375;
            }
            break;
        case 438 :
            if (scene->getPrimes()->isSuccessful()) {
                return 446;
            }
            if (scene->getPrimes()->isRunning()) {
                return 444;
            }
            if (scene->getCoffre(0, 10)) {
                if (link->getInventory()->hasCoquillages(9)) {
                    return 455;
                }
                if (link->getInventory()->hasCoquillages(8)) {
                    return 453;
                }
                if (link->getInventory()->hasCoquillages(6)) {
                    return 451;
                }
                if (link->getInventory()->hasCoquillages(4)) {
                    return 449;
                }
                if (link->getInventory()->hasCoquillages(3)) {
                    return 447;
                }
                return 440;
            }
            scene->setCoffre(0, 10, 1);
            break;
        case 456 :
            if (link->getInventory()->hasObject(BOMBES)) {
                return 340;
            }
            break;
        case 459 :
            if (link->getInventory()->hasQuartCoeur(15)) {
                return 340;
            }
            break;
        case 545 :
            if (link->getInventory()->hasCoquillages(18)) {
                return 340;
            }
            break;
        case 547 :
            if (link->getInventory()->hasObject(FLACON_3)) {
                return 340;
            }
            break;
        case 567 :
            if (link->getInventory()->hasCoquillages(19)) {
                return 571;
            }
            break;
        default : break;
    }
    return texte;
}
