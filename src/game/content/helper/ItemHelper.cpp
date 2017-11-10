#include "ItemHelper.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../items/PetitCoeur.h"
#include "../items/RubisVert.h"
#include "../items/RubisBleu.h"
#include "../items/RubisRouge.h"
#include "../items/Arrow.h"
#include "../items/Bomb.h"
#include "../items/MagiePetit.h"
#include "../items/MagieGrand.h"
#include "../items/QuartCoeur.h"
#include "../items/GemmeForce.h"
#include "../items/Coeur.h"
#include "../items/PetiteCle.h"
#include "../items/Medaillon.h"

ItemHelper ItemHelper::instance=ItemHelper();

ItemHelper::ItemHelper() : image(0), imageObjects(0), imageMedaillons(0) {
}

ItemHelper::~ItemHelper() {
}

void ItemHelper::close() {
    ResourceManager::getInstance()->free(image);
    ResourceManager::getInstance()->free(imageObjects);
    ResourceManager::getInstance()->free(imageMedaillons);
}

ItemHelper* ItemHelper::getInstance() {
    return &instance;
}

Item* ItemHelper::createItem(TypeItem type, int x, int y, int id, bool withAnim) {
    y -= 8;
    switch (type) {
        case TI_BOMBE : return new Bomb(x - 6, y);
        case TI_FLECHE : return new Arrow(x - 2, y);
        case TI_MAGIE_BEAUCOUP : return new MagieGrand(x - 4, y);
        case TI_MAGIE_PEU : return new MagiePetit(x - 4, y);
        case TI_PETIT_COEUR : return new PetitCoeur(x - 4, y);
        case TI_QUART_COEUR : return new QuartCoeur(x - 8, y, id);
        case TI_RUBIS_BLEU : return new RubisBleu(x - 4, y);
        case TI_RUBIS_ROUGE : return new RubisRouge(x - 4, y);
        case TI_RUBIS_VERT : return new RubisVert(x - 4, y);
        case TI_GEMME_FORCE : return new GemmeForce(x - 8, y, id, withAnim);
        case TI_COEUR : return new Coeur(x - 8, y, id, withAnim);
        case TI_CLE : return new PetiteCle(x - 4, y, id);
        case TI_MEDAILLON_1 : return new Medaillon(x - 8, y - 8, 0);
        case TI_MEDAILLON_2 : return new Medaillon(x - 8, y - 8, 1);
        case TI_MEDAILLON_3 : return new Medaillon(x - 8, y - 8, 2);
        case TI_MEDAILLON_4 : return new Medaillon(x - 8, y - 8, 3);
        case TI_MEDAILLON_5 : return new Medaillon(x - 8, y - 8, 4);
        case TI_MEDAILLON_6 : return new Medaillon(x - 8, y - 8, 5);
        case TI_MEDAILLON_7 : return new Medaillon(x - 8, y - 8, 6);
        default : return 0;
    }
}

void ItemHelper::drawItem(TypeItem type, int x, int y) {
    if (image == 0) {
        image = ResourceManager::getInstance()->loadImage("data/images/items/items.png", true);
    }
    if (imageObjects == 0) {
        imageObjects = ResourceManager::getInstance()->loadImage("data/images/items/objects.png", true);
    }
    if (imageMedaillons == 0) {
        imageMedaillons = ResourceManager::getInstance()->loadImage("data/images/status/medaillons.png", true);
    }

    switch (type) {
        case TI_QUART_COEUR : WindowManager::getInstance()->draw(image, 45, 0, 16, 14, x - 8, y - 7); break;
        case TI_FLACON_1 : case TI_FLACON_2 : case TI_FLACON_3 : case TI_FLACON_4 :
            WindowManager::getInstance()->draw(image, 30, 14, 16, 16, x - 8, y - 8); break;
        case TI_GEMME_FORCE : WindowManager::getInstance()->draw(image, 16, 63, 16, 16, x - 8, y - 8); break;
        case TI_COQUILLAGE : WindowManager::getInstance()->draw(image, 30, 30, 16, 16, x - 8, y - 8); break;
        case TI_SAC_RIZ : WindowManager::getInstance()->draw(imageObjects, 0, 0, 16, 16, x - 8, y - 8); break;
        case TI_BOCAL_EPICES : WindowManager::getInstance()->draw(imageObjects, 16, 0, 16, 16, x - 8, y - 8); break;
        case TI_SAC_OLIVES : WindowManager::getInstance()->draw(imageObjects, 32, 0, 16, 16, x - 8, y - 8); break;
        case TI_PERLES : WindowManager::getInstance()->draw(imageObjects, 48, 0, 16, 16, x - 8, y - 8); break;
        case TI_PLUME : WindowManager::getInstance()->draw(imageObjects, 64, 0, 16, 16, x - 8, y - 8); break;
        case TI_POULET : WindowManager::getInstance()->draw(imageObjects, 80, 0, 16, 16, x - 8, y - 8); break;
        case TI_TAPISSERIE : WindowManager::getInstance()->draw(imageObjects, 96, 0, 16, 16, x - 8, y - 8); break;
        case TI_AMULETTE : WindowManager::getInstance()->draw(imageObjects, 112, 0, 16, 16, x - 8, y - 8); break;
        case TI_PARFUM : WindowManager::getInstance()->draw(imageObjects, 128, 0, 16, 16, x - 8, y - 8); break;
        case TI_ROC_GIVRE : WindowManager::getInstance()->draw(imageObjects, 144, 0, 16, 16, x - 8, y - 8); break;
        case TI_BOURSE_1 : WindowManager::getInstance()->draw(image, 0, 79, 16, 16, x - 8, y - 8); break;
        case TI_BOURSE_2 : WindowManager::getInstance()->draw(image, 16, 79, 16, 16, x - 8, y - 8); break;
        case TI_BOURSE_3 : WindowManager::getInstance()->draw(image, 32, 79, 16, 16, x - 8, y - 8); break;
        case TI_BOURSE_4 : WindowManager::getInstance()->draw(image, 48, 79, 16, 16, x - 8, y - 8); break;
        case TI_BOUCLIER_1 : WindowManager::getInstance()->draw(image, 0, 95, 8, 10, x - 4, y - 5); break;
        case TI_BOUCLIER_2 : WindowManager::getInstance()->draw(image, 8, 95, 14, 16, x - 7, y - 8); break;
        case TI_BOUCLIER_3 : WindowManager::getInstance()->draw(image, 22, 95, 14, 16, x - 7, y - 8); break;
        case TI_CHANT_1 : WindowManager::getInstance()->draw(image, 48, 16, 16, 16, x - 8, y - 8); break;
        case TI_CHANT_2 : WindowManager::getInstance()->draw(image, 48, 32, 16, 16, x - 8, y - 8); break;
        case TI_CHANT_3 : WindowManager::getInstance()->draw(image, 48, 48, 16, 16, x - 8, y - 8); break;
        case TI_EPEE_1 : WindowManager::getInstance()->draw(image, 36, 95, 6, 16, x - 3, y - 8); break;
        case TI_EPEE_2 : WindowManager::getInstance()->draw(image, 42, 95, 7, 16, x - 3, y - 8); break;
        case TI_EPEE_3 : WindowManager::getInstance()->draw(image, 49, 95, 7, 16, x - 3, y - 8); break;
        case TI_EPEE_5 : WindowManager::getInstance()->draw(image, 56, 95, 7, 16, x - 3, y - 8); break;
        case TI_RECETTE_POTION_JAUNE : case TI_CARTE_ILE : case TI_CARTE_DONJON : WindowManager::getInstance()->draw(image, 32, 63, 16, 16, x - 8, y - 8); break;
        case TI_GANT : WindowManager::getInstance()->draw(image, 64, 0, 16, 16, x - 8, y - 8); break;
        case TI_GANT2 : WindowManager::getInstance()->draw(image, 64, 16, 16, 16, x - 8, y - 8); break;
        case TI_ARC : WindowManager::getInstance()->draw(image, 64, 32, 16, 16, x - 8, y - 8); break;
        case TI_ARC_FEE : WindowManager::getInstance()->draw(image, 64, 48, 16, 16, x - 8, y - 8); break;
        case TI_BOOMERANG : WindowManager::getInstance()->draw(image, 64, 64, 16, 16, x - 8, y - 8); break;
        case TI_BOOMERANG_FEE : WindowManager::getInstance()->draw(image, 64, 80, 16, 16, x - 8, y - 8); break;
        case TI_COEUR : WindowManager::getInstance()->draw(image, 30, 0, 14, 13, x - 7, y - 7); break;
        case TI_PALMES : WindowManager::getInstance()->draw(image, 30, 46, 16, 17, x - 8, y - 9); break;
        case TI_OCARINA : WindowManager::getInstance()->draw(image, 49, 65, 14, 14, x - 7, y - 7); break;
        case TI_MARTEAU : WindowManager::getInstance()->draw(image, 64, 96, 16, 16, x - 8, y - 8); break;
        case TI_LANTERNE : WindowManager::getInstance()->draw(image, 80, 0, 16, 16, x - 8, y - 8); break;
        case TI_PERLE_LUNE : WindowManager::getInstance()->draw(image, 80, 16, 16, 16, x - 8, y - 8); break;
        case TI_DETECTEUR : WindowManager::getInstance()->draw(image, 80, 32, 16, 16, x - 8, y - 8); break;
        case TI_TUNIQUE_BLEUE : WindowManager::getInstance()->draw(image, 80, 48, 16, 16, x - 8, y - 8); break;
        case TI_TUNIQUE_ROUGE : WindowManager::getInstance()->draw(image, 80, 64, 16, 16, x - 8, y - 8); break;
        case TI_CANNE_BYRNA : WindowManager::getInstance()->draw(image, 80, 80, 16, 16, x - 8, y - 8); break;
        case TI_BOTTES : WindowManager::getInstance()->draw(image, 80, 96, 16, 16, x - 8, y - 8); break;
        case TI_SAC_TROC : case TI_SAC_BOMBES : WindowManager::getInstance()->draw(image, 96, 0, 16, 16, x - 8, y - 8); break;
        case TI_POTION_ROUGE : WindowManager::getInstance()->draw(image, 112, 0, 16, 16, x - 8, y - 8); break;
        case TI_POTION_VERTE : WindowManager::getInstance()->draw(image, 112, 16, 16, 16, x - 8, y - 8); break;
        case TI_POTION_JAUNE : WindowManager::getInstance()->draw(image, 112, 32, 16, 16, x - 8, y - 8); break;
        case TI_POTION_BLEUE : WindowManager::getInstance()->draw(image, 112, 48, 16, 16, x - 8, y - 8); break;
        case TI_POTION_ORANGE : WindowManager::getInstance()->draw(image, 112, 64, 16, 16, x - 8, y - 8); break;
        case TI_POTION_VIOLETTE : WindowManager::getInstance()->draw(image, 112, 80, 16, 16, x - 8, y - 8); break;
        case TI_POTION_GRISE : WindowManager::getInstance()->draw(image, 112, 96, 16, 16, x - 8, y - 8); break;
        case TI_BOUSSOLE : WindowManager::getInstance()->draw(image, 96, 16, 16, 16, x - 8, y - 8); break;
        case TI_CLE_BOSS : WindowManager::getInstance()->draw(image, 96, 32, 16, 16, x - 8, y - 8); break;
        case TI_CLE : WindowManager::getInstance()->draw(image, 96, 48, 16, 16, x - 8, y - 8); break;
        case TI_MEDAILLON_1 : WindowManager::getInstance()->draw(imageMedaillons, 0, 0, 16, 16, x - 8, y - 8); break;
        case TI_MEDAILLON_2 : WindowManager::getInstance()->draw(imageMedaillons, 16, 0, 16, 16, x - 8, y - 8); break;
        case TI_MEDAILLON_3 : WindowManager::getInstance()->draw(imageMedaillons, 32, 0, 16, 16, x - 8, y - 8); break;
        case TI_MEDAILLON_4 : WindowManager::getInstance()->draw(imageMedaillons, 48, 0, 16, 16, x - 8, y - 8); break;
        case TI_MEDAILLON_5 : WindowManager::getInstance()->draw(imageMedaillons, 64, 0, 16, 16, x - 8, y - 8); break;
        case TI_MEDAILLON_6 : WindowManager::getInstance()->draw(imageMedaillons, 80, 0, 16, 16, x - 8, y - 8); break;
        case TI_MEDAILLON_7 : WindowManager::getInstance()->draw(imageMedaillons, 96, 0, 16, 16, x - 8, y - 8); break;
        case TI_GRAPPIN : WindowManager::getInstance()->draw(image, 96, 64, 16, 16, x - 8, y - 8); break;
        case TI_BAGUETTE_FEU : WindowManager::getInstance()->draw(image, 96, 96, 16, 16, x - 8, y - 8); break;
        case TI_CANNE_SOMARIA : WindowManager::getInstance()->draw(image, 96, 80, 16, 16, x - 8, y - 8); break;
        case TI_RUBIS_ROUGE : WindowManager::getInstance()->draw(image, 0, 35, 8, 14, x - 4, y - 7); break;
        case TI_CAPE_INVISIBILITE : WindowManager::getInstance()->draw(image, 128, 0, 16, 16, x - 8, y - 8); break;
        case TI_BAGUETTE_GLACE : WindowManager::getInstance()->draw(image, 128, 16, 16, 16, x - 8, y - 8); break;
        default : return;
    }
}
