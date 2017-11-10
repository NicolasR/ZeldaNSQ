#include "MapInitializer.h"

#include "Map.h"

#include "../../MainController.h"

#include "../ennemis/Ennemi001.h"
#include "../ennemis/Ennemi004.h"
#include "../ennemis/Ennemi005.h"
#include "../ennemis/Ennemi006.h"
#include "../ennemis/Ennemi007.h"
#include "../ennemis/Ennemi008.h"
#include "../ennemis/Ennemi009.h"
#include "../ennemis/Ennemi010.h"
#include "../ennemis/Ennemi012.h"
#include "../ennemis/Ennemi013.h"
#include "../ennemis/Ennemi014.h"
#include "../ennemis/Ennemi015.h"
#include "../ennemis/Ennemi018.h"
#include "../ennemis/Ennemi022.h"
#include "../ennemis/Ennemi023.h"
#include "../ennemis/Ennemi024.h"
#include "../ennemis/Ennemi026.h"
#include "../ennemis/Ennemi029.h"
#include "../ennemis/Ennemi030.h"
#include "../ennemis/Ennemi037.h"
#include "../ennemis/Ennemi041.h"
#include "../ennemis/Ennemi042.h"
#include "../ennemis/Ennemi044.h"
#include "../ennemis/Ennemi045.h"
#include "../ennemis/Ennemi046.h"
#include "../ennemis/Ennemi047.h"
#include "../ennemis/Ennemi048.h"
#include "../ennemis/Ennemi053.h"
#include "../ennemis/Ennemi054.h"
#include "../ennemis/Ennemi055.h"
#include "../ennemis/Ennemi056.h"
#include "../ennemis/Ennemi062.h"
#include "../ennemis/Ennemi066.h"
#include "../ennemis/Ennemi076.h"
#include "../ennemis/Ennemi084.h"
#include "../ennemis/Ennemi091.h"
#include "../ennemis/Ennemi092.h"
#include "../ennemis/Ennemi095.h"
#include "../ennemis/Ennemi102.h"
#include "../ennemis/Ennemi110.h"
#include "../ennemis/Ennemi111.h"
#include "../ennemis/Ennemi113.h"
#include "../ennemis/Ennemi114.h"
#include "../ennemis/Ennemi118.h"
#include "../ennemis/Ennemi142.h"
#include "../ennemis/Ennemi143.h"
#include "../ennemis/Ennemi144.h"
#include "../ennemis/Ennemi145.h"
#include "../ennemis/Ennemi146.h"
#include "../ennemis/Ennemi147.h"
#include "../ennemis/Ennemi148.h"
#include "../ennemis/Ennemi149.h"
#include "../ennemis/Ennemi150.h"
#include "../ennemis/Ennemi151.h"
#include "../ennemis/Ennemi152.h"
#include "../ennemis/Ennemi153.h"
#include "../objects/Caisse.h"
#include "../objects/Pancarte.h"
#include "../objects/Pierre.h"
#include "../objects/Coffre.h"
#include "../objects/Sphere.h"
#include "../objects/Switch.h"
#include "../objects/Pnj.h"
#include "../objects/Stele.h"
#include "../objects/Tombe.h"
#include "../objects/Jarre.h"
#include "../objects/Interrupteur.h"
#include "../objects/Statue.h"
#include "../objects/Oeil.h"
#include "../objects/TalkableArea.h"
#include "../objects/Cible.h"
#include "../objects/PiegeLong.h"
#include "../objects/PiegePics.h"
#include "../objects/PiegeFeu.h"
#include "../objects/PiegeMain.h"
#include "../objects/Poule.h"

MapInitializer::MapInitializer(Map* m) : map(m) {
}

MapInitializer::~MapInitializer() {
}


void MapInitializer::addEnnemi(Ennemi* ennemi) {
    map->addEnnemi(ennemi);
}

void MapInitializer::addItem(Item* item) {
    map->addItem(item);
}

void MapInitializer::addObject(Object* object) {
    map->addObject(object);
}

void MapInitializer::ouvrePorte(int x, int y, int type) {
    map->ouvrePorte(x, y, type);
}


void MapInitializer::init() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    Inventory* inventory = link->getInventory();

    if (scene->getAvancement() == AV_GARDE_MOVE) {
        scene->setAvancement(AV_GARDE_OUT);
    }

    int tmp = 0;
    switch (map->getId()) {
        case 1 :
            addObject(new Pierre(16*110, 16*33, 1, map));
            addObject(new Pierre(16*111, 16*32, 1, map));
            addObject(new Pierre(16*112, 16*32, 1, map));
            addObject(new Pierre(16*113, 16*31, 1, map));
            addObject(new Pierre(16*19, 16*17, 1, map));
            addObject(new Pierre(16*28, 16*30, 1, map));
            addObject(new Pierre(16*36, 16*17, 1, map));
            addObject(new Pierre(16*52, 16*26, 1, map));
            addObject(new Pierre(16*53, 16*41, 1, map));
            addObject(new Pierre(16*71, 16*8, 1, map));
            addObject(new Pierre(16*83, 16*22, 1, map));
            addObject(new Pierre(16*84, 16*40, 1, map));
            addObject(new Pierre(16*96, 16*17, 1, map));
            addObject(new Pierre(16*99, 16*46, 1, map));
            addObject(new Pierre(16*22, 16*5, 2, map));
            addObject(new Pierre(16*26, 16*24, 2, map));
            addObject(new Pierre(16*44, 16*29, 2, map));
            addObject(new Pierre(16*86, 16*6, 2, map));
            addObject(new Pancarte(16*21, 16*6, 34, map));
            addObject(new Pancarte(16*108, 16*4, 35, map));
            addObject(new Pancarte(16*112, 16*26, 36, map));

            addObject(new Coffre(16*27, 16*11, 0, inventory->hasObject(FLACON_4), TI_FLACON_4));

            if (!inventory->hasQuartCoeur(0)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 7*16, 51*16, 0));
            }
            if (!inventory->hasQuartCoeur(1)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 64*16 + 8, 50*16 + 8, 1));
            }

            addEnnemi(new Ennemi023(18*16-5,20*16));
            addEnnemi(new Ennemi023(30*16-5,33*16));
            addEnnemi(new Ennemi023(54*16-5,37*16));
            addEnnemi(new Ennemi023(91*16-5,29*16));
            addEnnemi(new Ennemi023(48*16-5,26*16));
            addEnnemi(new Ennemi023(67*16-5,7*16));

            break;
        case 2 :
            addObject(new Pierre(16*38, 16*53, 1, map));
            addObject(new Pierre(16*39, 16*54, 1, map));
            addObject(new Pierre(16*117, 16*50, 1, map));
            addObject(new Pierre(16*116, 16*51, 1, map));
            addObject(new Pierre(16*116, 16*52, 1, map));
            addObject(new Pierre(16*117, 16*53, 1, map));
            addObject(new Pierre(16*26, 16*46, 1, map));
            addObject(new Pierre(16*42, 16*13, 1, map));
            addObject(new Pierre(16*84, 16*41, 1, map));
            addObject(new Pierre(16*110, 16*2, 1, map));

            addObject(new Pierre(16*2, 16*42, 2, map));
            addObject(new Pierre(16*3, 16*43, 2, map));
            addObject(new Pierre(16*3, 16*44, 2, map));
            addObject(new Pierre(16*2, 16*45, 2, map));
            addObject(new Pierre(16*110, 16*55, 2, map));


            addObject(new Pancarte(16*4, 16*40, 41, map));
            addObject(new Pancarte(16*10, 16*2, 42, map));
            addObject(new Pancarte(16*82, 16*2, 43, map));
            addObject(new Pancarte(16*114, 16*49, 44, map));
            addObject(new Pancarte(16*26, 16*55, 45, map));
            addObject(new Pancarte(16*58, 16*55, 46, map));
            addObject(new Pancarte(16*98, 16*55, 46, map));
            addObject(new Pancarte(16*108, 16*14, 47, map));


            if (!inventory->hasQuartCoeur(2)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 107*16 + 8, 47*16, 2));
            }

            if (!inventory->hasGemmeForce(7)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 6*16, 6*16 + 8, 7, false));
            }


            addEnnemi(new Ennemi004(26*16,10*16));
            addEnnemi(new Ennemi004(37*16,9*16));
            addEnnemi(new Ennemi004(44*16,12*16));
            addEnnemi(new Ennemi004(18*16,42*16));
            addEnnemi(new Ennemi004(25*16,36*16));
            addEnnemi(new Ennemi004(29*16,45*16));
            addEnnemi(new Ennemi004(64*16,42*16));
            addEnnemi(new Ennemi004(66*16,34*16));
            addEnnemi(new Ennemi004(70*16,38*16));
            addEnnemi(new Ennemi004(80*16,9*16));
            addEnnemi(new Ennemi004(86*16,48*16));
            addEnnemi(new Ennemi004(103*16,37*16));
            addEnnemi(new Ennemi004(104*16,31*16));
            addEnnemi(new Ennemi004(108*16,33*16));
            break;
        case 3 :
            addObject(new Pierre(16*12, 16*4, 1, map));
            addObject(new Pierre(16*25, 16*41, 1, map));
            addObject(new Pierre(16*53, 16*4, 1, map));

            addObject(new Pierre(16*8, 16*44, 2, map));
            addObject(new Pierre(16*68, 16*4, 2, map));

            addObject(new Pancarte(16*17, 16*3, 35, map));
            addObject(new Pancarte(16*63, 16*3, 35, map));
            addObject(new Pancarte(16*16, 16*37, 65, map));
            addObject(new Pancarte(16*76, 16*46, 66, map));

            addObject(new Sphere(16*73, 16*48));

            if (inventory->hasCoquillages(0)) {
                addObject(new Poule(16*71, 16*33));
                addObject(new Poule(16*71, 16*37));
                addObject(new Poule(16*72, 16*35));
                addObject(new Poule(16*73, 16*37));
                addObject(new Poule(16*74, 16*34));
            } else {
                addObject(new Poule(16*5, 16*18));
                addObject(new Poule(16*32, 16*13));
                addObject(new Poule(16*45, 16*49));
                addObject(new Poule(16*45, 16*8));
                addObject(new Poule(16*66, 16*43));
            }

            addObject(new Pnj(16*69, 16*31, 1, 67));

            addObject(new Pnj(16*13 + 8, 16*24 + 4, 2, 93));
            addObject(new Pnj(16*26 + 8, 16*24 + 6, 3, 107));
            addObject(new Pnj(16*13 + 8, 16*18 + 6, 4, 109));
            addObject(new Pnj(16*26 + 8, 16*18 + 6, 5, 111));
            addObject(new Pnj(16*13 + 8, 16*12 + 5, 6, 113));
            addObject(new Pnj(16*26 + 8, 16*12 + 7, 7, 126));
            addObject(new Pnj(16*20, 16*15, 9, 139));
            addObject(new Pnj(16*44 + 8, 16*5 + 7, 10, 140));
            addObject(new Pnj(16*42 + 8, 16*40 + 18, 11, 141));
            addObject(new Pnj(16*34, 16*28, 12, 143));

            if (scene->getAvancement() < AV_GARDE_OUT) {
                addObject(new Pnj(16*60 - 3, 16*5, 8, 135));
            } else {
                addObject(new Pnj(16*57 - 3, 16*6, 8, 135));
            }

            if (link->getStatus()->getMaxRupees() == 0) {
                map->setSol(12*16,13*16,571);
            } else if (link->getStatus()->getMaxRupees() == 99) {
                map->setSol(12*16,13*16,572);
            } else if (link->getStatus()->getMaxRupees() == 200) {
                map->setSol(12*16,13*16,573);
            } else if (link->getStatus()->getMaxRupees() == 500) {
                map->setSol(12*16,13*16,574);
            }

            if (link->getBouclier() == 0) {
                map->setSol(25*16,13*16,568);
            } else if (link->getBouclier() == 1) {
                map->setSol(25*16,13*16,569);
            } else if (link->getBouclier() == 2) {
                map->setSol(25*16,13*16,570);
            }

            if (!inventory->hasQuartCoeur(3)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 4*16 + 8, 46*16 + 8, 3));
            }

            if (!inventory->hasGemmeForce(11)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 70*16+8, 16*16, 11, false));
            }
            break;
        case 4 :
            addObject(new Pierre(16*4, 16*35, 1, map));
            addObject(new Pierre(16*31, 16*20, 1, map));
            addObject(new Pierre(16*46, 16*42, 1, map));
            addObject(new Pierre(16*60, 16*4, 1, map));

            addObject(new Pierre(16*20, 16*16, 2, map));
            addObject(new Pierre(16*31, 16*40, 2, map));
            addObject(new Pierre(16*44, 16*5, 2, map));

            addObject(new Pancarte(16*4, 16*43, 151, map));
            addObject(new Pancarte(16*25, 16*4, 152, map));
            addObject(new Pancarte(16*74, 16*42, 153, map));
            addObject(new Pancarte(16*76, 16*29, 153, map));

            addEnnemi(new Ennemi054(10*16,10*16,S));
            addEnnemi(new Ennemi054(7*16,25*16,S));
            addEnnemi(new Ennemi054(47*16,5*16,N));
            addEnnemi(new Ennemi054(70*16,22*16,N));

            addEnnemi(new Ennemi009(12*16,16*16));
            addEnnemi(new Ennemi009(51*16,11*16));
            addEnnemi(new Ennemi009(59*16,15*16));
            addEnnemi(new Ennemi009(60*16,23*16));
            addEnnemi(new Ennemi009(60*16,42*16));
            addEnnemi(new Ennemi009(11*16,36*16));

            addEnnemi(new Ennemi010(21*16,54*16));
            addEnnemi(new Ennemi010(25*16,32*16));
            addEnnemi(new Ennemi010(44*16,52*16));
            addEnnemi(new Ennemi010(47*16,28*16));

            if (scene->getCoffre(0, 0)) {
                ouvrePorte(5, 33, 0);
            }
            break;
        case 5 :
            addObject(new Pierre(16*9, 16*39, 1, map));
            addObject(new Pierre(16*24, 16*4, 1, map));
            addObject(new Pierre(16*48, 16*38, 1, map));
            addObject(new Pierre(16*63, 16*5, 1, map));
            addObject(new Pierre(16*63, 16*34, 1, map));
            addObject(new Pierre(16*87, 16*13, 1, map));
            addObject(new Pierre(16*110, 16*13, 1, map));
            addObject(new Pierre(16*110, 16*14, 1, map));
            addObject(new Pierre(16*110, 16*15, 1, map));
            addObject(new Pierre(16*136, 16*5, 1, map));
            addObject(new Pierre(16*138, 16*33, 1, map));

            addObject(new Pierre(16*54, 16*22, 2, map));
            addObject(new Pierre(16*82, 16*31, 2, map));
            addObject(new Pierre(16*100, 16*44, 2, map));
            addObject(new Pierre(16*129, 16*42, 2, map));

            addObject(new Pancarte(16*3, 16*50, 154, map));
            addObject(new Pancarte(16*28, 16*52, 155, map));
            addObject(new Pancarte(16*38, 16*27, 156, map));
            addObject(new Pancarte(16*42, 16*2, 157, map));
            addObject(new Pancarte(16*115, 16*4, 158, map));
            addObject(new Pancarte(16*134, 16*38, 159, map));

            addEnnemi(new Ennemi008(14*16,26*16));
            addEnnemi(new Ennemi008(20*16,48*16));
            addEnnemi(new Ennemi008(54*16,35*16));
            addEnnemi(new Ennemi008(58*16,17*16));
            addEnnemi(new Ennemi008(80*16,15*16));
            addEnnemi(new Ennemi008(96*16,28*16));
            addEnnemi(new Ennemi008(125*16,35*16));

            addEnnemi(new Ennemi053(50*16,47*16));
            addEnnemi(new Ennemi053(59*16,45*16));
            addEnnemi(new Ennemi053(66*16,51*16));
            addEnnemi(new Ennemi053(87*16,45*16));
            addEnnemi(new Ennemi053(74*16,44*16));

            addEnnemi(new Ennemi054(65*16,47*16,N));
            addEnnemi(new Ennemi054(72*16,45*16,N));
            addEnnemi(new Ennemi054(79*16,38*16,N));
            addEnnemi(new Ennemi054(86*16,41*16,N));
            addEnnemi(new Ennemi054(12*16,42*16,S));
            addEnnemi(new Ennemi054(13*16,7*16,S));
            addEnnemi(new Ennemi054(65*16,21*16,N));
            addEnnemi(new Ennemi054(56*16,53*16,S));
            addEnnemi(new Ennemi054(89*16,9*16,S));
            addEnnemi(new Ennemi054(139*16,6*16,N));
            addEnnemi(new Ennemi054(122*16,29*16,N));
            addEnnemi(new Ennemi054(138*16,41*16,N));

            if (!inventory->hasQuartCoeur(4)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 149*16 + 8, 45*16 + 8, 4));
            }

            if (!inventory->hasGemmeForce(21)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 94*16 + 8, 8*16 + 8, 21, false));
            }

            if (scene->getCoffre(0, 1)) {
                ouvrePorte(145, 17, 0);
            }
            break;
        case 6 :
            addObject(new Pierre(16*44, 16*6, 1, map));
            addObject(new Pierre(16*62, 16*30, 1, map));
            addObject(new Pierre(16*65, 16*13, 1, map));

            addObject(new Pierre(16*18, 16*34, 2, map));
            addObject(new Pierre(16*36, 16*14, 2, map));
            addObject(new Pierre(16*42, 16*34, 2, map));

            addObject(new Pancarte(16*4, 16*31, 160, map));
            addObject(new Pancarte(16*16, 16*21, 161, map));
            addObject(new Pancarte(16*40, 16*6, 152, map));
            addObject(new Pancarte(16*4, 16*42, 160, map));

            addEnnemi(new Ennemi006(16*16,23*16));
            addEnnemi(new Ennemi006(22*16,23*16));
            addEnnemi(new Ennemi006(37*16,34*16));
            addEnnemi(new Ennemi006(38*16,7*16));

            addEnnemi(new Ennemi054(42*16,14*16,N));
            addEnnemi(new Ennemi054(42*16,24*16,N));
            addEnnemi(new Ennemi054(9*16,27*16,S));

            addEnnemi(new Ennemi005(52*16,8*16));
            addEnnemi(new Ennemi005(46*16,28*16));
            addEnnemi(new Ennemi005(27*16,35*16));
            addEnnemi(new Ennemi005(63*16,15*16));

            if (!inventory->hasQuartCoeur(5)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 68*16, 40*16 + 8, 5));
            }
            break;
        case 7 :
            addObject(new Pierre(16*31, 16*53, 1, map));
            addObject(new Pierre(16*41, 16*9, 1, map));
            addObject(new Pierre(16*63, 16*39, 1, map));
            addObject(new Pierre(16*75, 16*9, 1, map));

            addObject(new Pierre(16*29, 16*5, 2, map));
            addObject(new Pierre(16*60, 16*4, 2, map));
            addObject(new Pierre(16*61, 16*5, 2, map));
            addObject(new Pierre(16*62, 16*5, 2, map));
            addObject(new Pierre(16*63, 16*5, 2, map));
            addObject(new Pierre(16*64, 16*4, 2, map));

            addObject(new Pancarte(16*39, 16*53, 163, map));
            addObject(new Pancarte(16*64, 16*7, 164, map));
            addObject(new Pancarte(16*68, 16*40, 165, map));

            addObject(new Pnj(16*14 + 8, 16*17 + 5, 13, 166));
            addObject(new Pnj(16*23 + 8, 16*17 + 7, 14, 168));
            addObject(new Pnj(16*14 + 4, 16*12 + 7, 15, 170));
            addObject(new Pnj(16*23 + 8, 16*12 + 7, 16, 172));
            addObject(new Pnj(16*14 + 8, 16*7 + 5, 17, 174));
            addObject(new Pnj(16*23 + 8, 16*7 + 4, 18, 178));

            addObject(new Pnj(16*23, 16*29, 19, 190));
            addObject(new Pnj(16*34, 16*21, 20, 191));
            addObject(new Pnj(16*66, 16*27, 21, 192));

            if (!link->getInventory()->hasObject(FLACON_1)
                || !link->getInventory()->hasObject(FLACON_2)) {
                map->setSol(13*16,8*16,567);
            }

            if (!link->getInventory()->hasGemmeForce(25)
                || !link->getInventory()->hasGemmeForce(26)
                || !link->getInventory()->hasGemmeForce(27)
                || !link->getInventory()->hasGemmeForce(28)
                || !link->getInventory()->hasGemmeForce(29)) {
                map->setSol(22*16,8*16,565);
            } else if (!link->getInventory()->hasCoquillages(1)) {
                map->setSol(22*16,8*16,785);
            }
            break;
        case 8 :
            addObject(new Pierre(16*17, 16*10, 1, map));
            addObject(new Pierre(16*54, 16*38, 1, map));

            addObject(new Pierre(16*54, 16*5, 2, map));
            addObject(new Pierre(16*56, 16*48, 2, map));

            addObject(new Pancarte(16*15, 16*51, 45, map));
            addObject(new Pancarte(16*35, 16*3, 193, map));
            addObject(new Pancarte(16*77, 16*41, 194, map));
            addObject(new Pancarte(16*55, 16*35, 195, map));

            addEnnemi(new Ennemi012(13*16,24*16));
            addEnnemi(new Ennemi012(22*16,40*16));
            addEnnemi(new Ennemi012(31*16,16*16));
            addEnnemi(new Ennemi012(37*16,26*16));
            addEnnemi(new Ennemi012(42*16,40*16));
            addEnnemi(new Ennemi012(48*16,21*16));
            addEnnemi(new Ennemi012(59*16,16*16));
            addEnnemi(new Ennemi012(68*16,20*16));

            if (!inventory->hasQuartCoeur(6)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 64*16 + 8, 48*16 + 8, 6));
            }

            if (!inventory->hasGemmeForce(37)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 75*16, 27*16 + 8, 37, false));
            }
            break;
        case 9 :
            addObject(new Pierre(8*16, 37*16, 1, map));
            addObject(new Pierre(29*16, 55*16, 1, map));
            addObject(new Pierre(45*16, 44*16, 1, map));
            addObject(new Pierre(56*16, 22*16, 1, map));

            addObject(new Pierre(5*16, 8*16, 2, map));
            addObject(new Pierre(18*16, 27*16, 2, map));
            addObject(new Pierre(73*16, 7*16, 2, map));

            addObject(new Pancarte(26*16, 53*16, 196, map));
            addObject(new Pancarte(72*16, 8*16, 197, map));

            addObject(new Stele(58*16, 23*16, 198, inventory->hasObject(CHANT_1)));

            if (scene->getFees(0)) {
                addObject(new Pnj(39*16, 31*16, 22, 215));
            }

            if (scene->getCoffre(0, 2)) {
                ouvrePorte(39, 27, 0);
            }
            break;
        case 10 :
            addObject(new Pierre(5*16, 4*16, 1, map));
            addObject(new Pierre(16*16, 33*16, 1, map));
            addObject(new Pierre(35*16, 51*16, 1, map));
            addObject(new Pierre(37*16, 40*16, 1, map));
            addObject(new Pierre(48*16, 14*16, 1, map));

            addObject(new Pierre(17*16, 24*16, 2, map));
            addObject(new Pierre(46*16, 44*16, 2, map));
            addObject(new Pierre(54*16, 31*16, 2, map));

            addObject(new Pancarte(6*16, 5*16, 256, map));
            addObject(new Pancarte(25*16, 54*16, 257, map));

            addEnnemi(new Ennemi005(29*16,15*16));
            addEnnemi(new Ennemi005(46*16,15*16));
            addEnnemi(new Ennemi005(15*16,42*16));
            addEnnemi(new Ennemi005(48*16,50*16));

            addEnnemi(new Ennemi013(6*16,1*16));
            addEnnemi(new Ennemi013(31*16,1*16));
            addEnnemi(new Ennemi013(46*16,11*16));

            if (!inventory->hasQuartCoeur(7)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 34*16 + 8, 33*16 + 8, 7));
            }

            if (scene->getCoffre(0, 3)) {
                ouvrePorte(15, 2, 0);
            }
            break;
        case 11 :
            addObject(new Pierre(26*16, 74*16, 1, map));
            addObject(new Pierre(58*16, 55*16, 1, map));
            addObject(new Pierre(70*16, 23*16, 1, map));
            addObject(new Pierre(79*16, 32*16, 1, map));
            addObject(new Pierre(87*16, 79*16, 1, map));
            addObject(new Pierre(96*16, 61*16, 1, map));
            addObject(new Pierre(114*16, 25*16, 1, map));
            addObject(new Pierre(117*16, 48*16, 1, map));
            addObject(new Pierre(123*16, 35*16, 1, map));
            addObject(new Pierre(135*16, 83*16, 1, map));
            addObject(new Pierre(145*16, 29*16, 1, map));
            addObject(new Pierre(157*16, 61*16, 1, map));
            addObject(new Pierre(158*16, 78*16, 1, map));
            addObject(new Pierre(168*16, 12*16, 1, map));
            addObject(new Pierre(177*16, 30*16, 1, map));

            addObject(new Pierre(5*16, 13*16, 2, map));
            addObject(new Pierre(58*16, 31*16, 2, map));
            addObject(new Pierre(78*16, 54*16, 2, map));
            addObject(new Pierre(89*16, 51*16, 2, map));
            addObject(new Pierre(97*16, 35*16, 2, map));
            addObject(new Pierre(103*16, 76*16, 2, map));
            addObject(new Pierre(141*16, 69*16, 2, map));
            addObject(new Pierre(145*16, 45*16, 2, map));
            addObject(new Pierre(149*16, 14*16, 2, map));
            addObject(new Pierre(184*16, 56*16, 2, map));

            addObject(new Pancarte(17*16, 38*16, 258, map));
            addObject(new Pancarte(180*16, 76*16, 259, map));

            addEnnemi(new Ennemi014(68*16,27*16));
            addEnnemi(new Ennemi014(75*16,66*16));
            addEnnemi(new Ennemi014(116*16,27*16));
            addEnnemi(new Ennemi014(148*16,35*16));
            addEnnemi(new Ennemi014(163*16,58*16));

            addEnnemi(new Ennemi007(62*16,54*16));
            addEnnemi(new Ennemi007(87*16,24*16));
            addEnnemi(new Ennemi007(99*16,68*16));
            addEnnemi(new Ennemi007(131*16,54*16));
            addEnnemi(new Ennemi007(135*16,24*16));
            addEnnemi(new Ennemi007(151*16,66*16));

            addEnnemi(new Ennemi013(44*16,39*16));
            addEnnemi(new Ennemi013(44*16,67*16));
            addEnnemi(new Ennemi013(71*16,81*16));
            addEnnemi(new Ennemi013(81*16,11*16));
            addEnnemi(new Ennemi013(124*16,11*16));
            addEnnemi(new Ennemi013(124*16,78*16));
            addEnnemi(new Ennemi013(155*16,16*16));
            addEnnemi(new Ennemi013(167*16,31*16));
            addEnnemi(new Ennemi013(182*16,53*16));


            if (!inventory->hasGemmeForce(53)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 14*16, 36*16 + 8, 53, false));
            }

            if (scene->getCoffre(0, 4)) {
                ouvrePorte(20, 69, 0);
            }
            break;
        case 12 :
            addObject(new Pierre(5*16, 46*16, 1, map));
            addObject(new Pierre(75*16, 5*16, 1, map));

            addObject(new Pierre(8*16, 35*16, 2, map));
            addObject(new Pierre(63*16, 54*16, 2, map));

            addObject(new Pancarte(36*16, 49*16, 260, map));
            addObject(new Pancarte(44*16, 4*16, 261, map));
            addObject(new Pancarte(75*16, 48*16, 262, map));

            addObject(new Pnj(16*14 + 8 - 2, 16*19 + 5, 23, 263));
            addObject(new Pnj(16*30 + 8, 16*19 + 6, 24, 266));
            addObject(new Pnj(16*14 + 8, 16*24 + 4, 2, 278));
            addObject(new Pnj(16*30 + 8, 16*24 + 5, 6, 280));
            addObject(new Pnj(16*14 + 8, 16*29 + 7, 25, 274));
            addObject(new Pnj(16*30 + 8, 16*29 + 4, 18, 276));

            addObject(new Pnj(16*41 + 8 - 2, 16*15 + 10, 26, 282));
            addObject(new Pnj(16*22 + 8, 16*23 + 8, 27, 284));
            addObject(new Pnj(16*57, 16*33 + 8, 28, 286));

            if (!link->getInventory()->hasObject(RECETTE_POTION_JAUNE)) {
                map->setSol(13*16,20*16,575);
            }

            if (!inventory->hasQuartCoeur(8)
                || !inventory->hasQuartCoeur(9)
                || !inventory->hasQuartCoeur(10)
                || !inventory->hasQuartCoeur(11)) {
                map->setSol(29*16,20*16,566);
            }
            break;
        case 13 :
            addObject(new Pierre(15*16, 6*16, 1, map));
            addObject(new Pierre(18*16, 55*16, 1, map));
            addObject(new Pierre(26*16, 11*16, 1, map));
            addObject(new Pierre(36*16, 29*16, 1, map));

            addObject(new Pierre(5*16, 52*16, 2, map));
            addObject(new Pierre(55*16, 33*16, 2, map));

            addObject(new Pancarte(16*3, 16*47, 287, map));
            addObject(new Pancarte(16*12, 16*14, 288, map));
            addObject(new Pancarte(16*17, 16*53, 289, map));

            addObject(new Tombe(47*16, 6*16));

            addEnnemi(new Ennemi056(7*16,33*16));
            addEnnemi(new Ennemi056(13*16,46*16));
            addEnnemi(new Ennemi056(22*16,34*16));
            addEnnemi(new Ennemi056(31*16,40*16));

            if (!inventory->hasQuartCoeur(12)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 52*16 + 8, 35*16 + 8, 12));
            }
            break;
        case 14 :
            addObject(new Pierre(16*16, 69*16, 1, map));
            addObject(new Pierre(38*16, 15*16, 1, map));
            addObject(new Pierre(38*16, 68*16, 1, map));
            addObject(new Pierre(41*16, 40*16, 1, map));
            addObject(new Pierre(79*16, 21*16, 1, map));
            addObject(new Pierre(95*16, 78*16, 1, map));
            addObject(new Pierre(104*16, 39*16, 1, map));
            addObject(new Pierre(119*16, 54*16, 1, map));
            addObject(new Pierre(153*16, 72*16, 1, map));

            addObject(new Pierre(32*16, 60*16, 2, map));
            addObject(new Pierre(74*16, 56*16, 2, map));
            addObject(new Pierre(112*16, 33*16, 2, map));
            addObject(new Pierre(155*16, 84*16, 2, map));

            addObject(new Pancarte(16*17, 16*26, 290, map));
            addObject(new Pancarte(16*41, 16*81, 291, map));
            addObject(new Pancarte(16*137, 16*37, 292, map));

            addEnnemi(new Ennemi015(18*16,55*16));
            addEnnemi(new Ennemi015(20*16,76*16));
            addEnnemi(new Ennemi015(34*16,36*16));
            addEnnemi(new Ennemi015(42*16,70*16));
            addEnnemi(new Ennemi015(58*16,56*16));
            addEnnemi(new Ennemi015(61*16,38*16));
            addEnnemi(new Ennemi015(82*16,79*16));
            addEnnemi(new Ennemi015(91*16,38*16));
            addEnnemi(new Ennemi015(102*16,56*16));
            addEnnemi(new Ennemi015(131*16,62*16));

            addEnnemi(new Ennemi055(57*16+5,77*16));
            addEnnemi(new Ennemi055(34*16+5,61*16));
            addEnnemi(new Ennemi055(72*16+5,63*16));
            addEnnemi(new Ennemi055(109*16+5,45*16));
            addEnnemi(new Ennemi055(109*16+5,80*16));

            addEnnemi(new Ennemi084(29*16,69*16,S));
            addEnnemi(new Ennemi084(23*16,43*16,N));
            addEnnemi(new Ennemi084(46*16,33*16,N));
            addEnnemi(new Ennemi084(97*16,67*16,S));

            if (!inventory->hasQuartCoeur(13)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 37*16 + 8, 53*16 + 8, 13));
            }

            if (!inventory->hasQuartCoeur(14)) {
                addItem(ItemHelper::getInstance()->createItem(TI_QUART_COEUR, 109*16 + 8, 28*16 + 8, 14));
            }

            if (scene->getCoffre(0, 5)) {
                ouvrePorte(25, 25, 0);
            }
            break;
        case 15 :
            addObject(new Jarre(8*16, 4*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(9*16, 4*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(10*16, 4*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(11*16, 4*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(8*16, 32*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(9*16, 33*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(10*16, 33*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(11*16, 32*16, 0, TI_PETIT_COEUR, map));

            addObject(new Pancarte(8*16, 36*16, 296, map));
            addObject(new Pancarte(8*16, 18*16, 297, map));

            addObject(new Coffre(9 * 16 + 8, 21 * 16, 0, link->getEpee(), TI_EPEE_1));

            if (scene->getCoffre(0, 6)) {
                ouvrePorte(9, 13, 0);
            }
            break;
        case 16 :
            addObject(new Jarre(4*16, 12*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(15*16, 12*16, 0, TI_NO_ITEM, map));

            if (scene->getFees(1)) {
                addObject(new Pnj(9*16, 3*16 + 8, 22, 298));
            }
            break;
        case 17 :
            addObject(new Jarre(5*16, 12*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(14*16, 12*16, 0, TI_RUBIS_VERT, map));

            if (scene->getFees(2)) {
                addObject(new Pnj(9*16 + 4, 5*16 - 3, 29, 310));
            }
            break;
        case 18 :
            addObject(new Jarre(4*16, 12*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(15*16, 12*16, 0, TI_RUBIS_BLEU, map));

            if (scene->getFees(3)) {
                addObject(new Pnj(9*16, 3*16 + 8, 22, 316));
            }
            break;
        case 19 :
            addObject(new Jarre(5*16, 12*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(14*16, 12*16, 0, TI_RUBIS_VERT, map));

            if (scene->getFees(4)) {
                addObject(new Pnj(9*16 + 4, 5*16 - 3, 29, 310));
            }
            break;
        case 20 :
            addObject(new Jarre(5*16, 19*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(5*16, 25*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(14*16, 19*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(14*16, 25*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(55*16, 4*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(55*16, 10*16, 0, TI_RUBIS_VERT, map));

            addObject(new Sphere(15*16, 2*16));

            addEnnemi(new Ennemi018(27*16,3*16 + 2));
            addEnnemi(new Ennemi018(33*16,3*16 + 2));
            break;
        case 21 :
            addObject(new Jarre(7*16, 20*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(7*16, 24*16, 0, TI_FLECHE, map));
            addObject(new Jarre(12*16, 20*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(12*16, 24*16, 0, TI_BOMBE, map));

            if (!link->getInventory()->hasCoeur(0)) {
                addEnnemi(new Ennemi066(7*16,1*16 + 4, map));
            } else {
                addObject(new Pnj(9*16, 6*16 + 8, 30, 319));
            }
            break;
        case 22 :
            addObject(new Jarre(11*16, 2*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(12*16, 2*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(13*16, 2*16, 0, TI_NO_ITEM, map));

            addEnnemi(new Ennemi018(6*16,20*16 + 2));
            addEnnemi(new Ennemi018(6*16,23*16 + 2));
            addEnnemi(new Ennemi018(13*16,20*16 + 2));
            addEnnemi(new Ennemi018(13*16,23*16 + 2));

            addObject(new Coffre(9 * 16 + 8, 7 * 16, 0, link->getInventory()->hasObject(OCARINA), TI_OCARINA));
            break;
        case 23 :
            addObject(new Jarre(6*16, 35*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(6*16, 39*16, 0, TI_BOMBE, map));
            addObject(new Jarre(13*16, 35*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(13*16, 39*16, 0, TI_BOMBE, map));
            addObject(new Jarre(6*16, 9*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(13*16, 9*16, 0, TI_NO_ITEM, map));

            if (!link->getInventory()->hasCoeur(1)) {
                addEnnemi(new Ennemi142(9*16-3,19*16 - 3));
            } else {
                ouvrePorte(9, 13, 0);
            }

            addObject(new Coffre(9 * 16 + 8, 6 * 16, 0, link->getInventory()->hasObject(MARTEAU), TI_MARTEAU));
            break;
        case 24 :
            addObject(new Jarre(4*16, 12*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(15*16, 12*16, 0, TI_NO_ITEM, map));

            if (scene->getFees(5)) {
                addObject(new Pnj(9*16, 3*16 + 8, 31, 326));
            }
            break;
        case 25 :
            addObject(new Jarre(5*16, 12*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(14*16, 12*16, 0, TI_RUBIS_BLEU, map));

            addObject(new Jarre(30*16, 41*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(31*16, 41*16, 0, TI_NO_ITEM, map));

            addObject(new Jarre(5*16, 34*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(5*16, 40*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(14*16, 34*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(14*16, 40*16, 0, TI_BOMBE, map));

            addObject(new Interrupteur(7*16, 27*16, 0, scene->getCoffre(0, 7), map));

            if (scene->getFees(6)) {
                addObject(new Pnj(9*16 + 4, 5*16 - 3, 29, 310));
            }

            if (scene->getCoffre(0, 7)) {
                ouvrePorte(9, 28, 0);
            }
            break;
        case 26 :
            addObject(new Jarre(5*16, 4*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(6*16, 4*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(13*16, 4*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(14*16, 4*16, 0, TI_BOMBE, map));

            addObject(new Coffre(9 * 16 + 8, 4 * 16, 0, link->getInventory()->hasObject(LANTERNE), TI_LANTERNE));
            break;
        case 27 :
            addObject(new Jarre(7*16, 34*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(12*16, 34*16, 1, TI_FLECHE, map));
            addObject(new Jarre(24*16, 34*16, 1, TI_PETIT_COEUR, map));
            addObject(new Jarre(24*16, 40*16, 1, TI_FLECHE, map));
            addObject(new Jarre(35*16, 34*16, 1, TI_PETIT_COEUR, map));
            addObject(new Jarre(35*16, 40*16, 1, TI_RUBIS_BLEU, map));

            if (!link->getInventory()->hasCoeur(2)) {
                addEnnemi(new Ennemi143(29*16-7,18*16));
            } else {
                ouvrePorte(29, 13, 0);
            }

            addObject(new Coffre(29 * 16 + 8, 7 * 16, 0, link->getInventory()->hasObject(PERLE_LUNE), TI_PERLE_LUNE));
            break;
        case 28 :
            addObject(new Jarre(25*16, 12*16, 1, TI_PETIT_COEUR, map));
            addObject(new Jarre(34*16, 12*16, 1, TI_RUBIS_BLEU, map));

            addObject(new Jarre(5*16, 21*16, 1, TI_RUBIS_VERT, map));
            addObject(new Jarre(7*16, 19*16, 1, TI_NO_ITEM, map));

            addObject(new Jarre(34*16, 19*16, 1, TI_RUBIS_VERT, map));
            addObject(new Jarre(34*16, 25*16, 1, TI_FLECHE, map));

            addObject(new Interrupteur(22*16, 19*16, 0, scene->getCoffre(0, 8), map));

            if (scene->getFees(7)) {
                addObject(new Pnj(29*16 + 4, 5*16 - 3, 29, 310));
            }

            if (scene->getCoffre(0, 8)) {
                ouvrePorte(18, 21, 1);
            }
            break;
        case 29 :
            addObject(new Oeil(7*16 + 4, 17*16 + 4, W, map));

            addObject(new Statue(8*16, 23*16 + 7));
            addObject(new Statue(11*16, 23*16 + 7));

            addObject(new Stele(9*16, 5*16, 198, inventory->hasObject(CHANT_2)));
            break;
        case 30 :

            if (link->getInventory()->hasMedaillons(0)) map->setSol(3*16,10*16,1239);
            if (link->getInventory()->hasMedaillons(1)) map->setSol(3*16,7*16,1242);
            if (link->getInventory()->hasMedaillons(2)) map->setSol(3*16,4*16,1245);
            if (link->getInventory()->hasMedaillons(3)) map->setSol(9*16,4*16,1248);
            if (link->getInventory()->hasMedaillons(4)) map->setSol(16*16,4*16,1251);
            if (link->getInventory()->hasMedaillons(5)) map->setSol(16*16,7*16,1254);
            if (link->getInventory()->hasMedaillons(6)) map->setSol(16*16,10*16,1257);

            if (scene->getCoffre(0, 9)) {
                map->setSol(9*16,7*16,1261);
                map->setSol(10*16,7*16,1261);
            }
            break;
        case 31 :
            addObject(new Pnj(14*16, 8*16, 32, 359));

            addObject(new TalkableArea(5*16, 6*16, 341));
            if (!link->getInventory()->hasObject(CARTE_ILE)) {
                map->setSol(5*16,5*16,1365);
            } else {
                map->setSol(5*16,5*16,1366);
            }

            addObject(new TalkableArea(7*16, 6*16, 344));
            if (!link->getInventory()->hasCoeur(3)) {
                map->setSol(7*16,5*16,1367);
            } else if (!link->getInventory()->hasCoeur(4)) {
                map->setSol(7*16,5*16,1367);
            } else {
                map->setSol(7*16,5*16,1366);
            }

            addObject(new TalkableArea(9*16, 6*16, 346));
            if (!link->getInventory()->hasObject(DETECTEUR)) {
                map->setSol(9*16,5*16,1368);
            } else {
                map->setSol(9*16,5*16,1366);
            }

            addObject(new TalkableArea(11*16, 6*16, 350));
            if (link->getTunique() == 1) {
                map->setSol(11*16,5*16,1369);
            } else if (link->getTunique() == 2) {
                map->setSol(11*16,5*16,1370);
            } else {
                map->setSol(11*16,5*16,1366);
            }

            addObject(new TalkableArea(13*16, 6*16, 356));
            if (!link->getInventory()->hasObject(CANNE_BYRNA)) {
                map->setSol(13*16,5*16,1371);
            } else {
                map->setSol(13*16,5*16,1366);
            }

            break;
        case 32 :
            addObject(new Pnj(14*16, 8*16, 33, 372));

            addObject(new TalkableArea(6*16, 6*16, 361));
            if (!link->getInventory()->hasObject(BOTTES)) {
                map->setSol(6*16,5*16,1372);
            } else {
                map->setSol(6*16,5*16,1366);
            }

            addObject(new TalkableArea(9*16, 6*16, 364));
            map->setSol(9*16,5*16,1373);

            addObject(new TalkableArea(12*16, 6*16, 368));
            if (!link->getInventory()->hasObject(ARC_FEE)) {
                map->setSol(12*16,5*16,1374);
            } else {
                map->setSol(12*16,5*16,1375);
            }
            break;
        case 33 :
            addObject(new Pnj(14*16, 8*16, 34, 372));

            addObject(new TalkableArea(6*16, 6*16, 373));
            map->setSol(6*16,5*16,1376);

            addObject(new TalkableArea(9*16, 6*16, 376));
            map->setSol(9*16,5*16,1377);

            addObject(new TalkableArea(12*16, 6*16, 378));
            map->setSol(12*16,5*16,1378);
            break;
        case 34 :
            addObject(new Pnj(14*16, 6*16, 1, 380));

            addObject(new Coffre(6*16, 6*16, 0, true, TI_NO_ITEM));
            addObject(new Coffre(8*16, 6*16, 0, true, TI_NO_ITEM));
            addObject(new Coffre(10*16, 6*16, 0, true, TI_NO_ITEM));
            addObject(new Coffre(6*16, 8*16, 0, true, TI_NO_ITEM));
            addObject(new Coffre(8*16, 8*16, 0, true, TI_NO_ITEM));
            addObject(new Coffre(10*16, 8*16, 0, true, TI_NO_ITEM));
            addObject(new Coffre(6*16, 10*16, 0, true, TI_NO_ITEM));
            addObject(new Coffre(8*16, 10*16, 0, true, TI_NO_ITEM));
            addObject(new Coffre(10*16, 10*16, 0, true, TI_NO_ITEM));
            break;
        case 35 :
            addObject(new Pnj(6*16+6, 6*16, 35, 395));

            addObject(new Jarre(13*16, 12*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(14*16, 12*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(15*16, 12*16, 0, TI_RUBIS_BLEU, map));
            break;
        case 36 :
            addObject(new Pnj(11*16, 7*16, 8, 410));
            break;
        case 37 :
            addObject(new Pnj(7*16, 5*16, 36, 411));

            addObject(new Jarre(15*16, 11*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(15*16, 12*16, 0, TI_RUBIS_BLEU, map));
            break;
        case 38 :
            addObject(new Pnj(9*16+8, 7*16, 37, 413));

            addObject(new Jarre(4*16, 12*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(5*16, 12*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(6*16, 12*16, 0, TI_PETIT_COEUR, map));
            break;
        case 39 :
            addObject(new Pnj(5*16+8, 4*16 - 2, 38, 434));
            addObject(new TalkableArea(5*16+8, 6*16, 434));

            addObject(new Pnj(13*16, 4*16 + 5, 39, 435));
            addObject(new Pnj(5*16, 9*16, 40, 436));

            addObject(new Jarre(4*16, 12*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(5*16, 12*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(6*16, 12*16, 0, TI_PETIT_COEUR, map));
            break;
        case 40 :
            addObject(new Pnj(5*16+8, 4*16 - 7, 41, 438));
            addObject(new TalkableArea(5*16+8, 6*16, 438));

            addObject(new Pnj(12*16, 5*16 - 7, 42, 437));

            addObject(new Jarre(14*16, 4*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(15*16, 4*16, 0, TI_PETIT_COEUR, map));
            break;
        case 41 :
            addObject(new Pnj(14*16, 8*16, 43, 372));

            addObject(new TalkableArea(6*16, 6*16, 456));
            if (!link->getStatus()->getMaxBombs()) {
                map->setSol(6*16,5*16,1380);
            } else {
                map->setSol(6*16,5*16,1366);
            }

            addObject(new TalkableArea(9*16, 6*16, 364));
            map->setSol(9*16,5*16,1373);

            addObject(new TalkableArea(12*16, 6*16, 368));
            if (!link->getInventory()->hasObject(ARC_FEE)) {
                map->setSol(12*16,5*16,1374);
            } else {
                map->setSol(12*16,5*16,1375);
            }
            break;
        case 42 :
            addObject(new Pnj(14*16, 8*16, 44, 372));

            addObject(new TalkableArea(5*16, 6*16, 459));
            if (!link->getInventory()->hasQuartCoeur(15)) {
                map->setSol(5*16,5*16,1381);
            } else {
                map->setSol(5*16,5*16,1366);
            }

            addObject(new TalkableArea(8*16, 6*16, 373));
            map->setSol(8*16,5*16,1376);

            addObject(new TalkableArea(11*16, 6*16, 376));
            map->setSol(11*16,5*16,1377);

            addObject(new TalkableArea(14*16, 6*16, 378));
            map->setSol(14*16,5*16,1378);
            break;
        case 43 :
            addObject(new Pnj(14*16, 10*16+7, 45, 461));

            addEnnemi(new Cible(1, 0));
            addEnnemi(new Cible(1, 100));
            addEnnemi(new Cible(1, 200));
            addEnnemi(new Cible(1, 300));
            addEnnemi(new Cible(1, 400));
            addEnnemi(new Cible(1, 500));
            addEnnemi(new Cible(2, 600));
            addEnnemi(new Cible(1, 700));
            addEnnemi(new Cible(2, 800));
            addEnnemi(new Cible(1, 900));
            addEnnemi(new Cible(2, 1000));
            addEnnemi(new Cible(1, 1100));
            addEnnemi(new Cible(2, 1200));
            addEnnemi(new Cible(1, 1300));
            addEnnemi(new Cible(2, 1400));

            addEnnemi(new Cible(3, 500));
            addEnnemi(new Cible(3, 1000));
            addEnnemi(new Cible(3, 1500));

            addEnnemi(new Cible(0, 500));
            addEnnemi(new Cible(0, 500 + 64));
            addEnnemi(new Cible(0, 500 + 64*2));
            addEnnemi(new Cible(0, 500 + 64*3));
            break;
        case 44 :
            addObject(new TalkableArea(7*16, 7*16, 476));
            addObject(new Pnj(7*16, 4*16+8, 46, 476));
            addObject(new Pnj(12*16-3, 14*16+5, 47, 477));
            addObject(new Pnj(11*16, 20*16+9, 48, 478));
            addObject(new Pnj(6*16, 12*16, 44, 479));

            addObject(new Jarre(13*16, 27*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(14*16, 27*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(15*16, 27*16, 0, TI_RUBIS_VERT, map));
            break;
        case 45 :
            addObject(new Pnj(8*16, 5*16, 49, 480));
            break;
        case 46 :
            addObject(new Pnj(11*16, 5*16, 50, 488));

            addObject(new Jarre(4*16, 12*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(5*16, 12*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(6*16, 12*16, 0, TI_PETIT_COEUR, map));
            break;
        case 47 :
            addObject(new Pnj(7*16, 6*16, 43, 489));
            break;
        case 48 :
            addObject(new Pnj(9*16 + 2, 7*16-3, 51, 495));
            addObject(new Pnj(6*16, 4*16-1, 52, 514));
            addObject(new Pnj(9*16+8, 4*16-1, 53, 536));
            addObject(new Pnj(13*16, 4*16-1, 54, 542));
            break;
        case 49 :
            addObject(new Pnj(14*16, 8*16, 48, 372));

            addObject(new TalkableArea(6*16, 6*16, 545));
            if (!link->getInventory()->hasCoquillages(18)) {
                map->setSol(6*16,5*16,1408);
            } else {
                map->setSol(6*16,5*16,1366);
            }

            addObject(new TalkableArea(9*16, 6*16, 364));
            map->setSol(9*16,5*16,1373);

            addObject(new TalkableArea(12*16, 6*16, 368));
            if (!link->getInventory()->hasObject(ARC_FEE)) {
                map->setSol(12*16,5*16,1374);
            } else {
                map->setSol(12*16,5*16,1375);
            }
            break;
        case 50 :
            addObject(new Pnj(14*16, 8*16, 55, 372));

            addObject(new TalkableArea(5*16, 6*16, 547));
            if (!link->getInventory()->hasObject(FLACON_3)) {
                map->setSol(5*16,5*16,1409);
            } else {
                map->setSol(5*16,5*16,1366);
            }

            addObject(new TalkableArea(8*16, 6*16, 373));
            map->setSol(8*16,5*16,1376);

            addObject(new TalkableArea(11*16, 6*16, 376));
            map->setSol(11*16,5*16,1377);

            addObject(new TalkableArea(14*16, 6*16, 378));
            map->setSol(14*16,5*16,1378);
            break;
        case 51 :
            addObject(new Pnj(9*16+5, 3*16+5, 47, 549));
            addObject(new Pnj(13*16+8, 8*16+8, 56, 550));
            addObject(new Pnj(5*16+8, 8*16+9, 43, 552));
            addObject(new Pnj(13*16, 4*16+7, 57, 554));
            addObject(new Pnj(6*16, 4*16+7, 35, 556));
            break;
        case 52 :
            addObject(new Pnj(5*16+8, 4*16, 38, 567));
            addObject(new TalkableArea(5*16+8, 6*16, 567));

            addObject(new Pnj(11*16, 7*16, 58, 573));

            addObject(new Jarre(13*16, 12*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(14*16, 12*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(15*16, 12*16, 0, TI_NO_ITEM, map));
            break;
        case 53 :
            addObject(new Pnj(9*16, 5*16, 59, 574));

            addObject(new Jarre(4*16, 12*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(5*16, 12*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(6*16, 12*16, 0, TI_NO_ITEM, map));
            break;
        case 54 :
            addObject(new Pnj(12*16, 5*16, 60, 579));

            addObject(new Jarre(14*16, 12*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(15*16, 12*16, 0, TI_RUBIS_BLEU, map));
            break;
        case 55 :
            addObject(new Pnj(7*16, 6*16, 34, 581));

            addObject(new Jarre(4*16, 12*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(5*16, 12*16, 0, TI_RUBIS_VERT, map));
            break;
        case 56 :
            addObject(new Pnj(12*16, 8*16, 40, 582));
            break;
        case 57 :
            addObject(new Pnj(12*16+8, 6*16, 1, 585));

            addObject(new Jarre(4*16, 12*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(5*16, 12*16, 0, TI_NO_ITEM, map));
            break;
        case 58 :
            if (!scene->getCoffre(1, 31)) {
                addEnnemi(new Ennemi091(272*16+3, 66*16, map));
            }

            if (!inventory->hasCoeur(5)) {
                addEnnemi(new Ennemi144(289*16+3, 17*16+6, map));
            }

            if (!inventory->hasMedaillons(0)) {
                addItem(ItemHelper::getInstance()->createItem(TI_MEDAILLON_1, 290*16, 6*16));
            }

            addObject(new Statue(134*16, 52*16 + 7));
            addObject(new Statue(127*16, 33*16 + 7));
            addObject(new Statue(140*16, 38*16 + 7));
            addObject(new Statue(140*16, 39*16 + 7));
            addObject(new Statue(229*16, 17*16 + 7));
            addObject(new Statue(230*16, 17*16 + 7));

            addObject(new Interrupteur(125*16, 53*16, 1, false, map));
            addObject(new Interrupteur(127*16, 36*16, 1, false, map));
            addObject(new Interrupteur(222*16, 19*16, 1, false, map));

            addObject(new Interrupteur(266*16, 64*16, 1, false, map));
            addObject(new Interrupteur(273*16, 64*16, 1, false, map));
            addObject(new Interrupteur(266*16, 70*16, 1, false, map));
            addObject(new Interrupteur(273*16, 70*16, 1, false, map));

            addObject(new Coffre(149 * 16 + 8, 40 * 16, 0, scene->getCoffre(1, 0), TI_CARTE_DONJON));
            if (scene->getCoffre(1, 1)) {
                map->setSol(144*16,7*16,1412);
                addObject(new Coffre(144 * 16, 7 * 16, 0, true, TI_BOUSSOLE));
            }
            addObject(new Coffre(49 * 16 + 8, 6 * 16, 0, scene->getCoffre(1, 2), TI_CLE_BOSS));

            addObject(new Coffre(289 * 16, 66 * 16 + 8, 1, inventory->hasObject(BOOMERANG), TI_BOOMERANG));

            if (scene->getCoffre(1, 2) == 2) ouvrePorte(289, 28, 0);
            if (inventory->hasCoeur(5)) ouvrePorte(289, 13, 0);

            addObject(new Coffre(29 * 16 + 8, 67 * 16, 0, scene->getCoffre(1, 4), TI_CLE, 4));
            addObject(new Coffre(79 * 16 + 8, 47 * 16, 0, scene->getCoffre(1, 5), TI_CLE, 5));
            addObject(new Coffre(129 * 16 + 8, 52 * 16, 0, scene->getCoffre(1, 7), TI_CLE, 7));
            addObject(new Coffre(159 * 16, 27 * 16, 0, scene->getCoffre(1, 8), TI_CLE, 8));
            addObject(new Coffre(228 * 16 + 8, 22 * 16, 0, scene->getCoffre(1, 9), TI_CLE, 9));
            if (scene->getCoffre(1, 10)) {
                map->setSol(275*16,7*16,1414);
                addObject(new Coffre(275 * 16, 7 * 16, 0, true, TI_CLE, 10));
            }

            if (!scene->getCoffre(1, 6)) {
                addItem(ItemHelper::getInstance()->createItem(TI_CLE, 129*16, 25*16 + 8, 6));
            }

            addObject(new Interrupteur(37*16, 64*16, 0, scene->getCoffre(1, 18), map));
            addObject(new Interrupteur(32*16, 17*16, 0, scene->getCoffre(1, 19), map));
            addObject(new Interrupteur(36*16, 22*16, 0, scene->getCoffre(1, 20), map));
            addObject(new Interrupteur(47*16, 34*16, 0, scene->getCoffre(1, 21), map));
            addObject(new Interrupteur(60*16, 22*16, 0, scene->getCoffre(1, 22), map));
            addObject(new Interrupteur(74*16, 37*16, 0, scene->getCoffre(1, 23), map));
            addObject(new Interrupteur(62*16, 64*16, 0, scene->getCoffre(1, 24), map));
            addObject(new Interrupteur(88*16, 38*16, 0, scene->getCoffre(1, 25), map));
            addObject(new Interrupteur(227*16, 62*16, 0, scene->getCoffre(1, 26), map));
            addObject(new Interrupteur(272*16, 47*16, 0, scene->getCoffre(1, 27), map));
            addObject(new Interrupteur(292*16, 62*16, 0, scene->getCoffre(1, 28), map));
            addObject(new Interrupteur(151*16, 47*16, 0, scene->getCoffre(1, 29), map));
            addObject(new Interrupteur(204*16, 67*16, 0, scene->getCoffre(1, 30), map));

            if (scene->getCoffre(1, 11)) ouvrePorte(18, 6, 1);
            if (scene->getCoffre(1, 12)) ouvrePorte(18, 21, 1);
            if (scene->getCoffre(1, 13)) ouvrePorte(29, 13, 0);
            if (scene->getCoffre(1, 14)) ouvrePorte(69, 58, 0);
            if (scene->getCoffre(1, 15)) ouvrePorte(78, 36, 1);
            if (scene->getCoffre(1, 16)) ouvrePorte(89, 58, 0);
            if (scene->getCoffre(1, 17)) ouvrePorte(258, 66, 1);
            if (scene->getCoffre(1, 18)) ouvrePorte(38, 66, 1);

            tmp = 0;
            if (scene->getCoffre(1, 19)) tmp++;
            if (scene->getCoffre(1, 20)) tmp++;
            if (scene->getCoffre(1, 21)) tmp++;
            if (scene->getCoffre(1, 22)) tmp++;
            if (scene->getCoffre(1, 23)) tmp++;
            while (tmp--) {
                ouvrePorte(49, 32 - 2 * tmp, 2);
            }

            if (scene->getCoffre(1, 24)) ouvrePorte(58, 66, 1);
            if (scene->getCoffre(1, 25)) ouvrePorte(89, 43, 0);
            if (scene->getCoffre(1, 26)) ouvrePorte(229, 58, 0);
            if (scene->getCoffre(1, 27)) ouvrePorte(278, 51, 1);
            if (scene->getCoffre(1, 28)) ouvrePorte(289, 58, 0);
            if (scene->getCoffre(1, 29)) ouvrePorte(158, 51, 1);
            if (scene->getCoffre(1, 30)) ouvrePorte(218, 66, 1);
            if (scene->getCoffre(1, 31)) ouvrePorte(278, 66, 1);

            if (scene->getCoffre(1, 32)) {
                addEnnemi(new Ennemi001(45*16,51*16+6));
                addEnnemi(new Ennemi001(54*16,51*16+6));
                map->setSol(45*16,52*16,1410);
                map->setSol(54*16,52*16,1410);
            }

            addEnnemi(new Ennemi001(269*16+8,51*16+6));
            addEnnemi(new Ennemi001(228*16+8,5*16+6));
            addEnnemi(new Ennemi001(267*16,3*16+6));
            addEnnemi(new Ennemi001(272*16,3*16+6));
            addEnnemi(new Ennemi001(9*16+8,21*16+6));
            addEnnemi(new Ennemi001(109*16+8,21*16+6));
            addEnnemi(new Ennemi001(109*16+8,36*16+6));
            addEnnemi(new Ennemi001(29*16+8,51*16+6));

            addEnnemi(new Ennemi004(146*16,9*16));
            addEnnemi(new Ennemi004(148*16,4*16));
            addEnnemi(new Ennemi004(149*16,11*16));
            addEnnemi(new Ennemi004(28*16,21*16));
            addEnnemi(new Ennemi004(31*16,37*16));
            addEnnemi(new Ennemi004(68*16,23*16));
            addEnnemi(new Ennemi004(9*16+8,37*16));
            addEnnemi(new Ennemi004(26*16,64*16));
            addEnnemi(new Ennemi004(33*16,70*16));
            addEnnemi(new Ennemi004(4*16,20*16));
            addEnnemi(new Ennemi004(126*16,20*16));
            addEnnemi(new Ennemi004(184*16,23*16));
            addEnnemi(new Ennemi004(190*16,26*16));
            addEnnemi(new Ennemi004(191*16,36*16));
            addEnnemi(new Ennemi004(195*16,31*16));
            addEnnemi(new Ennemi004(169*16+8,51*16));
            addEnnemi(new Ennemi004(211*16,22*16));
            addEnnemi(new Ennemi004(211*16,40*16));
            addEnnemi(new Ennemi004(229*16+8,36*16));
            addEnnemi(new Ennemi004(269*16+8,36*16));
            addEnnemi(new Ennemi004(249*16+8,7*16));
            addEnnemi(new Ennemi004(229*16+8,52*16));

            addEnnemi(new Ennemi018(25*16,31*16));
            addEnnemi(new Ennemi018(192*16,41*16));
            addEnnemi(new Ennemi018(195*16,20*16));
            addEnnemi(new Ennemi018(266*16,57*16));
            addEnnemi(new Ennemi018(273*16,57*16));
            addEnnemi(new Ennemi018(170*16,56*16));
            addEnnemi(new Ennemi018(75*16,18*16));
            addEnnemi(new Ennemi018(15*16,28*16));

            addObject(new PiegeLong(236*16, 21*16, S));
            addObject(new PiegeLong(246*16, 36*16, N));
            addObject(new PiegeLong(256*16, 21*16, S));
            addObject(new PiegeLong(146*16, 49*16, S));
            addObject(new PiegeLong(146*16, 63*16, S));
            addObject(new PiegeLong(206*16, 70*16, N));
            addObject(new PiegeLong(246*16, 49*16, S));
            addObject(new PiegeLong(246*16, 70*16, N));

            addObject(new PiegePics(74*16, 53*16, S));
            addObject(new PiegePics(75*16, 52*16, S));
            addObject(new PiegePics(76*16, 51*16, S));
            addObject(new PiegePics(83*16, 53*16, S));
            addObject(new PiegePics(84*16, 52*16, S));
            addObject(new PiegePics(85*16, 51*16, S));

            addObject(new PiegePics(88*16, 26*16, W));
            addObject(new PiegePics(88*16, 29*16, W));
            addObject(new PiegePics(88*16, 32*16, W));
            addObject(new PiegePics(88*16, 35*16, W));

            addObject(new Jarre(5*16, 34*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(5*16, 40*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(25*16, 25*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(26*16, 25*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(27*16, 25*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(42*16, 49*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(57*16, 49*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(42*16, 55*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(57*16, 55*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(64*16, 27*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(65*16, 27*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(66*16, 27*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(107*16, 19*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(107*16, 25*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(107*16, 34*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(107*16, 40*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(138*16, 20*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(139*16, 20*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(140*16, 20*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(141*16, 20*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(142*16, 20*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(144*16, 2*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(155*16, 2*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(144*16, 12*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(155*16, 12*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(172*16, 52*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(172*16, 53*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(182*16, 33*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(183*16, 33*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(184*16, 33*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(185*16, 33*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(205*16, 66*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(205*16, 67*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(205*16, 68*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(208*16, 33*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(209*16, 33*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(264*16, 2*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(275*16, 2*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(264*16, 12*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(275*16, 12*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(270*16, 34*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(271*16, 34*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(272*16, 34*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(282*16, 19*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(297*16, 19*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(282*16, 25*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(297*16, 25*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(285*16, 34*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(294*16, 34*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(285*16, 40*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(294*16, 40*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(297*16, 64*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(297*16, 70*16, 0, TI_PETIT_COEUR, map));
            break;
        case 59 :
            if (!scene->getCoffre(2, 33)) {
                addEnnemi(new Ennemi145(132*16+3, 66*16));
            }

            if (!inventory->hasCoeur(6)) {
                addEnnemi(new Ennemi146(68*16+7, 32*16));
            }

            if (!inventory->hasMedaillons(1)) {
                addItem(ItemHelper::getInstance()->createItem(TI_MEDAILLON_2, 70*16, 21*16));
            }

            addObject(new Coffre(49 * 16 + 8, 127 * 16, 0, scene->getCoffre(2, 0), TI_CARTE_DONJON));

            if (scene->getCoffre(2, 1)) {
                map->setSol(25*16,157*16,1641);
                addObject(new Coffre(25 * 16, 157 * 16, 0, true, TI_BOUSSOLE));
            }
            addObject(new Coffre(89 * 16 + 8, 82 * 16, 0, scene->getCoffre(2, 2), TI_CLE_BOSS));

            addObject(new Coffre(129 * 16, 35 * 16 + 8, 1, inventory->hasObject(GANTS), TI_GANT));

            if (scene->getCoffre(2, 2) == 2) ouvrePorte(69, 43, 0);
            if (inventory->hasCoeur(6)) ouvrePorte(69, 28, 0);

            addObject(new Coffre(9 * 16 + 8, 6 * 16, 0, scene->getCoffre(2, 4), TI_CLE, 4));
            addObject(new Coffre(9 * 16 + 8, 171 * 16, 0, scene->getCoffre(2, 5), TI_CLE, 5));
            addObject(new Coffre(29 * 16 + 8, 67 * 16, 0, scene->getCoffre(2, 6), TI_CLE, 6));
            addObject(new Coffre(29 * 16 + 8, 217 * 16, 0, scene->getCoffre(2, 7), TI_CLE, 7));
            if (scene->getCoffre(2, 8)) {
                map->setSol(54*16,37*16,1645);
                addObject(new Coffre(54 * 16, 37 * 16, 0, true, TI_CLE, 8));
            }
            addObject(new Coffre(69 * 16 + 8, 157 * 16, 0, scene->getCoffre(2, 9), TI_CLE, 9));
            addObject(new Coffre(109 * 16 + 8, 82 * 16, 0, scene->getCoffre(2, 10), TI_CLE, 10));
            if (scene->getCoffre(2, 11)) {
                map->setSol(114*16,157*16,1641);
                addObject(new Coffre(114 * 16, 157 * 16, 0, true, TI_CLE, 11));
            }
            addObject(new Coffre(129 * 16 + 8, 22 * 16, 0, scene->getCoffre(2, 12), TI_CLE, 12));

            addObject(new Caisse(74*16, 124*16, 4));
            addObject(new Caisse(8*16, 187 *16, 4));
            addObject(new Caisse(9*16, 187 *16, 4));
            addObject(new Caisse(10*16, 187 *16, 4));
            addObject(new Caisse(11*16, 187 *16, 4));
            addObject(new Caisse(9*16, 185 *16, 4));
            addObject(new Caisse(10*16, 185 *16, 4));
            addObject(new Caisse(129*16, 41*16, 4));
            addObject(new Caisse(130*16, 41*16, 4));

            addObject(new Pierre(43*16, 21*16, 3, map));
            addObject(new Pierre(43*16, 22*16, 3, map));
            addObject(new Pierre(43*16, 23*16, 3, map));
            addObject(new Pierre(89*16, 18*16, 3, map));
            addObject(new Pierre(90*16, 18*16, 3, map));
            addObject(new Pierre(109*16, 26*16, 3, map));
            addObject(new Pierre(110*16, 26*16, 3, map));
            addObject(new Pierre(69*16, 93*16, 3, map));
            addObject(new Pierre(70*16, 93*16, 3, map));

            addObject(new Interrupteur(65*16, 124*16, 1, false, map));
            addObject(new Interrupteur(136*16, 37*16, 1, false, map));

            addObject(new Interrupteur(37*16, 220*16, 0, scene->getCoffre(2, 22), map));
            addObject(new Interrupteur(12*16, 177*16, 0, scene->getCoffre(2, 23), map));
            addObject(new Interrupteur(32*16, 47*16, 0, scene->getCoffre(2, 24), map));
            addObject(new Interrupteur(32*16, 62*16, 0, scene->getCoffre(2, 25), map));
            addObject(new Interrupteur(27*16, 207*16, 0, scene->getCoffre(2, 26), map));
            addObject(new Interrupteur(47*16, 12*16, 0, scene->getCoffre(2, 27), map));
            addObject(new Interrupteur(62*16, 55*16, 0, scene->getCoffre(2, 28), map));
            addObject(new Interrupteur(72*16, 87*16, 0, scene->getCoffre(2, 29), map));
            addObject(new Interrupteur(82*16, 55*16, 0, scene->getCoffre(2, 30), map));
            addObject(new Interrupteur(97*16, 175*16, 0, scene->getCoffre(2, 31), map));
            addObject(new Interrupteur(107*16, 62*16, 0, scene->getCoffre(2, 32), map));

            if (scene->getCoffre(2, 13)) ouvrePorte(18, 126, 1);
            if (scene->getCoffre(2, 14)) ouvrePorte(38, 81, 1);
            if (scene->getCoffre(2, 15)) ouvrePorte(49, 13, 0);
            if (scene->getCoffre(2, 16)) ouvrePorte(58, 6, 1);
            if (scene->getCoffre(2, 17)) ouvrePorte(49, 193, 0);
            if (scene->getCoffre(2, 18)) ouvrePorte(78, 66, 1);
            if (scene->getCoffre(2, 19)) ouvrePorte(109, 58, 0);
            if (scene->getCoffre(2, 20)) ouvrePorte(118, 66, 1);
            if (scene->getCoffre(2, 21)) ouvrePorte(129, 208, 0);

            if (scene->getCoffre(2, 22)) ouvrePorte(38, 216, 1);
            if (scene->getCoffre(2, 23)) ouvrePorte(9, 163, 0);
            if (scene->getCoffre(2, 24)) ouvrePorte(38, 51, 1);
            if (scene->getCoffre(2, 25)) ouvrePorte(29, 58, 0);
            if (scene->getCoffre(2, 26)) ouvrePorte(18, 201, 1);
            if (scene->getCoffre(2, 27)) ouvrePorte(38, 6, 1);
            if (scene->getCoffre(2, 28)) ouvrePorte(78, 51, 1);
            if (scene->getCoffre(2, 29)) ouvrePorte(78, 81, 1);
            if (scene->getCoffre(2, 30)) ouvrePorte(98, 51, 1);
            if (scene->getCoffre(2, 31)) ouvrePorte(89, 163, 0);
            if (scene->getCoffre(2, 32)) ouvrePorte(98, 66, 1);
            if (scene->getCoffre(2, 33)) ouvrePorte(129, 58, 0);


            addEnnemi(new Ennemi005(69*16+8,201*16));
            addEnnemi(new Ennemi005(29*16+8,201*16));
            addEnnemi(new Ennemi005(28*16,153*16));
            addEnnemi(new Ennemi005(28*16,160*16));
            addEnnemi(new Ennemi005(29*16,141*16));
            addEnnemi(new Ennemi005(65*16,169*16));
            addEnnemi(new Ennemi005(74*16,169*16));
            addEnnemi(new Ennemi005(111*16,153*16));
            addEnnemi(new Ennemi005(111*16,160*16));
            addEnnemi(new Ennemi005(6*16,97*16));
            addEnnemi(new Ennemi005(129*16+8,96*16));
            addEnnemi(new Ennemi005(49*16+8,5*16));
            addEnnemi(new Ennemi005(135*16,6*16));
            addEnnemi(new Ennemi005(87*16,35*16));
            addEnnemi(new Ennemi005(49*16+8,51*16));
            addEnnemi(new Ennemi005(52*16,36*16));
            addEnnemi(new Ennemi005(5*16,21*16));
            addEnnemi(new Ennemi005(69*16+8,66*16));

            addEnnemi(new Ennemi018(92*16,7*16));
            addEnnemi(new Ennemi018(97*16,3*16));
            addEnnemi(new Ennemi018(103*16,11*16));
            addEnnemi(new Ennemi018(112*16,11*16));
            addEnnemi(new Ennemi018(30*16,24*16));
            addEnnemi(new Ennemi018(23*16,30*16));
            addEnnemi(new Ennemi018(36*16,31*16));
            addEnnemi(new Ennemi018(110*16,178*16));
            addEnnemi(new Ennemi018(115*16,168*16));
            addEnnemi(new Ennemi018(130*16,176*16));
            addEnnemi(new Ennemi018(24*16,211*16));
            addEnnemi(new Ennemi018(35*16,211*16));
            addEnnemi(new Ennemi018(64*16,97*16));
            addEnnemi(new Ennemi018(75*16,98*16));
            addEnnemi(new Ennemi018(106*16,73*16));
            addEnnemi(new Ennemi018(113*16,73*16));
            addEnnemi(new Ennemi018(104*16,58*16));

            addEnnemi(new Ennemi053(34*16,179*16));
            addEnnemi(new Ennemi053(44*16,178*16));
            addEnnemi(new Ennemi053(50*16,180*16));
            addEnnemi(new Ennemi053(88*16,187*16));
            addEnnemi(new Ennemi053(86*16,171*16));
            addEnnemi(new Ennemi053(92*16,173*16));
            addEnnemi(new Ennemi053(90*16,156*16));
            addEnnemi(new Ennemi053(119*16,199*16));
            addEnnemi(new Ennemi053(121*16,188*16));
            addEnnemi(new Ennemi053(118*16,123*16));
            addEnnemi(new Ennemi053(120*16,114*16));
            addEnnemi(new Ennemi053(88*16,112*16));
            addEnnemi(new Ennemi053(87*16,79*16));
            addEnnemi(new Ennemi053(91*16,85*16));
            addEnnemi(new Ennemi053(35*16,104*16));
            addEnnemi(new Ennemi053(45*16,103*16));
            addEnnemi(new Ennemi053(128*16,4*16));
            addEnnemi(new Ennemi053(132*16,9*16));
            addEnnemi(new Ennemi053(49*16,33*16));
            addEnnemi(new Ennemi053(50*16,39*16));
            addEnnemi(new Ennemi053(9*16,24*16));
            addEnnemi(new Ennemi053(51*16,22*16));

            addObject(new PiegeLong(106*16, 33*16, S));
            addObject(new PiegeLong(6*16, 53*16, S));
            addObject(new PiegeLong(47*16, 199*16, S));
            addObject(new PiegeLong(86*16, 199*16, S));

            addObject(new PiegePics(118*16, 218*16, S));
            addObject(new PiegePics(121*16, 218*16, S));
            addObject(new PiegePics(124*16, 218*16, S));
            addObject(new PiegePics(113*16, 141*16, S));
            addObject(new PiegePics(117*16, 141*16, S));
            addObject(new PiegePics(121*16, 141*16, S));
            addObject(new PiegePics(125*16, 141*16, S));

            addObject(new Jarre(124*16, 2*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(135*16, 2*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(124*16, 12*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(135*16, 12*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(4*16, 19*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(4*16, 25*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(55*16, 19*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(55*16, 25*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(44*16, 32*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(53*16, 32*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(44*16, 42*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(53*16, 42*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(63*16, 34*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(76*16, 34*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(125*16, 40*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(134*16, 40*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(127*16, 49*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(132*16, 49*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(64*16, 47*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(75*16, 47*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(114*16, 49*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(114*16, 55*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(84*16, 62*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(95*16, 62*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(84*16, 72*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(95*16, 72*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(124*16, 62*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(135*16, 62*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(124*16, 72*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(135*16, 72*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(5*16, 94*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(5*16, 100*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(85*16, 109*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(85*16, 115*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(124*16, 109*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(124*16, 110*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(116*16, 128*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(117*16, 128*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(118*16, 128*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(24*16, 137*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(35*16, 137*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(24*16, 147*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(35*16, 147*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(115*16, 137*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(115*16, 138*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(26*16, 152*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(35*16, 152*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(26*16, 162*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(35*16, 162*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(85*16, 154*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(85*16, 160*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(102*16, 169*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(103*16, 169*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(24*16, 171*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(24*16, 174*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(24*16, 185*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(24*16, 188*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(55*16, 171*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(55*16, 174*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(55*16, 184*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(55*16, 190*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(8*16, 189*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(9*16, 184*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(9*16, 186*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(10*16, 183*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(11*16, 185*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(11*16, 189*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(93*16, 184*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(93*16, 190*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(104*16, 189*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(105*16, 189*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(106*16, 189*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(104*16, 199*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(105*16, 199*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(106*16, 199*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(111*16, 221*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(111*16, 222*16, 0, TI_RUBIS_VERT, map));
            break;
        case 60 :
            if (!scene->getCoffre(3, 28)) {
                addEnnemi(new Ennemi147(149*16, 17*16));
            }

            if (!inventory->hasCoeur(7)) {
                addEnnemi(new Ennemi092(46*16, 62*16));
            }

            if (!inventory->hasMedaillons(2)) {
                addItem(ItemHelper::getInstance()->createItem(TI_MEDAILLON_3, 50*16, 51*16));
            }

            if (scene->getCoffre(3, 0)) {
                map->setSol(156*16,112*16,1837);
                addObject(new Coffre(156 * 16, 112 * 16, 0, true, TI_CARTE_DONJON));
            }

            if (scene->getCoffre(3, 1)) {
                map->setSol(63*16,52*16,1839);
                addObject(new Coffre(63 * 16, 52 * 16, 0, true, TI_BOUSSOLE));
            }
            addObject(new Coffre(189 * 16 + 8, 51 * 16, 0, scene->getCoffre(3, 2), TI_CLE_BOSS));

            addObject(new Coffre(169 * 16, 21 * 16 + 8, 1, inventory->hasObject(ARC), TI_ARC));

            if (scene->getCoffre(3, 2) == 2) ouvrePorte(49, 73, 0);
            if (inventory->hasCoeur(7)) ouvrePorte(49, 58, 0);

            addObject(new Coffre(9 * 16 + 8, 21 * 16, 0, scene->getCoffre(3, 4), TI_CLE, 4));
            addObject(new Coffre(29 * 16 + 8, 52 * 16, 0, scene->getCoffre(3, 5), TI_CLE, 5));
            addObject(new Coffre(69 * 16 + 8, 6 * 16, 0, scene->getCoffre(3, 6), TI_CLE, 6));
            addObject(new Coffre(69 * 16 + 8, 85 * 16, 0, scene->getCoffre(3, 7), TI_CLE, 7));
            addObject(new Coffre(189 * 16 + 8, 84 * 16, 0, scene->getCoffre(3, 8), TI_CLE, 8));
            addObject(new Coffre(209 * 16 + 8, 112 * 16, 0, scene->getCoffre(3, 9), TI_CLE, 9));
            addObject(new Coffre(229 * 16 + 8, 67 * 16, 0, scene->getCoffre(3, 10), TI_CLE, 10));
            addObject(new Coffre(229 * 16 + 8, 112 * 16, 0, scene->getCoffre(3, 11), TI_CLE, 11));

            addObject(new Caisse(7*16, 66*16, 0));
            addObject(new Caisse(120*16, 95 *16, 0));
            addObject(new Caisse(187*16, 63 *16, 0));
            addObject(new Caisse(209*16, 86 *16, 0));
            addObject(new Caisse(210*16, 86 *16, 0));
            addObject(new Caisse(212*16, 73 *16, 0));
            addObject(new Caisse(212*16, 74 *16, 0));
            addObject(new Caisse(212*16, 75*16, 0));
            addObject(new Caisse(212*16, 76*16, 0));
            addObject(new Caisse(218*16, 42*16, 0));

            addObject(new Interrupteur(7*16, 62*16, 2, scene->getCoffre(3, 20), map));
            addObject(new Interrupteur(12*16, 62*16, 2, scene->getCoffre(3, 20), map));
            addObject(new Interrupteur(187*16, 62*16, 2, scene->getCoffre(3, 26), map));
            addObject(new Interrupteur(192*16, 62*16, 2, scene->getCoffre(3, 26), map));
            addObject(new Interrupteur(195*16, 83*16, 2, scene->getCoffre(3, 26), map));
            addObject(new Interrupteur(207*16, 79*16, 2, scene->getCoffre(3, 26), map));
            addObject(new Interrupteur(207*16, 85*16, 2, scene->getCoffre(3, 26), map));

            addObject(new Interrupteur(215*16, 79*16, 1, false, map));

            addObject(new Interrupteur(122*16, 85*16, 0, scene->getCoffre(3, 21), map));
            addObject(new Interrupteur(157*16, 40*16, 0, scene->getCoffre(3, 22), map));
            addObject(new Interrupteur(177*16, 25*16, 0, scene->getCoffre(3, 23), map));
            addObject(new Interrupteur(177*16, 70*16, 0, scene->getCoffre(3, 24), map));
            addObject(new Interrupteur(191*16, 32*16, 0, scene->getCoffre(3, 25), map));
            addObject(new Interrupteur(225*16, 38*16, 0, scene->getCoffre(3, 27), map));

            if (scene->getCoffre(3, 12)) ouvrePorte(38, 111, 1);
            if (scene->getCoffre(3, 13)) ouvrePorte(58, 111, 1);
            if (scene->getCoffre(3, 14)) ouvrePorte(129, 88, 0);
            if (scene->getCoffre(3, 15)) ouvrePorte(138, 51, 1);
            if (scene->getCoffre(3, 16)) ouvrePorte(149, 28, 0);
            if (scene->getCoffre(3, 17)) ouvrePorte(169, 43, 0);
            if (scene->getCoffre(3, 18)) ouvrePorte(178, 36, 1);
            if (scene->getCoffre(3, 19)) ouvrePorte(209, 88, 0);

            if (scene->getCoffre(3, 20)) ouvrePorte(9, 58, 0);
            if (scene->getCoffre(3, 21)) ouvrePorte(118, 81, 1);
            if (scene->getCoffre(3, 22)) ouvrePorte(138, 36, 1);
            if (scene->getCoffre(3, 23)) ouvrePorte(178, 21, 1);
            if (scene->getCoffre(3, 24)) ouvrePorte(178, 66, 1);
            if (scene->getCoffre(3, 25)) ouvrePorte(198, 36, 1);
            if (scene->getCoffre(3, 26)) ouvrePorte(189, 58, 0);
            if (scene->getCoffre(3, 27)) ouvrePorte(229, 43, 0);
            if (scene->getCoffre(3, 28)) ouvrePorte(158, 21, 1);

            if (scene->getCoffre(3, 29)) ouvrePorte(29, 58, 2);
            if (scene->getCoffre(3, 30)) ouvrePorte(69, 58, 2);
            if (scene->getCoffre(3, 31)) ouvrePorte(69, 88, 2);
            if (scene->getCoffre(3, 32)) ouvrePorte(129, 43, 2);
            if (scene->getCoffre(3, 33)) ouvrePorte(129, 73, 2);
            if (scene->getCoffre(3, 34)) ouvrePorte(138, 66, 3);
            if (scene->getCoffre(3, 35)) ouvrePorte(189, 28, 2);
            if (scene->getCoffre(3, 36)) ouvrePorte(198, 21, 3);
            if (scene->getCoffre(3, 37)) ouvrePorte(209, 43, 2);
            if (scene->getCoffre(3, 38)) ouvrePorte(209, 58, 2);

            addObject(new Sphere(164*16, 64*16));

            addEnnemi(new Ennemi001(27*16,49*16+6));
            addEnnemi(new Ennemi001(33*16,54*16+6));
            addEnnemi(new Ennemi001(108*16,28*16+6));
            addEnnemi(new Ennemi001(115*16,22*16+6));
            addEnnemi(new Ennemi001(123*16,24*16+6));
            addEnnemi(new Ennemi001(128*16,30*16+6));
            addEnnemi(new Ennemi001(225*16,53*16+6));
            addEnnemi(new Ennemi001(229*16,84*16+6));
            addEnnemi(new Ennemi001(230*16,95*16+6));
            addEnnemi(new Ennemi001(234*16,54*16+6));

            addEnnemi(new Ennemi007(5*16,19*16));
            addEnnemi(new Ennemi007(14*16,23*16));
            addEnnemi(new Ennemi007(65*16,54*16));
            addEnnemi(new Ennemi007(66*16,49*16));
            addEnnemi(new Ennemi007(75*16,64*16));
            addEnnemi(new Ennemi007(80*16,73*16));
            addEnnemi(new Ennemi007(89*16,69*16));
            addEnnemi(new Ennemi007(108*16,103*16));
            addEnnemi(new Ennemi007(109*16,112*16));
            addEnnemi(new Ennemi007(112*16,107*16));
            addEnnemi(new Ennemi007(117*16,111*16));
            addEnnemi(new Ennemi007(123*16,113*16));
            addEnnemi(new Ennemi007(128*16,106*16));
            addEnnemi(new Ennemi007(130*16,111*16));
            addEnnemi(new Ennemi007(148*16,58*16));
            addEnnemi(new Ennemi007(156*16,51*16));
            addEnnemi(new Ennemi007(169*16,64*16));
            addEnnemi(new Ennemi007(171*16,56*16));
            addEnnemi(new Ennemi007(187*16,21*16));
            addEnnemi(new Ennemi007(188*16,68*16));
            addEnnemi(new Ennemi007(193*16,23*16));
            addEnnemi(new Ennemi007(208*16,29*16));
            addEnnemi(new Ennemi007(209*16,67*16));
            addEnnemi(new Ennemi007(210*16,97*16));
            addEnnemi(new Ennemi007(213*16,22*16));
            addEnnemi(new Ennemi007(220*16,24*16));
            addEnnemi(new Ennemi007(224*16,34*16));
            addEnnemi(new Ennemi007(226*16,29*16));

            addEnnemi(new Ennemi018(3*16,10*16));
            addEnnemi(new Ennemi018(4*16,65*16));
            addEnnemi(new Ennemi018(4*16,98*16));
            addEnnemi(new Ennemi018(4*16,110*16));
            addEnnemi(new Ennemi018(10*16,118*16));
            addEnnemi(new Ennemi018(12*16,103*16));
            addEnnemi(new Ennemi018(15*16,91*16));
            addEnnemi(new Ennemi018(15*16,108*16));
            addEnnemi(new Ennemi018(16*16,8*16));
            addEnnemi(new Ennemi018(36*16,65*16));
            addEnnemi(new Ennemi018(46*16,11*16));
            addEnnemi(new Ennemi018(49*16,3*16));
            addEnnemi(new Ennemi018(53*16,8*16));
            addEnnemi(new Ennemi018(83*16,99*16));
            addEnnemi(new Ennemi018(83*16,106*16));
            addEnnemi(new Ennemi018(86*16,46*16));
            addEnnemi(new Ennemi018(96*16,52*16));
            addEnnemi(new Ennemi018(130*16,63*16));
            addEnnemi(new Ennemi018(133*16,71*16));
            addEnnemi(new Ennemi018(145*16,46*16));
            addEnnemi(new Ennemi018(146*16,68*16));
            addEnnemi(new Ennemi018(158*16,68*16));
            addEnnemi(new Ennemi018(175*16,43*16));
            addEnnemi(new Ennemi018(178*16,50*16));
            addEnnemi(new Ennemi018(178*16,61*16));
            addEnnemi(new Ennemi018(186*16,93*16));
            addEnnemi(new Ennemi018(189*16,101*16));
            addEnnemi(new Ennemi018(193*16,95*16));
            addEnnemi(new Ennemi018(205*16,49*16));
            addEnnemi(new Ennemi018(205*16,53*16));
            addEnnemi(new Ennemi018(207*16,106*16));
            addEnnemi(new Ennemi018(209*16,118*16));
            addEnnemi(new Ennemi018(213*16,93*16));
            addEnnemi(new Ennemi018(214*16,51*16));
            addEnnemi(new Ennemi018(214*16,55*16));
            addEnnemi(new Ennemi018(216*16,38*16));
            addEnnemi(new Ennemi018(222*16,113*16));
            addEnnemi(new Ennemi018(235*16,22*16));
            addEnnemi(new Ennemi018(235*16,33*16));
            addEnnemi(new Ennemi018(237*16,110*16));

            addEnnemi(new Ennemi029(9*16+8,96*16+6));
            addEnnemi(new Ennemi029(29*16+8,6*16+6));
            addEnnemi(new Ennemi029(29*16+8,76*16+6));
            addEnnemi(new Ennemi029(29*16+8,111*16+6));
            addEnnemi(new Ennemi029(68*16+8,69*16+6));
            addEnnemi(new Ennemi029(89*16+8,103*16+6));
            addEnnemi(new Ennemi029(109*16+8,51*16+6));
            addEnnemi(new Ennemi029(129*16+8,81*16+6));
            addEnnemi(new Ennemi029(145*16,88*16+6));
            addEnnemi(new Ennemi029(151*16,108*16+6));
            addEnnemi(new Ennemi029(151*16,114*16+6));
            addEnnemi(new Ennemi029(156*16,94*16+6));
            addEnnemi(new Ennemi029(164*16,79*16+6));
            addEnnemi(new Ennemi029(169*16+8,35*16+6));
            addEnnemi(new Ennemi029(171*16,88*16+6));
            addEnnemi(new Ennemi029(205*16+8,20*16+6));

            addEnnemi(new Ennemi030(9*16+8,36*16+8));
            addEnnemi(new Ennemi030(29*16+8,36*16+8));
            addEnnemi(new Ennemi030(29*16+8,96*16+8));
            addEnnemi(new Ennemi030(69*16+8,111*16+8));
            addEnnemi(new Ennemi030(109*16+8,81*16+8));
            addEnnemi(new Ennemi030(129*16+8,51*16+8));
            addEnnemi(new Ennemi030(149*16+8,36*16+8));
            addEnnemi(new Ennemi030(154*16,111*16+8));
            addEnnemi(new Ennemi030(184*16+8,111*16+8));
            addEnnemi(new Ennemi030(189*16+8,36*16+8));

            addEnnemi(new Ennemi062(9*16+4,48*16-5));
            addEnnemi(new Ennemi062(69*16+4,51*16-5));
            addEnnemi(new Ennemi062(105*16+4,66*16-5));
            addEnnemi(new Ennemi062(113*16+4,66*16-5));
            addEnnemi(new Ennemi062(229*16+4,51*16-5));

            addObject(new PiegeLong(12*16+8, 70*16, S));
            addObject(new PiegeLong(77*16, 76*16, S));
            addObject(new PiegeLong(175*16, 108*16, S));
            addObject(new PiegeLong(187*16, 108*16, S));
            addObject(new PiegeLong(196*16, 68*16, S));

            addObject(new Jarre(7*16, 100*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(22*16, 49*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(22*16, 55*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(33*16, 84*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(33*16, 85*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(37*16, 49*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(37*16, 55*16, 0, TI_BOMBE, map));
            addObject(new Jarre(44*16, 77*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(44*16, 85*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(45*16, 66*16, 0, TI_FLECHE, map));
            addObject(new Jarre(48*16, 66*16, 0, TI_FLECHE, map));
            addObject(new Jarre(51*16, 66*16, 0, TI_FLECHE, map));
            addObject(new Jarre(54*16, 66*16, 0, TI_FLECHE, map));
            addObject(new Jarre(55*16, 77*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(55*16, 85*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(62*16, 49*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(62*16, 55*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(75*16, 92*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(75*16, 102*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(77*16, 49*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(77*16, 55*16, 0, TI_BOMBE, map));
            addObject(new Jarre(93*16, 49*16, 0, TI_BOMBE, map));
            addObject(new Jarre(93*16, 55*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(94*16, 104*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(95*16, 104*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(105*16, 94*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(105*16, 95*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(105*16, 96*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(142*16, 70*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(143*16, 70*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(144*16, 107*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(144*16, 117*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(147*16, 32*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(152*16, 32*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(155*16, 107*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(155*16, 117*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(158*16, 93*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(158*16, 94*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(158*16, 95*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(162*16, 19*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(162*16, 25*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(166*16, 111*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(166*16, 112*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(166*16, 113*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(168*16, 68*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(169*16, 68*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(170*16, 68*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(175*16, 49*16, 0, TI_BOMBE, map));
            addObject(new Jarre(175*16, 50*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(187*16, 57*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(192*16, 57*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(216*16, 79*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(217*16, 79*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(222*16, 49*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(222*16, 55*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(237*16, 49*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(237*16, 55*16, 0, TI_BOMBE, map));
            break;
        case 61 :
            if (!scene->getCoffre(4, 37)) {
                addEnnemi(new Ennemi148(149*16+2-40, 19*16+6-34));
            }

            if (!inventory->hasCoeur(8)) {
                addEnnemi(new Ennemi024(69*16, 21*16, true));
                addEnnemi(new Ennemi024(209*16, 21*16, false));
            }

            if (!inventory->hasMedaillons(3)) {
                addItem(ItemHelper::getInstance()->createItem(TI_MEDAILLON_4, 70*16, 6*16));
                addItem(ItemHelper::getInstance()->createItem(TI_MEDAILLON_4, 210*16, 6*16));
            }

            addObject(new Coffre(47 * 16, 13 * 16, 0, scene->getCoffre(4, 0), TI_CARTE_DONJON));
            addObject(new Coffre(187 * 16, 13 * 16, 0, scene->getCoffre(4, 0), TI_CARTE_DONJON));
            addObject(new Coffre(170 * 16, 95 * 16, 0, scene->getCoffre(4, 1), TI_BOUSSOLE));
            addObject(new Coffre(249 * 16 + 8, 22 * 16, 0, scene->getCoffre(4, 2), TI_CLE_BOSS));

            addObject(new Coffre(9 * 16, 6 * 16 + 8, 1, inventory->hasObject(GRAPPIN), TI_GRAPPIN));
            addObject(new Coffre(149 * 16, 6 * 16 + 8, 1, inventory->hasObject(GRAPPIN), TI_GRAPPIN));

            if (scene->getCoffre(4, 2) == 2) {
                ouvrePorte(69, 28, 0);
                ouvrePorte(209, 28, 0);
            }
            if (inventory->hasCoeur(8)) {
                ouvrePorte(69, 13, 0);
                ouvrePorte(209, 13, 0);
            }

            addObject(new Coffre(149 * 16 + 8, 141 * 16, 0, scene->getCoffre(4, 4), TI_CLE, 4));
            addObject(new Coffre(32 * 16, 126 * 16, 0, scene->getCoffre(4, 5), TI_CLE, 5));
            addObject(new Coffre(172 * 16, 126 * 16, 0, scene->getCoffre(4, 5), TI_CLE, 5));
            addObject(new Coffre(189 * 16 + 8, 141 * 16, 0, scene->getCoffre(4, 6), TI_CLE, 6));
            addObject(new Coffre(49 * 16 + 8, 145 * 16, 0, scene->getCoffre(4, 7), TI_CLE, 7));
            addObject(new Coffre(189 * 16 + 8, 145 * 16, 0, scene->getCoffre(4, 7), TI_CLE, 7));
            addObject(new Coffre(69 * 16 + 8, 141 * 16, 0, scene->getCoffre(4, 8), TI_CLE, 8));
            addObject(new Coffre(209 * 16 + 8, 141 * 16, 0, scene->getCoffre(4, 8), TI_CLE, 8));
            addObject(new Coffre(229 * 16 + 8, 12 * 16, 0, scene->getCoffre(4, 9), TI_CLE, 9));

            if (scene->getCoffre(4, 10)) {
                map->setSol(96*16,82*16,2072);
                map->setSol(236*16,82*16,2072);
                addObject(new Coffre(96 * 16, 82 * 16, 0, true, TI_CLE, 10));
                addObject(new Coffre(236 * 16, 82 * 16, 0, true, TI_CLE, 10));
            }

            addObject(new Coffre(89 * 16 + 8, 127 * 16, 0, scene->getCoffre(4, 11), TI_CLE, 11));
            addObject(new Coffre(229 * 16 + 8, 127 * 16, 0, scene->getCoffre(4, 11), TI_CLE, 11));
            addObject(new Coffre(129 * 16 + 8, 36 * 16, 0, scene->getCoffre(4, 13), TI_CLE, 13));
            addObject(new Coffre(269 * 16 + 8, 36 * 16, 0, scene->getCoffre(4, 13), TI_CLE, 13));
            addObject(new Coffre(269 * 16 + 8, 111 * 16, 0, scene->getCoffre(4, 14), TI_CLE, 14));

            addObject(new Caisse(151*16, 37*16, 2));
            addObject(new Caisse(253*16, 22*16, 2));

            addObject(new Interrupteur(146*16, 36*16, 1, false, map));
            addObject(new Interrupteur(253*16, 21*16, 1, false, map));

            addObject(new Interrupteur(17*16, 4*16, 0, scene->getCoffre(4, 26), map));
            addObject(new Interrupteur(157*16, 4*16, 0, scene->getCoffre(4, 26), map));
            addObject(new Interrupteur(7*16, 117*16, 0, scene->getCoffre(4, 27), map));
            addObject(new Interrupteur(147*16, 117*16, 0, scene->getCoffre(4, 27), map));
            addObject(new Interrupteur(166*16, 23*16, 0, scene->getCoffre(4, 28), map));
            addObject(new Interrupteur(37*16, 115*16, 0, scene->getCoffre(4, 29), map));
            addObject(new Interrupteur(177*16, 115*16, 0, scene->getCoffre(4, 29), map));
            addObject(new Interrupteur(137*16, 160*16, 0, scene->getCoffre(4, 29), map));
            addObject(new Interrupteur(193*16, 68*16, 0, scene->getCoffre(4, 30), map));
            addObject(new Interrupteur(57*16, 175*16, 0, scene->getCoffre(4, 31), map));
            addObject(new Interrupteur(72*16, 137*16, 0, scene->getCoffre(4, 32), map));
            addObject(new Interrupteur(212*16, 137*16, 0, scene->getCoffre(4, 32), map));
            addObject(new Interrupteur(97*16, 115*16, 0, scene->getCoffre(4, 33), map));
            addObject(new Interrupteur(237*16, 115*16, 0, scene->getCoffre(4, 33), map));
            addObject(new Interrupteur(82*16, 175*16, 0, scene->getCoffre(4, 34), map));
            addObject(new Interrupteur(92*16, 182*16, 0, scene->getCoffre(4, 35), map));
            addObject(new Interrupteur(117*16, 4*16, 0, scene->getCoffre(4, 36), map));
            addObject(new Interrupteur(257*16, 4*16, 0, scene->getCoffre(4, 36), map));
            addObject(new Interrupteur(137*16, 169*16, 0, scene->getCoffre(4, 36), map));
            addObject(new Interrupteur(112*16, 32*16, 0, scene->getCoffre(4, 38), map));
            addObject(new Interrupteur(252*16, 32*16, 0, scene->getCoffre(4, 38), map));

            if (scene->getCoffre(4, 15)) {ouvrePorte(18, 36, 1); ouvrePorte(158, 36, 1);}
            if (scene->getCoffre(4, 16)) {ouvrePorte(38, 81, 1); ouvrePorte(178, 81, 1);}
            if (scene->getCoffre(4, 17)) ouvrePorte(49, 208, 0);
            if (scene->getCoffre(4, 18)) {ouvrePorte(58, 36, 1); ouvrePorte(198, 36, 1);}
            if (scene->getCoffre(4, 19)) {ouvrePorte(69, 43, 0); ouvrePorte(209, 43, 0);}
            if (scene->getCoffre(4, 20)) {ouvrePorte(78, 36, 1); ouvrePorte(218, 36, 1);}
            if (scene->getCoffre(4, 21)) {ouvrePorte(98, 51, 1); ouvrePorte(238, 51, 1);}
            if (scene->getCoffre(4, 22)) {ouvrePorte(98, 66, 1); ouvrePorte(238, 66, 1);}
            if (scene->getCoffre(4, 23)) ouvrePorte(89, 193, 0);
            if (scene->getCoffre(4, 24)) {ouvrePorte(109, 43, 0); ouvrePorte(249, 43, 0);}
            if (scene->getCoffre(4, 25)) {ouvrePorte(129, 13, 0); ouvrePorte(269, 13, 0);}

            if (scene->getCoffre(4, 26)) {ouvrePorte(18, 6, 1); ouvrePorte(158, 6, 1);}
            if (scene->getCoffre(4, 27)) {ouvrePorte(9, 103, 0); ouvrePorte(149, 103, 0);}
            if (scene->getCoffre(4, 28)) {ouvrePorte(29, 28, 0); ouvrePorte(169, 28, 0);}
            if (scene->getCoffre(4, 29)) {ouvrePorte(38, 111, 1); ouvrePorte(178, 111, 1); ouvrePorte(138, 156, 1);}
            if (scene->getCoffre(4, 30)) {ouvrePorte(58, 66, 1); ouvrePorte(198, 66, 1);}
            if (scene->getCoffre(4, 31)) ouvrePorte(58, 171, 1);
            if (scene->getCoffre(4, 32)) {ouvrePorte(69, 133, 0); ouvrePorte(209, 133, 0);}
            if (scene->getCoffre(4, 33)) {ouvrePorte(89, 118, 0); ouvrePorte(229, 118, 0);}
            if (scene->getCoffre(4, 34)) ouvrePorte(78, 171, 1);
            if (scene->getCoffre(4, 35)) ouvrePorte(89, 178, 0);
            if (scene->getCoffre(4, 36)) {ouvrePorte(98, 6, 1); ouvrePorte(238, 6, 1); ouvrePorte(118, 171, 1);}
            if (scene->getCoffre(4, 37)) {ouvrePorte(9, 13, 0); ouvrePorte(149, 13, 0);}
            if (scene->getCoffre(4, 38)) {ouvrePorte(109, 28, 0); ouvrePorte(249, 28, 0);}

            addObject(new Switch(34*16, 108*16));
            addObject(new Switch(174*16, 108*16));
            addObject(new Switch(134*16, 153*16));

            addObject(new Switch(109*16+8, 9*16+8));
            addObject(new Switch(249*16+8, 9*16+8));
            addObject(new Switch(129*16+8, 174*16+8));

            addEnnemi(new Ennemi009(69*16+8,163*16));
            addEnnemi(new Ennemi009(29*16+8,200*16));
            addEnnemi(new Ennemi009(148*16,36*16));
            addEnnemi(new Ennemi009(170*16,18*16));
            addEnnemi(new Ennemi009(182*16,7*16));
            addEnnemi(new Ennemi009(192*16,15*16));
            addEnnemi(new Ennemi009(229*16+8,14*16));
            addEnnemi(new Ennemi009(171*16,60*16));
            addEnnemi(new Ennemi009(174*16,51*16));
            addEnnemi(new Ennemi009(188*16,59*16));
            addEnnemi(new Ennemi009(252*16,59*16));
            addEnnemi(new Ennemi009(258*16,63*16));
            addEnnemi(new Ennemi009(261*16,52*16));
            addEnnemi(new Ennemi009(269*16,58*16));
            addEnnemi(new Ennemi009(150*16,86*16));
            addEnnemi(new Ennemi009(162*16,95*16));
            addEnnemi(new Ennemi009(171*16,91*16));
            addEnnemi(new Ennemi009(157*16,127*16));
            addEnnemi(new Ennemi009(158*16,141*16));
            addEnnemi(new Ennemi009(164*16,131*16));
            addEnnemi(new Ennemi009(170*16,134*16));
            addEnnemi(new Ennemi009(193*16,103*16));
            addEnnemi(new Ennemi009(203*16,105*16));
            addEnnemi(new Ennemi009(204*16,111*16));
            addEnnemi(new Ennemi009(249*16,85*16));
            addEnnemi(new Ennemi009(260*16,92*16));
            addEnnemi(new Ennemi009(265*16,82*16));
            addEnnemi(new Ennemi009(269*16+8,112*16));
            addEnnemi(new Ennemi009(255*16,134*16));
            addEnnemi(new Ennemi009(264*16,141*16));
            addEnnemi(new Ennemi009(265*16,127*16));

            addEnnemi(new Ennemi010(9*16+8,37*16));
            addEnnemi(new Ennemi010(29*16+8,19*16));
            addEnnemi(new Ennemi010(42*16,7*16));
            addEnnemi(new Ennemi010(51*16,18*16));
            addEnnemi(new Ennemi010(33*16,61*16));
            addEnnemi(new Ennemi010(40*16,50*16));
            addEnnemi(new Ennemi010(52*16,60*16));
            addEnnemi(new Ennemi010(89*16+8,14*16));
            addEnnemi(new Ennemi010(108*16,59*16));
            addEnnemi(new Ennemi010(118*16,63*16));
            addEnnemi(new Ennemi010(122*16,51*16));
            addEnnemi(new Ennemi010(130*16,58*16));
            addEnnemi(new Ennemi010(9*16,88*16));
            addEnnemi(new Ennemi010(21*16,95*16));
            addEnnemi(new Ennemi010(32*16,90*16));
            addEnnemi(new Ennemi010(9*16+8,128*16));
            addEnnemi(new Ennemi010(21*16,141*16));
            addEnnemi(new Ennemi010(23*16,127*16));
            addEnnemi(new Ennemi010(32*16,134*16));
            addEnnemi(new Ennemi010(52*16,103*16));
            addEnnemi(new Ennemi010(63*16,105*16));
            addEnnemi(new Ennemi010(69*16,112*16));
            addEnnemi(new Ennemi010(107*16,88*16));
            addEnnemi(new Ennemi010(119*16,95*16));
            addEnnemi(new Ennemi010(125*16,81*16));
            addEnnemi(new Ennemi010(129*16+8,112*16));
            addEnnemi(new Ennemi010(112*16,134*16));
            addEnnemi(new Ennemi010(122*16,127*16));
            addEnnemi(new Ennemi010(128*16,142*16));

            addEnnemi(new Ennemi018(42*16,168*16));
            addEnnemi(new Ennemi018(52*16,178*16));
            addEnnemi(new Ennemi018(55*16,166*16));
            addEnnemi(new Ennemi018(84*16,166*16));
            addEnnemi(new Ennemi018(97*16,172*16));
            addEnnemi(new Ennemi018(104*16,196*16));
            addEnnemi(new Ennemi018(108*16,208*16));
            addEnnemi(new Ennemi018(117*16,202*16));
            addEnnemi(new Ennemi018(22*16,218*16));
            addEnnemi(new Ennemi018(37*16,214*16));
            addEnnemi(new Ennemi018(87*16,136*16)); addEnnemi(new Ennemi018(227*16,136*16));
            addEnnemi(new Ennemi018(92*16,148*16)); addEnnemi(new Ennemi018(232*16,148*16));
            addEnnemi(new Ennemi018(64*16,142*16)); addEnnemi(new Ennemi018(204*16,142*16));
            addEnnemi(new Ennemi018(72*16,148*16)); addEnnemi(new Ennemi018(212*16,148*16));
            addEnnemi(new Ennemi018(75*16,136*16)); addEnnemi(new Ennemi018(215*16,136*16));

            addEnnemi(new Ennemi022(47*16,155*16));
            addEnnemi(new Ennemi022(50*16,157*16));
            addEnnemi(new Ennemi022(53*16,154*16));
            addEnnemi(new Ennemi022(107*16,188*16));
            addEnnemi(new Ennemi022(111*16,186*16));
            addEnnemi(new Ennemi022(69*16+8,126*16)); addEnnemi(new Ennemi022(209*16+8,126*16));
            addEnnemi(new Ennemi022(110*16,112*16)); addEnnemi(new Ennemi022(250*16,112*16));
            addEnnemi(new Ennemi022(69*16+8,81*16)); addEnnemi(new Ennemi022(209*16+8,81*16));
            addEnnemi(new Ennemi022(127*16,8*16)); addEnnemi(new Ennemi022(267*16,8*16));
            addEnnemi(new Ennemi022(129*16+8,4*16)); addEnnemi(new Ennemi022(269*16+8,4*16));
            addEnnemi(new Ennemi022(133*16,7*16)); addEnnemi(new Ennemi022(273*16,7*16));
            addEnnemi(new Ennemi022(109*16+8,36*16)); addEnnemi(new Ennemi022(249*16+8,36*16));
            addEnnemi(new Ennemi022(49*16+8,36*16)); addEnnemi(new Ennemi022(189*16+8,36*16));
            addEnnemi(new Ennemi022(89*16+8,51*16)); addEnnemi(new Ennemi022(229*16+8,51*16));
            addEnnemi(new Ennemi022(89*16+8,66*16)); addEnnemi(new Ennemi022(229*16+8,66*16));
            addEnnemi(new Ennemi022(69*16+8,51*16)); addEnnemi(new Ennemi022(209*16+8,51*16));
            addEnnemi(new Ennemi022(89*16+8,36*16)); addEnnemi(new Ennemi022(229*16+8,36*16));

            addEnnemi(new Ennemi149(89*16+4,156*16+1));
            addEnnemi(new Ennemi149(89*16+4,186*16+1));
            addEnnemi(new Ennemi149(49*16+4,201*16+1));
            addEnnemi(new Ennemi149(92*16+4,78*16+1)); addEnnemi(new Ennemi149(232*16+4,78*16+1));
            addEnnemi(new Ennemi149(92*16+4,85*16+1)); addEnnemi(new Ennemi149(232*16+4,85*16+1));
            addEnnemi(new Ennemi149(49*16+4,126*16+1)); addEnnemi(new Ennemi149(189*16+4,126*16+1));
            addEnnemi(new Ennemi149(89*16+4,110*16+1)); addEnnemi(new Ennemi149(229*16+4,110*16+1));
            addEnnemi(new Ennemi149(9*16+4,111*16+1)); addEnnemi(new Ennemi149(149*16+4,111*16+1));
            addEnnemi(new Ennemi149(49*16+4,81*16+1)); addEnnemi(new Ennemi149(189*16+4,81*16+1));
            addEnnemi(new Ennemi149(129*16+4,21*16+1)); addEnnemi(new Ennemi149(269*16+4,21*16+1));
            addEnnemi(new Ennemi149(29*16+4,36*16+1)); addEnnemi(new Ennemi149(169*16+4,36*16+1));
            addEnnemi(new Ennemi149(88*16+4,58*16+1)); addEnnemi(new Ennemi149(228*16+4,58*16+1));
            addEnnemi(new Ennemi149(69*16+4,66*16+1)); addEnnemi(new Ennemi149(209*16+4,66*16+1));



            addObject(new Jarre(2*16, 4*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(2*16, 4*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(2*16, 10*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(2*16, 10*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(34*16, 34*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(174*16, 34*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(34*16, 40*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(174*16, 40*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(45*16, 34*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(185*16, 34*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(45*16, 40*16, 0, TI_RUBIS_BLEU, map)); addObject(new Jarre(185*16, 40*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(74*16, 49*16, 0, TI_FLECHE, map)); addObject(new Jarre(214*16, 49*16, 0, TI_FLECHE, map));
            addObject(new Jarre(74*16, 55*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(214*16, 55*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(74*16, 64*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(214*16, 64*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(74*16, 70*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(214*16, 70*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(94*16, 34*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(234*16, 34*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(94*16, 40*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(234*16, 40*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(124*16, 2*16, 0, TI_FLECHE, map)); addObject(new Jarre(264*16, 2*16, 0, TI_FLECHE, map));
            addObject(new Jarre(124*16, 12*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(264*16, 12*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(135*16, 2*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(275*16, 2*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(135*16, 12*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(275*16, 12*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(5*16, 79*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(145*16, 79*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(6*16, 79*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(146*16, 79*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(7*16, 79*16, 0, TI_FLECHE, map)); addObject(new Jarre(147*16, 79*16, 0, TI_FLECHE, map));
            addObject(new Jarre(8*16, 79*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(148*16, 79*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(9*16, 79*16, 0, TI_RUBIS_BLEU, map)); addObject(new Jarre(149*16, 79*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(10*16, 79*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(150*16, 79*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(27*16, 79*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(167*16, 79*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(27*16, 80*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(167*16, 80*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(27*16, 81*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(167*16, 81*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(5*16, 109*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(145*16, 109*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(5*16, 115*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(145*16, 115*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(9*16, 134*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(149*16, 134*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(10*16, 134*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(150*16, 134*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(9*16, 137*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(149*16, 137*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(10*16, 137*16, 0, TI_RUBIS_ROUGE, map)); addObject(new Jarre(150*16, 137*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(54*16, 79*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(194*16, 79*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(54*16, 85*16, 0, TI_FLECHE, map)); addObject(new Jarre(194*16, 85*16, 0, TI_FLECHE, map));
            addObject(new Jarre(42*16, 124*16, 0, TI_FLECHE, map)); addObject(new Jarre(182*16, 124*16, 0, TI_FLECHE, map));
            addObject(new Jarre(42*16, 130*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(182*16, 130*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(57*16, 124*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(197*16, 124*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(57*16, 130*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(197*16, 130*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(65*16, 79*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(205*16, 79*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(65*16, 85*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(205*16, 85*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(84*16, 77*16, 0, TI_FLECHE, map)); addObject(new Jarre(224*16, 77*16, 0, TI_FLECHE, map));
            addObject(new Jarre(84*16, 87*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(224*16, 87*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(95*16, 77*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(235*16, 77*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(95*16, 87*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(235*16, 87*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(84*16, 92*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(224*16, 92*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(84*16, 102*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(224*16, 102*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(95*16, 92*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(235*16, 92*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(95*16, 102*16, 0, TI_FLECHE, map)); addObject(new Jarre(235*16, 102*16, 0, TI_FLECHE, map));
            addObject(new Jarre(84*16, 107*16, 0, TI_FLECHE, map)); addObject(new Jarre(224*16, 107*16, 0, TI_FLECHE, map));
            addObject(new Jarre(95*16, 107*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(235*16, 107*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(104*16, 79*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(244*16, 79*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(135*16, 79*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(275*16, 79*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(115*16, 86*16, 0, TI_FLECHE, map)); addObject(new Jarre(255*16, 86*16, 0, TI_FLECHE, map));
            addObject(new Jarre(116*16, 86*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(256*16, 86*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(127*16, 88*16, 0, TI_RUBIS_BLEU, map)); addObject(new Jarre(267*16, 88*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(128*16, 88*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(268*16, 88*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(114*16, 109*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(254*16, 109*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(114*16, 115*16, 0, TI_FLECHE, map)); addObject(new Jarre(254*16, 115*16, 0, TI_FLECHE, map));
            addObject(new Jarre(124*16, 117*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(264*16, 117*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(125*16, 117*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(265*16, 117*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(126*16, 117*16, 0, TI_RUBIS_BLEU, map)); addObject(new Jarre(266*16, 117*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(115*16, 145*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(255*16, 145*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(116*16, 145*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(256*16, 145*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(117*16, 145*16, 0, TI_RUBIS_VERT, map)); addObject(new Jarre(257*16, 145*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(135*16, 127*16, 0, TI_PETIT_COEUR, map)); addObject(new Jarre(275*16, 127*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(135*16, 131*16, 0, TI_FLECHE, map)); addObject(new Jarre(275*16, 131*16, 0, TI_FLECHE, map));
            addObject(new Jarre(135*16, 138*16, 0, TI_NO_ITEM, map)); addObject(new Jarre(275*16, 138*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(135*16, 143*16, 0, TI_RUBIS_BLEU, map)); addObject(new Jarre(275*16, 143*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(25*16, 199*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(25*16, 205*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(45*16, 154*16, 0, TI_FLECHE, map));
            addObject(new Jarre(45*16, 160*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(94*16, 154*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(94*16, 160*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(114*16, 184*16, 0, TI_PETIT_COEUR, map));
            addObject(new Jarre(114*16, 190*16, 0, TI_RUBIS_VERT, map));
            break;
        case 62 :
            if (!scene->getCoffre(5, 47)) {
                addEnnemi(new Ennemi113(9*16+2, 96*16+2+8));
                addEnnemi(new Ennemi114(6*16, 96*16+8));
                addEnnemi(new Ennemi114(13*16, 96*16+8));
                addEnnemi(new Ennemi114(8*16, 93*16+8));
                addEnnemi(new Ennemi114(11*16, 93*16+8));
                addEnnemi(new Ennemi114(8*16, 99*16+8));
                addEnnemi(new Ennemi114(11*16, 99*16+8));
            }
            if (!scene->getCoffre(5, 48)) {
                addEnnemi(new Ennemi095(109*16+3, 96*16+7));
                for (int i = 0; i < 8; i++) {
                    addEnnemi(new Ennemi114((106+i)*16, 93*16, false));
                    addEnnemi(new Ennemi114((106+i)*16, 99*16, false));
                }
                for (int i = 0; i < 5; i++) {
                    addEnnemi(new Ennemi114(106*16, (94+i)*16, false));
                    addEnnemi(new Ennemi114(113*16, (94+i)*16, false));
                }
            }
            if (!scene->getCoffre(5, 49)) {
                addEnnemi(new Ennemi095(109*16+3, 21*16+7));
                for (int i = 0; i < 6; i++) {
                    addEnnemi(new Ennemi114((107+i)*16, 19*16, false));
                    addEnnemi(new Ennemi114((107+i)*16, 23*16, false));
                }
                for (int i = 0; i < 3; i++) {
                    addEnnemi(new Ennemi114(107*16, (20+i)*16, false));
                    addEnnemi(new Ennemi114(112*16, (20+i)*16, false));
                }
            }

            if (!inventory->hasCoeur(9)) {
                addEnnemi(new Ennemi045(48*16-2, 32*16+2));
            }

            if (!inventory->hasMedaillons(4)) {
                addItem(ItemHelper::getInstance()->createItem(TI_MEDAILLON_5, 50*16, 21*16));
            }

            addObject(new Coffre(29 * 16 + 8, 81 * 16, 0, scene->getCoffre(5, 0), TI_CARTE_DONJON));
            addObject(new Coffre(69 * 16 + 8, 171 * 16, 0, scene->getCoffre(5, 1), TI_BOUSSOLE));
            addObject(new Coffre(54 * 16, 93 * 16, 0, scene->getCoffre(5, 2), TI_CLE_BOSS));

            addObject(new Coffre(9 * 16, 81 * 16 + 8, 1, inventory->hasObject(BAGUETTE_FEU), TI_BAGUETTE_FEU));
            addObject(new Coffre(109 * 16, 6 * 16 + 8, 1, inventory->hasObject(CANNE_SOMARIA), TI_CANNE_SOMARIA));

            if (scene->getCoffre(5, 2) == 2) ouvrePorte(49, 43, 0);
            if (inventory->hasCoeur(9)) ouvrePorte(49, 28, 0);

            addObject(new Coffre(9 * 16 + 8, 6 * 16, 0, scene->getCoffre(5, 4), TI_CLE, 4));
            addObject(new Coffre(9 * 16 + 8, 111 * 16, 0, scene->getCoffre(5, 5), TI_CLE, 5));
            addObject(new Coffre(9 * 16 + 8, 156 * 16, 0, scene->getCoffre(5, 6), TI_CLE, 6));
            addObject(new Coffre(29 * 16 + 8, 66 * 16, 0, scene->getCoffre(5, 7), TI_CLE, 7));
            addObject(new Coffre(49 * 16 + 8, 66 * 16, 0, scene->getCoffre(5, 8), TI_CLE, 8));
            addObject(new Coffre(69 * 16 + 8, 36 * 16, 0, scene->getCoffre(5, 9), TI_CLE, 9));
            addObject(new Coffre(69 * 16 + 8, 156 * 16, 0, scene->getCoffre(5, 10), TI_CLE, 10));
            addObject(new Coffre(109 * 16 + 8, 66 * 16, 0, scene->getCoffre(5, 11), TI_CLE, 11));
            addObject(new Coffre(109 * 16 + 8, 81 * 16, 0, scene->getCoffre(5, 12), TI_CLE, 12));
            addObject(new Coffre(69 * 16 + 8, 81 * 16, 0, scene->getCoffre(5, 13), TI_RUBIS_ROUGE, 13));

            addObject(new Caisse(38*16, 174*16, 0));

            addObject(new Interrupteur(27*16, 17*16, 0, scene->getCoffre(5, 24), map));
            addObject(new Interrupteur(32*16, 62*16, 0, scene->getCoffre(5, 26), map));
            addObject(new Interrupteur(35*16, 87*16, 0, scene->getCoffre(5, 27), map));
            addObject(new Interrupteur(102*16, 10*16, 0, scene->getCoffre(5, 28), map));
            addObject(new Interrupteur(7*16, 192*16, 0, scene->getCoffre(5, 29), map));
            addObject(new Interrupteur(87*16, 128*16, 0, scene->getCoffre(5, 33), map));
            addObject(new Interrupteur(62*16, 220*16, 0, scene->getCoffre(5, 35), map));
            addObject(new Interrupteur(57*16, 145*16, 0, scene->getCoffre(5, 36), map));
            addObject(new Interrupteur(84*16, 87*16, 0, scene->getCoffre(5, 38), map));
            addObject(new Interrupteur(115*16, 87*16, 0, scene->getCoffre(5, 39), map));
            addObject(new Interrupteur(42*16, 4*16, 0, scene->getCoffre(5, 40), map));
            addObject(new Interrupteur(77*16, 70*16, 0, scene->getCoffre(5, 41), map));
            addObject(new Interrupteur(22*16, 186*16, 0, scene->getCoffre(5, 42), map));

            if (scene->getCoffre(5, 14)) ouvrePorte(18, 96, 1);
            if (scene->getCoffre(5, 15)) ouvrePorte(18, 171, 1);
            if (scene->getCoffre(5, 16)) ouvrePorte(18, 186, 1);
            if (scene->getCoffre(5, 17)) ouvrePorte(38, 51, 1);
            if (scene->getCoffre(5, 18)) ouvrePorte(38, 201, 1);
            if (scene->getCoffre(5, 19)) ouvrePorte(58, 6, 1);
            if (scene->getCoffre(5, 20)) ouvrePorte(49, 88, 0);
            if (scene->getCoffre(5, 21)) ouvrePorte(78, 96, 1);
            if (scene->getCoffre(5, 22)) ouvrePorte(98, 96, 1);

            if (scene->getCoffre(5, 23)) ouvrePorte(9, 13, 0);
            if (scene->getCoffre(5, 24)) ouvrePorte(29, 13, 0);
            if (scene->getCoffre(5, 25)) ouvrePorte(18, 81, 1);
            if (scene->getCoffre(5, 26)) ouvrePorte(29, 58, 0);
            if (scene->getCoffre(5, 27)) ouvrePorte(29, 88, 0);
            if (scene->getCoffre(5, 28)) ouvrePorte(98, 6, 1);
            if (scene->getCoffre(5, 29)) ouvrePorte(9, 178, 0);
            if (scene->getCoffre(5, 30)) ouvrePorte(18, 201, 1);
            if (scene->getCoffre(5, 31)) ouvrePorte(69, 178, 0);
            if (scene->getCoffre(5, 32)) ouvrePorte(78, 156, 1);
            if (scene->getCoffre(5, 33)) ouvrePorte(78, 126, 1);
            if (scene->getCoffre(5, 34)) ouvrePorte(49, 208, 0);
            if (scene->getCoffre(5, 35)) ouvrePorte(58, 216, 1);
            if (scene->getCoffre(5, 36)) ouvrePorte(58, 141, 1);
            if (scene->getCoffre(5, 37)) ouvrePorte(78, 111, 1);
            if (scene->getCoffre(5, 38)) ouvrePorte(89, 88, 0);
            if (scene->getCoffre(5, 39)) ouvrePorte(109, 88, 0);
            if (scene->getCoffre(5, 40)) ouvrePorte(38, 6, 1);
            if (scene->getCoffre(5, 41)) ouvrePorte(78, 66, 1);
            if (scene->getCoffre(5, 42)) ouvrePorte(38, 186, 1);
            if (scene->getCoffre(5, 43)) ouvrePorte(69, 88, 0);
            if (scene->getCoffre(5, 44)) ouvrePorte(109, 58, 0);
            if (scene->getCoffre(5, 45)) ouvrePorte(69, 28, 0);
            if (scene->getCoffre(5, 46)) ouvrePorte(69, 79, 2);
            if (scene->getCoffre(5, 47)) ouvrePorte(9, 88, 0);
            if (scene->getCoffre(5, 48)) ouvrePorte(106, 94, 3);
            if (scene->getCoffre(5, 49)) {ouvrePorte(109, 13, 0); ouvrePorte(107, 20, 4);}

            // torches
            if (scene->getCoffre(5, 23)) {
                map->setSol(15*16,23*16,972); map->setSol(24*16,23*16,972); map->setSol(15*16,28*16,972); map->setSol(24*16,28*16,972);
                map->setSol(15*16,35*16,972); map->setSol(15*16,40*16,972); map->setSol(24*16,40*16,972);
            }
            if (scene->getCoffre(5, 25)) {
                map->setSol(4*16,77*16,972); map->setSol(15*16,77*16,972); map->setSol(4*16,87*16,972); map->setSol(15*16,87*16,972);
            }
            if (scene->getCoffre(5, 30)) {
                map->setSol(37*16,197*16,972); map->setSol(37*16,207*16,972);
            }
            if (scene->getCoffre(5, 31)) {
                map->setSol(55*16,190*16,972); map->setSol(64*16,190*16,972); map->setSol(55*16,199*16,972); map->setSol(64*16,199*16,972);
            }
            if (scene->getCoffre(5, 32)) {
                map->setSol(82*16,154*16,972); map->setSol(82*16,160*16,972); map->setSol(84*16,171*16,972); map->setSol(84*16,175*16,972);
                map->setSol(89*16,164*16,972); map->setSol(95*16,164*16,972); map->setSol(89*16,172*16,972); map->setSol(95*16,172*16,972);
                map->setSol(108*16,157*16,972); map->setSol(112*16,157*16,972); map->setSol(108*16,167*16,972); map->setSol(112*16,167*16,972);
                map->setSol(107*16,177*16,972); map->setSol(112*16,177*16,972);
            }
            if (scene->getCoffre(5, 34)) {
                map->setSol(47*16,213*16,972); map->setSol(52*16,213*16,972);
            }
            if (scene->getCoffre(5, 37)) {
                map->setSol(88*16,131*16,972); map->setSol(91*16,131*16,972); map->setSol(108*16,127*16,972); map->setSol(111*16,127*16,972);
                map->setSol(103*16,116*16,972); map->setSol(106*16,116*16,972); map->setSol(103*16,119*16,972); map->setSol(106*16,119*16,972);
            }
            if (scene->getCoffre(5, 43)) {
                map->setSol(49*16,94*16,972); map->setSol(49*16,106*16,972); map->setSol(49*16,118*16,972); map->setSol(49*16,130*16,972);
                map->setSol(70*16,94*16,972); map->setSol(70*16,106*16,972); map->setSol(70*16,118*16,972); map->setSol(70*16,130*16,972);
            }
            if (scene->getCoffre(5, 44)) {
                map->setSol(86*16,43*16,972); map->setSol(94*16,37*16,972); map->setSol(95*16,49*16,972); map->setSol(99*16,44*16,972);
                map->setSol(104*16,39*16,972); map->setSol(105*16,51*16,972); map->setSol(110*16,45*16,972); map->setSol(115*16,37*16,972);
            }
            if (scene->getCoffre(5, 45)) {
                map->setSol(76*16,16*16,972); map->setSol(83*16,16*16,972); map->setSol(76*16,25*16,972); map->setSol(83*16,25*16,972);
            }
            if (scene->getCoffre(5, 48)) {
                map->setSol(102*16,92*16,972); map->setSol(117*16,92*16,972); map->setSol(102*16,102*16,972); map->setSol(117*16,102*16,972);
            }
            if (scene->getCoffre(5, 49)) {
                map->setSol(102*16,17*16,972); map->setSol(117*16,17*16,972); map->setSol(102*16,27*16,972); map->setSol(117*16,27*16,972);
            }

            addObject(new PiegeLong(6*16, 47*16, S));
            addObject(new PiegeLong(76*16, 17*16, S));

            addObject(new PiegeFeu(22*16, 197*16));
            addObject(new PiegeFeu(22*16, 207*16));
            addObject(new PiegeFeu(102*16, 205*16));
            addObject(new PiegeFeu(117*16, 205*16));
            addObject(new PiegeFeu(2*16, 220*16));
            addObject(new PiegeFeu(2*16, 124*16));
            addObject(new PiegeFeu(17*16, 124*16));
            addObject(new PiegeFeu(42*16, 62*16));
            addObject(new PiegeFeu(42*16, 72*16));
            addObject(new PiegeFeu(62*16, 42*16));
            addObject(new PiegeFeu(77*16, 42*16));
            addObject(new PiegeFeu(2*16, 2*16));
            addObject(new PiegeFeu(17*16, 2*16));
            addObject(new PiegeFeu(22*16, 2*16));
            addObject(new PiegeFeu(44*16, 32*16));
            addObject(new PiegeFeu(55*16, 32*16));

            addEnnemi(new Ennemi026(92*16,41*16));
            addEnnemi(new Ennemi026(103*16,47*16));
            addEnnemi(new Ennemi026(109*16,36*16));
            addEnnemi(new Ennemi026(6*16,27*16));
            addEnnemi(new Ennemi026(13*16,20*16));
            addEnnemi(new Ennemi026(20*16,39*16));
            addEnnemi(new Ennemi026(26*16,20*16));
            addEnnemi(new Ennemi026(34*16,31*16));
            addEnnemi(new Ennemi026(4*16,113*16));
            addEnnemi(new Ennemi026(15*16,111*16));
            addEnnemi(new Ennemi026(85*16,93*16));
            addEnnemi(new Ennemi026(90*16,101*16));
            addEnnemi(new Ennemi026(94*16,93*16));
            addEnnemi(new Ennemi026(6*16,141*16));
            addEnnemi(new Ennemi026(13*16,138*16));
            addEnnemi(new Ennemi026(14*16,146*16));
            addEnnemi(new Ennemi026(55*16,136*16));
            addEnnemi(new Ennemi026(82*16,121*16));
            addEnnemi(new Ennemi026(4*16,203*16));
            addEnnemi(new Ennemi026(15*16,198*16));
            addEnnemi(new Ennemi026(41*16,171*16));
            addEnnemi(new Ennemi026(83*16,163*16));
            addEnnemi(new Ennemi026(88*16,154*16));
            addEnnemi(new Ennemi026(98*16,154*16));
            addEnnemi(new Ennemi026(106*16,183*16));
            addEnnemi(new Ennemi026(113*16,186*16));
            addEnnemi(new Ennemi026(106*16,191*16));
            addEnnemi(new Ennemi026(89*16+8,181*16));
            addEnnemi(new Ennemi026(8*16,166*16));
            addEnnemi(new Ennemi026(15*16,178*16));
            addEnnemi(new Ennemi026(34*16,178*16));
            addEnnemi(new Ennemi026(55*16,174*16));

            addEnnemi(new Ennemi037(5*16,215*16+6));
            addEnnemi(new Ennemi037(12*16,220*16+6));
            addEnnemi(new Ennemi037(4*16,128*16+6));
            addEnnemi(new Ennemi037(15*16,127*16+6));
            addEnnemi(new Ennemi037(49*16+8,142*16+6));
            addEnnemi(new Ennemi037(89*16+8,216*16+6));
            addEnnemi(new Ennemi037(92*16,167*16+6));
            addEnnemi(new Ennemi037(110*16,160*16+6));
            addEnnemi(new Ennemi037(9*16+8,186*16+6));
            addEnnemi(new Ennemi037(29*16+8,96*16+6));
            addEnnemi(new Ennemi037(69*16+8,51*16+6));
            addEnnemi(new Ennemi037(51*16,66*16+6));
            addEnnemi(new Ennemi037(29*16+8,201*16+6));

            addEnnemi(new Ennemi062(109*16+4,201*16-5));
            addEnnemi(new Ennemi062(9*16+4,217*16-5));
            addEnnemi(new Ennemi062(9*16+4,126*16-5));

            addEnnemi(new Ennemi118(49*16+8,216*16));
            addEnnemi(new Ennemi118(27*16,173*16));
            addEnnemi(new Ennemi118(31*16,171*16));
            addEnnemi(new Ennemi118(29*16,157*16));
            addEnnemi(new Ennemi118(44*16,156*16));
            addEnnemi(new Ennemi118(48*16,173*16));
            addEnnemi(new Ennemi118(9*16+8,172*16));
            addEnnemi(new Ennemi118(92*16,158*16));
            addEnnemi(new Ennemi118(101*16+8,164*16));
            addEnnemi(new Ennemi118(105*16,172*16));
            addEnnemi(new Ennemi118(88*16,201*16));
            addEnnemi(new Ennemi118(92*16,186*16));
            addEnnemi(new Ennemi118(51*16,195*16));
            addEnnemi(new Ennemi118(59*16,200*16));
            addEnnemi(new Ennemi118(60*16,189*16));
            addEnnemi(new Ennemi118(69*16,194*16));
            addEnnemi(new Ennemi118(65*16,80*16));
            addEnnemi(new Ennemi118(72*16,79*16));
            addEnnemi(new Ennemi118(73*16,84*16));
            addEnnemi(new Ennemi118(97*16,123*16));
            addEnnemi(new Ennemi118(98*16,114*16));
            addEnnemi(new Ennemi118(105*16,128*16));
            addEnnemi(new Ennemi118(43*16,103*16));
            addEnnemi(new Ennemi118(49*16,113*16));
            addEnnemi(new Ennemi118(52*16,99*16));
            addEnnemi(new Ennemi118(60*16,125*16));
            addEnnemi(new Ennemi118(68*16,113*16));
            addEnnemi(new Ennemi118(72*16,128*16));
            addEnnemi(new Ennemi118(75*16,102*16));
            addEnnemi(new Ennemi118(67*16,69*16));
            addEnnemi(new Ennemi118(72*16,67*16));

            addObject(new Jarre(42*16, 17*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(57*16, 17*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(66*16, 49*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(66*16, 55*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(24*16, 77*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(35*16, 77*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(45*16, 139*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(45*16, 145*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(107*16, 132*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(112*16, 132*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(114*16, 109*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(115*16, 109*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(5*16, 169*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(5*16, 175*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(5*16, 184*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(5*16, 190*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(24*16, 177*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(31*16, 177*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(44*16, 190*16, 0, TI_FLECHE, map));
            addObject(new Jarre(44*16, 199*16, 0, TI_BOMBE, map));
            addObject(new Jarre(75*16, 190*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(75*16, 199*16, 0, TI_FLECHE, map));
            addObject(new Jarre(49*16, 186*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(49*16, 203*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(70*16, 186*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(70*16, 203*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(86*16, 193*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(93*16, 193*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(94*16, 214*16, 0, TI_MAGIE_PEU, map));
            addObject(new Jarre(94*16, 220*16, 0, TI_RUBIS_VERT, map));
            break;
        case 63 :
            if (!scene->getCoffre(6, 27)) {
                addEnnemi(new Ennemi102(69*16+4, 94*16+5));
            }

            if (!inventory->hasCoeur(10)) {
                addEnnemi(new Ennemi143(49*16-7,18*16));
            }

            if (!inventory->hasMedaillons(5)) {
                addItem(ItemHelper::getInstance()->createItem(TI_MEDAILLON_6, 50*16, 6*16));
            }

            addObject(new Coffre(9 * 16 + 8, 156 * 16, 0, scene->getCoffre(6, 0), TI_CARTE_DONJON));
            addObject(new Coffre(69 * 16 + 8, 111 * 16, 0, scene->getCoffre(6, 1), TI_BOUSSOLE));
            addObject(new Coffre(9 * 16 + 8, 36 * 16, 0, scene->getCoffre(6, 2), TI_CLE_BOSS));

            addObject(new Coffre(69 * 16, 81 * 16 + 8, 1, inventory->hasObject(CAPE_INVISIBILITE), TI_CAPE_INVISIBILITE));

            if (scene->getCoffre(6, 2) == 2) ouvrePorte(49, 28, 0);
            if (inventory->hasCoeur(10)) ouvrePorte(49, 13, 0);

            if (scene->getCoffre(6, 4)) {
                map->setSol(3*16,52*16,2563);
                addObject(new Coffre(3 * 16, 52 * 16, 0, true, TI_CLE, 4));
            }

            if (scene->getCoffre(6, 5)) {
                map->setSol(23*16,157*16,2559);
                addObject(new Coffre(23 * 16, 157 * 16, 0, true, TI_CLE, 5));
            }

            addObject(new Coffre(69 * 16 + 8, 6 * 16, 0, scene->getCoffre(6, 6), TI_CLE, 6));
            addObject(new Coffre(69 * 16 + 8, 21 * 16, 0, scene->getCoffre(6, 7), TI_CLE, 7));
            addObject(new Coffre(109 * 16 + 8, 51 * 16, 0, scene->getCoffre(6, 8), TI_CLE, 8));
            addObject(new Coffre(45 * 16, 36 * 16, 0, scene->getCoffre(6, 28), TI_CLE, 28));

            addObject(new Caisse(28*16, 7*16, 3));
            addObject(new Caisse(35*16, 7 *16, 3));

            addObject(new Interrupteur(18*16, 16*16, 2, scene->getCoffre(6, 14), map));
            addObject(new Interrupteur(21*16, 16*16, 2, scene->getCoffre(6, 14), map));
            addObject(new Interrupteur(28*16, 4*16, 2, scene->getCoffre(6, 14), map));
            addObject(new Interrupteur(35*16, 4*16, 2, scene->getCoffre(6, 14), map));

            addObject(new Interrupteur(92*16, 202*16, 1, false, map));

            addObject(new Interrupteur(7*16, 32*16, 0, scene->getCoffre(6, 15), map));
            addObject(new Interrupteur(37*16, 40*16, 0, scene->getCoffre(6, 16), map));
            addObject(new Interrupteur(56*16, 142*16, 0, scene->getCoffre(6, 17), map));
            addObject(new Interrupteur(56*16, 157*16, 0, scene->getCoffre(6, 18), map));
            addObject(new Interrupteur(42*16, 109*16, 0, scene->getCoffre(6, 19), map));
            addObject(new Interrupteur(52*16, 117*16, 0, scene->getCoffre(6, 20), map));
            addObject(new Interrupteur(82*16, 214*16, 0, scene->getCoffre(6, 21), map));
            addObject(new Interrupteur(86*16, 38*16, 0, scene->getCoffre(6, 22), map));
            addObject(new Interrupteur(112*16, 12*16, 0, scene->getCoffre(6, 23), map));

            if (scene->getCoffre(6, 9)) ouvrePorte(29, 118, 0);
            if (scene->getCoffre(6, 10)) ouvrePorte(29, 103, 0);
            if (scene->getCoffre(6, 11)) ouvrePorte(49, 103, 0);
            if (scene->getCoffre(6, 12)) ouvrePorte(49, 118, 0);
            if (scene->getCoffre(6, 13)) ouvrePorte(69, 103, 0);

            if (scene->getCoffre(6, 14)) ouvrePorte(9, 28, 0);
            if (scene->getCoffre(6, 15)) ouvrePorte(18, 36, 1);
            if (scene->getCoffre(6, 16)) ouvrePorte(38, 36, 1);
            if (scene->getCoffre(6, 17)) ouvrePorte(38, 141, 1);
            if (scene->getCoffre(6, 18)) ouvrePorte(49, 163, 0);
            if (scene->getCoffre(6, 19)) ouvrePorte(38, 111, 1);
            if (scene->getCoffre(6, 20)) ouvrePorte(58, 111, 1);
            if (scene->getCoffre(6, 21)) ouvrePorte(78, 216, 1);
            if (scene->getCoffre(6, 22)) ouvrePorte(89, 43, 0);
            if (scene->getCoffre(6, 23)) ouvrePorte(109, 13, 0);

            if (scene->getCoffre(6, 24)) ouvrePorte(9, 193, 3);
            if (scene->getCoffre(6, 25)) ouvrePorte(9, 208, 3);
            if (scene->getCoffre(6, 26)) ouvrePorte(9, 81, 2);

            if (scene->getCoffre(6, 27)) ouvrePorte(69, 88, 0);
            if (scene->getCoffre(6, 29)) ouvrePorte(29, 133, 0);

            addObject(new Oeil(7*16 + 4, 92*16 + 4, W, map));
            addObject(new Oeil(66*16 + 4, 47*16 + 4, W, map));

            addObject(new PiegeLong(64*16, 63*16, S));
            addObject(new PiegeLong(76*16, 63*16, S));
            addObject(new PiegeLong(88*16, 63*16, S));

            addObject(new PiegeMain(9*16, 51*16));
            addObject(new PiegeMain(89*16, 6*16));
            addObject(new PiegeMain(69*16, 36*16));
            addObject(new PiegeMain(109*16, 51*16));
            addObject(new PiegeMain(29*16, 36*16));

            addEnnemi(new Ennemi026(6*16,200*16));
            addEnnemi(new Ennemi026(13*16,203*16));
            addEnnemi(new Ennemi026(54*16,202*16));
            addEnnemi(new Ennemi026(34*16,205*16));
            addEnnemi(new Ennemi026(21*16,202*16));
            addEnnemi(new Ennemi026(68*16,196*16));
            addEnnemi(new Ennemi026(85*16,181*16));
            addEnnemi(new Ennemi026(56*16,185*16));
            addEnnemi(new Ennemi026(59*16,174*16));
            addEnnemi(new Ennemi026(70*16,182*16));
            addEnnemi(new Ennemi026(102*16,178*16));
            addEnnemi(new Ennemi026(92*16,161*16));
            addEnnemi(new Ennemi026(102*16,162*16));
            addEnnemi(new Ennemi026(46*16,161*16));
            addEnnemi(new Ennemi026(48*16,153*16));
            addEnnemi(new Ennemi026(53*16,157*16));
            addEnnemi(new Ennemi026(5*16,171*16));
            addEnnemi(new Ennemi026(11*16,176*16));
            addEnnemi(new Ennemi026(14*16,168*16));
            addEnnemi(new Ennemi026(6*16,96*16));
            addEnnemi(new Ennemi026(13*16,98*16));
            addEnnemi(new Ennemi026(26*16,123*16));
            addEnnemi(new Ennemi026(33*16,128*16));
            addEnnemi(new Ennemi026(46*16,138*16));
            addEnnemi(new Ennemi026(66*16,127*16));
            addEnnemi(new Ennemi026(72*16,123*16));
            addEnnemi(new Ennemi026(46*16,95*16));
            addEnnemi(new Ennemi026(53*16,98*16));
            addEnnemi(new Ennemi026(92*16,78*16));
            addEnnemi(new Ennemi026(46*16,67*16));
            addEnnemi(new Ennemi026(74*16,48*16));
            addEnnemi(new Ennemi026(21*16,60*16));

            addEnnemi(new Ennemi056(29*16+8,37*16));
            addEnnemi(new Ennemi056(65*16,22*16));
            addEnnemi(new Ennemi056(71*16,18*16));
            addEnnemi(new Ennemi056(74*16,23*16));
            addEnnemi(new Ennemi056(65*16,6*16));
            addEnnemi(new Ennemi056(69*16,10*16));
            addEnnemi(new Ennemi056(72*16,3*16));
            addEnnemi(new Ennemi056(92*16,26*16));
            addEnnemi(new Ennemi056(99*16,35*16));
            addEnnemi(new Ennemi056(94*16,99*16));
            addEnnemi(new Ennemi056(104*16,109*16));
            addEnnemi(new Ennemi056(113*16,98*16));

            addEnnemi(new Ennemi062(69*16+4,36*16-5));
            addEnnemi(new Ennemi062(89*16+4,6*16-5));
            addEnnemi(new Ennemi062(9*16+4,16*16-5));
            addEnnemi(new Ennemi062(29*16+4,16*16-5));
            addEnnemi(new Ennemi062(9*16+4,126*16-5));
            addEnnemi(new Ennemi062(109*16+4,126*16-5));
            addEnnemi(new Ennemi062(69*16+4,156*16-5));

            addEnnemi(new Ennemi150(69*16+8,217*16));
            addEnnemi(new Ennemi150(100*16,196*16));
            addEnnemi(new Ennemi150(67*16,153*16));
            addEnnemi(new Ennemi150(72*16,159*16));
            addEnnemi(new Ennemi150(26*16,155*16));
            addEnnemi(new Ennemi150(28*16,160*16));
            addEnnemi(new Ennemi150(31*16,157*16));
            addEnnemi(new Ennemi150(5*16,158*16));
            addEnnemi(new Ennemi150(13*16,157*16));
            addEnnemi(new Ennemi150(4*16,128*16));
            addEnnemi(new Ennemi150(12*16,123*16));
            addEnnemi(new Ennemi150(26*16,141*16));
            addEnnemi(new Ennemi150(31*16,145*16));
            addEnnemi(new Ennemi150(105*16,128*16));
            addEnnemi(new Ennemi150(113*16,127*16));
            addEnnemi(new Ennemi150(72*16,114*16));
            addEnnemi(new Ennemi150(34*16,80*16));
            addEnnemi(new Ennemi150(39*16,82*16));
            addEnnemi(new Ennemi150(44*16,80*16));
            addEnnemi(new Ennemi150(6*16,49*16));
            addEnnemi(new Ennemi150(8*16,54*16));
            addEnnemi(new Ennemi150(11*16,51*16));
            addEnnemi(new Ennemi150(105*16,51*16));
            addEnnemi(new Ennemi150(109*16,54*16));
            addEnnemi(new Ennemi150(113*16,52*16));
            addEnnemi(new Ennemi150(66*16,38*16));
            addEnnemi(new Ennemi150(73*16,37*16));
            addEnnemi(new Ennemi150(90*16,33*16));
            addEnnemi(new Ennemi150(99*16,27*16));
            addEnnemi(new Ennemi150(109*16,29*16));
            addEnnemi(new Ennemi150(88*16,10*16));
            addEnnemi(new Ennemi150(16*16,10*16));
            addEnnemi(new Ennemi150(22*16,22*16));
            addEnnemi(new Ennemi150(69*16+8,142*16));


            addObject(new Jarre(22*16, 55*16, 1, TI_RUBIS_VERT, map));
            addObject(new Jarre(23*16, 55*16, 1, TI_FLECHE, map));
            addObject(new Jarre(26*16, 56*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(33*16, 63*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(115*16, 38*16, 1, TI_RUBIS_BLEU, map));
            addObject(new Jarre(115*16, 39*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(115*16, 40*16, 1, TI_BOMBE, map));
            addObject(new Jarre(35*16, 86*16, 1, TI_FLECHE, map));
            addObject(new Jarre(35*16, 87*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(44*16, 86*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(44*16, 87*16, 1, TI_RUBIS_BLEU, map));
            addObject(new Jarre(46*16, 171*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(73*16, 171*16, 1, TI_RUBIS_BLEU, map));
            addObject(new Jarre(46*16, 188*16, 1, TI_MAGIE_PEU, map));
            addObject(new Jarre(73*16, 188*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(112*16, 173*16, 1, TI_RUBIS_BLEU, map));
            addObject(new Jarre(112*16, 174*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(112*16, 175*16, 1, TI_BOMBE, map));
            addObject(new Jarre(82*16, 190*16, 1, TI_FLECHE, map));
            addObject(new Jarre(83*16, 190*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(116*16, 197*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(117*16, 197*16, 1, TI_RUBIS_BLEU, map));
            addObject(new Jarre(67*16, 199*16, 1, TI_RUBIS_BLEU, map));
            addObject(new Jarre(72*16, 199*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(94*16, 214*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(94*16, 220*16, 1, TI_MAGIE_PEU, map));
            addObject(new Jarre(36*16, 219*16, 1, TI_NO_ITEM, map));
            addObject(new Jarre(36*16, 220*16, 1, TI_RUBIS_BLEU, map));
            break;
        case 64 :
            if (!scene->getCoffre(7, 49)) {
                addEnnemi(new Ennemi110(69*16, 94*16));
            }

            if (!inventory->hasCoeur(11)) {
                addEnnemi(new Ennemi111(68*16,18*16));
            }

            if (!inventory->hasMedaillons(6)) {
                addItem(ItemHelper::getInstance()->createItem(TI_MEDAILLON_7, 70*16, 6*16));
            }

            addObject(new Coffre(89 * 16 + 8, 141 * 16, 0, scene->getCoffre(7, 0), TI_CARTE_DONJON));
            addObject(new Coffre(9 * 16 + 8, 36 * 16, 0, scene->getCoffre(7, 1), TI_BOUSSOLE));
            addObject(new Coffre(149 * 16 + 8, 66 * 16, 0, scene->getCoffre(7, 2), TI_CLE_BOSS));

            addObject(new Coffre(69 * 16, 81 * 16 + 8, 1, inventory->hasObject(BAGUETTE_GLACE), TI_BAGUETTE_GLACE));

            if (scene->getCoffre(7, 2) == 2) ouvrePorte(69, 28, 0);
            if (inventory->hasCoeur(11)) ouvrePorte(69, 13, 0);

            addObject(new Coffre(9 * 16 + 8, 96 * 16, 0, scene->getCoffre(7, 4), TI_CLE, 4));
            addObject(new Coffre(29 * 16 + 8, 64 * 16, 0, scene->getCoffre(7, 5), TI_CLE, 5));

            if (scene->getCoffre(7, 6)) {
                map->setSol(56*16,7*16,2778);
                addObject(new Coffre(56 * 16, 7 * 16, 0, true, TI_CLE, 6));
            }

            if (scene->getCoffre(7, 7)) {
                map->setSol(56*16,82*16,2777);
                addObject(new Coffre(56 * 16, 82 * 16, 0, true, TI_CLE, 7));
            }

            if (scene->getCoffre(7, 8)) {
                map->setSol(103*16,7*16,2778);
                addObject(new Coffre(103 * 16, 7 * 16, 0, true, TI_CLE, 8));
            }

            addObject(new Coffre(109 * 16 + 8, 36 * 16, 0, scene->getCoffre(7, 9), TI_CLE, 9));
            addObject(new Coffre(129 * 16 + 8, 66 * 16, 0, scene->getCoffre(7, 10), TI_CLE, 10));

            if (scene->getCoffre(7, 11)) {
                map->setSol(143*16,7*16,2776);
                addObject(new Coffre(143 * 16, 7 * 16, 0, true, TI_CLE, 11));
            }

            addObject(new Coffre(209 * 16 + 8, 21 * 16, 0, scene->getCoffre(7, 12), TI_CLE, 12));
            addObject(new Coffre(269 * 16 + 8, 36 * 16, 0, scene->getCoffre(7, 13), TI_CLE, 13));
            addObject(new Coffre(269 * 16 + 8, 51 * 16, 0, scene->getCoffre(7, 14), TI_CLE, 14));

            addObject(new Interrupteur(16*16, 37*16, 0, scene->getCoffre(7, 26), map));
            addObject(new Interrupteur(16*16, 82*16, 0, scene->getCoffre(7, 27), map));
            addObject(new Interrupteur(27*16, 32*16, 0, scene->getCoffre(7, 28), map));
            addObject(new Interrupteur(23*16, 52*16, 0, scene->getCoffre(7, 29), map));
            addObject(new Interrupteur(47*16, 87*16, 0, scene->getCoffre(7, 30), map));
            addObject(new Interrupteur(47*16, 102*16, 0, scene->getCoffre(7, 31), map));
            addObject(new Interrupteur(43*16, 142*16, 0, scene->getCoffre(7, 32), map));
            addObject(new Interrupteur(77*16, 85*16, 0, scene->getCoffre(7, 33), map));
            addObject(new Interrupteur(87*16, 17*16, 0, scene->getCoffre(7, 34), map));
            addObject(new Interrupteur(87*16, 87*16, 0, scene->getCoffre(7, 35), map));
            addObject(new Interrupteur(102*16, 40*16, 0, scene->getCoffre(7, 36), map));
            addObject(new Interrupteur(102*16, 79*16, 0, scene->getCoffre(7, 37), map));
            addObject(new Interrupteur(127*16, 47*16, 0, scene->getCoffre(7, 38), map));
            addObject(new Interrupteur(127*16, 117*16, 0, scene->getCoffre(7, 39), map));
            addObject(new Interrupteur(147*16, 32*16, 0, scene->getCoffre(7, 40), map));
            addObject(new Interrupteur(147*16, 62*16, 0, scene->getCoffre(7, 41), map));
            addObject(new Interrupteur(183*16, 7*16, 0, scene->getCoffre(7, 42), map));
            addObject(new Interrupteur(197*16, 25*16, 0, scene->getCoffre(7, 43), map));
            addObject(new Interrupteur(272*16, 62*16, 0, scene->getCoffre(7, 44), map));

            if (scene->getCoffre(7, 15)) ouvrePorte(18, 51, 1);
            if (scene->getCoffre(7, 16)) ouvrePorte(38, 36, 1);
            if (scene->getCoffre(7, 17)) ouvrePorte(49, 28, 0);
            if (scene->getCoffre(7, 18)) ouvrePorte(69, 103, 0);
            if (scene->getCoffre(7, 19)) ouvrePorte(69, 133, 0);
            if (scene->getCoffre(7, 20)) ouvrePorte(98, 66, 1);
            if (scene->getCoffre(7, 21)) ouvrePorte(158, 141, 1);
            if (scene->getCoffre(7, 22)) ouvrePorte(198, 36, 1);
            if (scene->getCoffre(7, 23)) ouvrePorte(218, 36, 1);
            if (scene->getCoffre(7, 24)) ouvrePorte(229, 28, 0);
            if (scene->getCoffre(7, 25)) ouvrePorte(258, 66, 1);

            if (scene->getCoffre(7, 26)) ouvrePorte(9, 28, 0);
            if (scene->getCoffre(7, 27)) ouvrePorte(9, 88, 0);
            if (scene->getCoffre(7, 28)) ouvrePorte(18, 36, 1);
            if (scene->getCoffre(7, 29)) ouvrePorte(38, 51, 1);
            if (scene->getCoffre(7, 30)) ouvrePorte(49, 88, 0);
            if (scene->getCoffre(7, 31)) ouvrePorte(49, 103, 0);
            if (scene->getCoffre(7, 32)) ouvrePorte(49, 133, 0);
            if (scene->getCoffre(7, 33)) ouvrePorte(78, 81, 1);
            if (scene->getCoffre(7, 34)) ouvrePorte(89, 28, 0);
            if (scene->getCoffre(7, 35)) ouvrePorte(89, 88, 0);
            if (scene->getCoffre(7, 36)) ouvrePorte(98, 36, 1);
            if (scene->getCoffre(7, 37)) ouvrePorte(118, 81, 1);
            if (scene->getCoffre(7, 38)) ouvrePorte(118, 51, 1);
            if (scene->getCoffre(7, 39)) ouvrePorte(118, 111, 1);
            if (scene->getCoffre(7, 40)) ouvrePorte(149, 43, 0);
            if (scene->getCoffre(7, 41)) ouvrePorte(149, 58, 0);
            if (scene->getCoffre(7, 42)) ouvrePorte(189, 13, 0);
            if (scene->getCoffre(7, 43)) ouvrePorte(198, 21, 1);
            if (scene->getCoffre(7, 44)) ouvrePorte(269, 58, 0);

            if (scene->getCoffre(7, 45)) ouvrePorte(9, 43, 2);
            if (scene->getCoffre(7, 46)) ouvrePorte(29, 141, 4);
            if (scene->getCoffre(7, 47)) ouvrePorte(98, 6, 3);
            if (scene->getCoffre(7, 48)) ouvrePorte(269, 43, 2);

            addObject(new PiegeLong(207*16+8, 77*16, S));

            addEnnemi(new Ennemi026(269*16,124*16));
            addEnnemi(new Ennemi026(272*16,111*16));
            addEnnemi(new Ennemi026(249*16,94*16));
            addEnnemi(new Ennemi026(250*16,116*16));
            addEnnemi(new Ennemi026(164*16,103*16));
            addEnnemi(new Ennemi026(175*16,108*16));
            addEnnemi(new Ennemi026(165*16,11*16));
            addEnnemi(new Ennemi026(169*16,3*16));
            addEnnemi(new Ennemi026(176*16,11*16));
            addEnnemi(new Ennemi026(192*16,3*16));
            addEnnemi(new Ennemi026(274*16,1*16));
            addEnnemi(new Ennemi026(264*16,28*16));
            addEnnemi(new Ennemi026(241*16,13*16));
            addEnnemi(new Ennemi026(146*16,21*16));
            addEnnemi(new Ennemi026(151*16,18*16));
            addEnnemi(new Ennemi026(153*16,26*16));
            addEnnemi(new Ennemi026(53*16,138*16));
            addEnnemi(new Ennemi026(13*16,108*16));
            addEnnemi(new Ennemi026(106*16,95*16));
            addEnnemi(new Ennemi026(113*16,99*16));
            addEnnemi(new Ennemi026(28*16,78*16));
            addEnnemi(new Ennemi026(106*16,48*16));
            addEnnemi(new Ennemi026(111*16,56*16));
            addEnnemi(new Ennemi026(26*16,26*16));
            addEnnemi(new Ennemi026(30*16,18*16));
            addEnnemi(new Ennemi026(48*16,18*16));
            addEnnemi(new Ennemi026(27*16,11*16));
            addEnnemi(new Ennemi026(32*16,3*16));
            addEnnemi(new Ennemi026(12*16,56*16));
            addEnnemi(new Ennemi026(90*16,3*16));

            addEnnemi(new Ennemi041(53*16,69*16));
            addEnnemi(new Ennemi041(59*16,67*16));
            addEnnemi(new Ennemi041(65*16,68*16));
            addEnnemi(new Ennemi041(55*16,48*16));
            addEnnemi(new Ennemi041(58*16,40*16));
            addEnnemi(new Ennemi041(69*16+8,43*16));
            addEnnemi(new Ennemi041(78*16,40*16));
            addEnnemi(new Ennemi041(86*16,45*16));
            addEnnemi(new Ennemi041(29*16+8,89*16));
            addEnnemi(new Ennemi041(108*16,117*16));
            addEnnemi(new Ennemi041(111*16,121*16));
            addEnnemi(new Ennemi041(60*16,115*16));
            addEnnemi(new Ennemi041(63*16,126*16));
            addEnnemi(new Ennemi041(78*16,114*16));
            addEnnemi(new Ennemi041(89*16,120*16));
            addEnnemi(new Ennemi041(27*16,140*16));
            addEnnemi(new Ennemi041(32*16,143*16));
            addEnnemi(new Ennemi041(149*16+8,111*16));
            addEnnemi(new Ennemi041(189*16+8,96*16));
            addEnnemi(new Ennemi041(227*16,81*16));
            addEnnemi(new Ennemi041(269*16+8,141*16));
            addEnnemi(new Ennemi041(6*16,23*16));
            addEnnemi(new Ennemi041(9*16+8,19*16));
            addEnnemi(new Ennemi041(12*16,22*16));
            addEnnemi(new Ennemi041(11*16,38*16));
            addEnnemi(new Ennemi041(89*16+8,21*16));
            addEnnemi(new Ennemi041(109*16+8,22*16));
            addEnnemi(new Ennemi041(52*16,3*16));
            addEnnemi(new Ennemi041(53*16,10*16));
            addEnnemi(new Ennemi041(129*16+8,51*16));
            addEnnemi(new Ennemi041(134*16,36*16));
            addEnnemi(new Ennemi041(106*16,4*16));
            addEnnemi(new Ennemi041(108*16,9*16));
            addEnnemi(new Ennemi041(111*16,5*16));
            addEnnemi(new Ennemi041(89*16+8,81*16));
            addEnnemi(new Ennemi041(129*16+8,111*16));
            addEnnemi(new Ennemi041(146*16,4*16));
            addEnnemi(new Ennemi041(148*16,10*16));
            addEnnemi(new Ennemi041(150*16,6*16));
            addEnnemi(new Ennemi041(189*16+8,36*16));
            addEnnemi(new Ennemi041(229*16+8,36*16));
            addEnnemi(new Ennemi041(269*16+8,53*16));
            addEnnemi(new Ennemi041(252*16+8,14*16));
            addEnnemi(new Ennemi041(261*16+8,22*16));
            addEnnemi(new Ennemi041(272*16+8,11*16));

            addEnnemi(new Ennemi042(252*16+3,39*16-3));
            addEnnemi(new Ennemi042(231*16+3,9*16-3));
            addEnnemi(new Ennemi042(109*16+3,82*16-3));
            addEnnemi(new Ennemi042(67*16+3,145*16-3));
            addEnnemi(new Ennemi042(70*16+3,141*16-3));
            addEnnemi(new Ennemi042(74*16+3,144*16-3));
            addEnnemi(new Ennemi042(86*16+3,144*16-3));
            addEnnemi(new Ennemi042(93*16+3,141*16-3));
            addEnnemi(new Ennemi042(90*16+3,126*16-3));
            addEnnemi(new Ennemi042(133*16+3,129*16-3));
            addEnnemi(new Ennemi042(27*16+3,115*16-3));
            addEnnemi(new Ennemi042(49*16+3,82*16-3));
            addEnnemi(new Ennemi042(52*16+3,80*16-3));
            addEnnemi(new Ennemi042(53*16+3,84*16-3));
            addEnnemi(new Ennemi042(48*16+3,50*16-3));
            addEnnemi(new Ennemi042(29*16+3,37*16-3));
            addEnnemi(new Ennemi042(125*16+3,38*16-3));
            addEnnemi(new Ennemi042(149*16+3,36*16-3));
            addEnnemi(new Ennemi042(209*16+3,7*16-3));
            addEnnemi(new Ennemi042(265*16+3,37*16-3));
            addEnnemi(new Ennemi042(273*16+3,38*16-3));
            addEnnemi(new Ennemi042(229*16+3,97*16-3));
            addEnnemi(new Ennemi042(209*16+3,36*16-3));
            addEnnemi(new Ennemi042(249*16+3,23*16-3));

            addEnnemi(new Ennemi062(129*16+4,126*16-5));
            addEnnemi(new Ennemi062(29*16+4,111*16-5));
            addEnnemi(new Ennemi062(129*16+4,36*16-5));
            addEnnemi(new Ennemi062(249*16+4,36*16-5));
            addEnnemi(new Ennemi062(149*16+4,126*16-5));
            addEnnemi(new Ennemi062(189*16+4,81*16-5));
            addEnnemi(new Ennemi062(169*16+4,28*16-5));
            addEnnemi(new Ennemi062(229*16+4,6*16-5));
            addEnnemi(new Ennemi062(49*16+4,6*16-5));

            addObject(new Jarre(67*16, 32*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(72*16, 32*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(85*16, 19*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(85*16, 25*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(96*16, 67*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(134*16, 49*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(134*16, 55*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(24*16, 98*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(134*16, 109*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(134*16, 115*16, 0, TI_RUBIS_BLEU, map));
            addObject(new Jarre(169*16, 18*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(170*16, 18*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(244*16, 27*16, 0, TI_RUBIS_VERT, map));
            addObject(new Jarre(255*16, 27*16, 0, TI_NO_ITEM, map));
            addObject(new Jarre(274*16, 64*16, 0, TI_RUBIS_ROUGE, map));
            addObject(new Jarre(274*16, 70*16, 0, TI_RUBIS_ROUGE, map));
            break;
        case 65 :

            if (!inventory->hasCoeur(12)) {
                addEnnemi(new Ennemi076(48*16+2,18*16+3));
            }

            addObject(new Coffre(129 * 16 + 8, 36 * 16, 0, scene->getCoffre(8, 0), TI_CARTE_DONJON));
            addObject(new Coffre(149 * 16 + 8, 21 * 16, 0, scene->getCoffre(8, 1), TI_BOUSSOLE));
            addObject(new Coffre(169 * 16 + 8, 51 * 16, 0, scene->getCoffre(8, 2), TI_CLE_BOSS));

            if (scene->getCoffre(8, 2) == 2) ouvrePorte(49, 28, 0);
            if (inventory->hasCoeur(12)) ouvrePorte(49, 13, 0);

            if (scene->getCoffre(8, 4)) {
                map->setSol(3*16,7*16,2997);
                addObject(new Coffre(3 * 16, 7 * 16, 0, true, TI_CLE, 4));
            }

            addObject(new Coffre(9 * 16 + 8, 21 * 16, 0, scene->getCoffre(8, 5), TI_CLE, 5));

            if (scene->getCoffre(8, 6)) {
                map->setSol(3*16,67*16,2997);
                addObject(new Coffre(3 * 16, 67 * 16, 0, true, TI_CLE, 6));
            }

            addObject(new Coffre(49 * 16 + 8, 36 * 16, 0, scene->getCoffre(8, 7), TI_CLE, 7));
            addObject(new Coffre(69 * 16 + 8, 6 * 16, 0, scene->getCoffre(8, 8), TI_CLE, 8));
            addObject(new Coffre(89 * 16 + 8, 6 * 16, 0, scene->getCoffre(8, 9), TI_CLE, 9));
            addObject(new Coffre(89 * 16 + 8, 53 * 16, 0, scene->getCoffre(8, 10), TI_CLE, 10));

            addObject(new Interrupteur(22*16, 25*16, 0, scene->getCoffre(8, 18), map));
            addObject(new Interrupteur(26*16, 48*16, 0, scene->getCoffre(8, 19), map));
            addObject(new Interrupteur(64*16, 36*16, 0, scene->getCoffre(8, 20), map));
            addObject(new Interrupteur(87*16, 47*16, 0, scene->getCoffre(8, 21), map));
            addObject(new Interrupteur(162*16, 55*16, 0, scene->getCoffre(8, 22), map));

            if (scene->getCoffre(8, 11)) ouvrePorte(138, 51, 1);
            if (scene->getCoffre(8, 12)) ouvrePorte(129, 43, 0);
            if (scene->getCoffre(8, 13)) ouvrePorte(129, 28, 0);
            if (scene->getCoffre(8, 14)) ouvrePorte(138, 21, 1);
            if (scene->getCoffre(8, 15)) ouvrePorte(158, 21, 1);
            if (scene->getCoffre(8, 16)) ouvrePorte(169, 28, 0);
            if (scene->getCoffre(8, 17)) ouvrePorte(169, 43, 0);

            if (scene->getCoffre(8, 18)) ouvrePorte(29, 28, 0);
            if (scene->getCoffre(8, 19)) ouvrePorte(18, 51, 1);
            if (scene->getCoffre(8, 20)) ouvrePorte(69, 28, 0);
            if (scene->getCoffre(8, 21)) ouvrePorte(89, 58, 0);
            if (scene->getCoffre(8, 22)) ouvrePorte(158, 51, 1);

            if (scene->getCoffre(8, 23)) ouvrePorte(18, 21, 3);
            if (scene->getCoffre(8, 24)) ouvrePorte(89, 43, 2);

            addObject(new PiegeLong(26*16, 9*16, S));

            addEnnemi(new Ennemi044(49*16+5,66*16));
            addEnnemi(new Ennemi044(79*16+5,68*16));
            addEnnemi(new Ennemi044(89*16+5,36*16));
            addEnnemi(new Ennemi044(86*16+5,21*16));
            addEnnemi(new Ennemi044(92*16+5,21*16));
            addEnnemi(new Ennemi044(69*16+5,21*16));
            addEnnemi(new Ennemi044(42*16+5,44*16));
            addEnnemi(new Ennemi044(60*16+5,38*16));
            addEnnemi(new Ennemi044(7*16+5,7*16));
            addEnnemi(new Ennemi044(8*16+5,10*16));
            addEnnemi(new Ennemi044(9*16+5,4*16));
            addEnnemi(new Ennemi044(7*16+5,36*16));
            addEnnemi(new Ennemi044(11*16+5,36*16));
            addEnnemi(new Ennemi044(9*16+5,51*16));
            addEnnemi(new Ennemi044(29*16+5,66*16));
            addEnnemi(new Ennemi044(8*16+5,64*16));
            addEnnemi(new Ennemi044(8*16+5,69*16));
            break;
        case 66 :

            addEnnemi(new Ennemi153(187*16+1+8,17*16-6));

            addObject(new Coffre(69 * 16 + 8, 51 * 16, 0, scene->getCoffre(9, 0), TI_CARTE_DONJON));
            addObject(new Coffre(89 * 16 + 8, 51 * 16, 0, scene->getCoffre(9, 1), TI_BOUSSOLE));
            addObject(new Coffre(169 * 16 + 8, 51 * 16, 0, scene->getCoffre(9, 2), TI_CLE_BOSS));

            if (scene->getCoffre(9, 2) == 2) ouvrePorte(189, 28, 0);


            addObject(new Coffre(9 * 16 + 8, 51 * 16, 0, scene->getCoffre(9, 4), TI_CLE, 4));
            addObject(new Coffre(89 * 16 + 8, 21 * 16, 0, scene->getCoffre(9, 5), TI_CLE, 5));
            addObject(new Coffre(229 * 16 + 8, 6 * 16, 0, scene->getCoffre(9, 6), TI_CLE, 6));
            addObject(new Coffre(229 * 16 + 8, 51 * 16, 0, scene->getCoffre(9, 7), TI_CLE, 7));

            addObject(new Interrupteur(9*16, 18*16, 0, scene->getCoffre(9, 12), map));
            addObject(new Interrupteur(17*16, 55*16, 0, scene->getCoffre(9, 13), map));
            addObject(new Interrupteur(36*16, 7*16, 0, scene->getCoffre(9, 14), map));
            addObject(new Interrupteur(42*16, 55*16, 0, scene->getCoffre(9, 15), map));
            addObject(new Interrupteur(66*16, 6*16, 0, scene->getCoffre(9, 16), map));
            addObject(new Interrupteur(89*16, 33*16, 0, scene->getCoffre(9, 17), map));
            addObject(new Interrupteur(109*16, 11*16, 0, scene->getCoffre(9, 18), map));
            addObject(new Interrupteur(117*16, 55*16, 0, scene->getCoffre(9, 19), map));
            addObject(new Interrupteur(130*16, 11*16, 0, scene->getCoffre(9, 20), map));
            addObject(new Interrupteur(132*16, 21*16, 0, scene->getCoffre(9, 21), map));
            addObject(new Interrupteur(132*16, 38*16, 0, scene->getCoffre(9, 22), map));
            addObject(new Interrupteur(146*16, 38*16, 0, scene->getCoffre(9, 23), map));
            addObject(new Interrupteur(177*16, 55*16, 0, scene->getCoffre(9, 24), map));

            if (scene->getCoffre(9, 8)) ouvrePorte(69, 43, 0);
            if (scene->getCoffre(9, 9)) ouvrePorte(89, 43, 0);
            if (scene->getCoffre(9, 10)) ouvrePorte(169, 13, 0);
            if (scene->getCoffre(9, 11)) ouvrePorte(169, 43, 0);

            if (scene->getCoffre(9, 12)) ouvrePorte(18, 21, 1);
            if (scene->getCoffre(9, 13)) ouvrePorte(18, 51, 1);
            if (scene->getCoffre(9, 14)) ouvrePorte(29, 13, 0);
            if (scene->getCoffre(9, 15)) ouvrePorte(38, 51, 1);
            if (scene->getCoffre(9, 16)) ouvrePorte(58, 6, 1);
            if (scene->getCoffre(9, 17)) ouvrePorte(98, 36, 1);
            if (scene->getCoffre(9, 18)) ouvrePorte(98, 6, 1);
            if (scene->getCoffre(9, 19)) ouvrePorte(118, 51, 1);
            if (scene->getCoffre(9, 20)) ouvrePorte(138, 6, 1);
            if (scene->getCoffre(9, 21)) ouvrePorte(129, 13, 0);
            if (scene->getCoffre(9, 22)) ouvrePorte(129, 43, 0);
            if (scene->getCoffre(9, 23)) ouvrePorte(138, 36, 1);
            if (scene->getCoffre(9, 24)) ouvrePorte(178, 51, 1);

            if (scene->getCoffre(9, 25)) ouvrePorte(89, 28, 2);
            if (scene->getCoffre(9, 26)) ouvrePorte(109, 13, 2);
            if (scene->getCoffre(9, 27)) ouvrePorte(229, 43, 2);

            addEnnemi(new Ennemi046(25*16+5,49*16+5));
            addEnnemi(new Ennemi046(33*16+5,49*16+5));
            addEnnemi(new Ennemi046(30*16+5+8,28*16+5));
            addEnnemi(new Ennemi046(38*16+5,17*16+5));
            addEnnemi(new Ennemi046(51*16+5,36*16+5));
            addEnnemi(new Ennemi046(69*16+5,36*16+5));
            addEnnemi(new Ennemi046(69*16+5,7*16+5));
            addEnnemi(new Ennemi046(69*16+5,20*16+5));
            addEnnemi(new Ennemi046(27*16+5,6*16+5));
            addEnnemi(new Ennemi046(4*16+5,35*16+5));
            addEnnemi(new Ennemi046(14*16+5,35*16+5));
            addEnnemi(new Ennemi046(13*16+5,24*16+5));
            addEnnemi(new Ennemi046(72*16+5,53*16+5));

            addEnnemi(new Ennemi047(49*16+5,3*16+5));
            addEnnemi(new Ennemi047(49*16+5,10*16+5));
            addEnnemi(new Ennemi047(6*16+5,3*16+5));
            addEnnemi(new Ennemi047(6*16+5,9*16+5));
            addEnnemi(new Ennemi047(9*16+5,38*16+5));
            addEnnemi(new Ennemi047(150*16+5,44*16+5));
            addEnnemi(new Ennemi047(130*16+5,29*16+5));
            addEnnemi(new Ennemi047(117*16+5,36*16+5));
            addEnnemi(new Ennemi047(105*16+5,29*16+5));
            addEnnemi(new Ennemi047(153*16+5,24*16+5));
            addEnnemi(new Ennemi047(109*16+5,4*16+5));
            addEnnemi(new Ennemi047(129*16+5,4*16+5));
            addEnnemi(new Ennemi047(89*16+5,6*16+5));
            addEnnemi(new Ennemi047(149*16+5,6*16+5));
            addEnnemi(new Ennemi047(85*16+5,23*16+5));
            addEnnemi(new Ennemi047(93*16+5,39*16+5));

            addEnnemi(new Ennemi048(109*16+5,22*16+5));
            addEnnemi(new Ennemi048(149*16+5,21*16+5));
            addEnnemi(new Ennemi048(86*16+5,53*16+5));

            addEnnemi(new Ennemi151(208*16,52*16));
            addEnnemi(new Ennemi151(212*16,54*16));
            addEnnemi(new Ennemi151(213*16,50*16));
            addEnnemi(new Ennemi151(206*16,37*16));
            addEnnemi(new Ennemi151(209*16,21*16));
            addEnnemi(new Ennemi151(212*16,38*16));
            addEnnemi(new Ennemi151(226*16,20*16));
            addEnnemi(new Ennemi151(230*16,38*16));
            addEnnemi(new Ennemi151(234*16,22*16));
            addEnnemi(new Ennemi151(232*16,7*16));
            addEnnemi(new Ennemi151(231*16,52*16));
            addEnnemi(new Ennemi151(208*16,4*16));
            addEnnemi(new Ennemi151(213*16,7*16));
            addEnnemi(new Ennemi151(190*16,11*16));
            addEnnemi(new Ennemi151(191*16,3*16));
            addEnnemi(new Ennemi151(164*16,7*16));
            addEnnemi(new Ennemi151(172*16,9*16));
            addEnnemi(new Ennemi151(166*16,36*16));
            addEnnemi(new Ennemi151(169*16,28*16));
            addEnnemi(new Ennemi151(174*16,20*16));
            addEnnemi(new Ennemi151(174*16,39*16));
            addEnnemi(new Ennemi151(185*16,35*16));
            addEnnemi(new Ennemi151(193*16,37*16));

            addEnnemi(new Ennemi152(189*16-3,6*16-3));
            addEnnemi(new Ennemi152(169*16-3,6*16-3));
            addEnnemi(new Ennemi152(166*16-3,24*16-3));
            addEnnemi(new Ennemi152(172*16-3,32*16-3));
            addEnnemi(new Ennemi152(165*16-3,53*16-3));
            break;
    }
}
