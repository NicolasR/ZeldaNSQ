#include "Inventory.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

Inventory::Inventory(Save* save) {
    loadFromSave(save);
    setCurrent((Equipment)0);
    inventaire = ResourceManager::getInstance()->loadImage("data/images/status/inventaire.png");
    imageObjects = ResourceManager::getInstance()->loadImage("data/images/items/objects.png", true);
}

Inventory::~Inventory() {
    ResourceManager::getInstance()->free(inventaire);
    ResourceManager::getInstance()->free(imageObjects);
}

void Inventory::saveData(Save* save) {
    for (int i = 0; i < NB_EQPMT - 1; i++) {
        save->setEquipment(i, equipment[i]);
    }
    for (int i = 0; i < NB_FLACON; i++) {
        save->setFlacons(i, flacons[i]);
    }
    for (int i = 0; i < NB_QUART_COEUR; i++) {
        save->setQuartCoeur(i, quartCoeur[i]);
    }
    for (int i = 0; i < NB_COEUR; i++) {
        save->setCoeur(i, coeur[i]);
    }
    for (int i = 0; i < NB_GEMME_FORCE; i++) {
        save->setGemmeForce(i, gemmeForce[i]);
    }
    for (int i = 0; i < NB_COQUILLAGES; i++) {
        save->setCoquillages(i, coquillages[i]);
    }
    for (int i = 0; i < NB_MEDAILLONS; i++) {
        save->setMedaillons(i, medaillons[i]);
    }
    for (int i = 0; i < NB_TROC; i++) {
        save->setTroc(i, troc[i]);
    }
}

void Inventory::loadFromSave(Save* save) {
    for (int i = 0; i < NB_EQPMT - 1; i++) {
        equipment[i] = save->getEquipment(i);
    }
    equipment[NAVI_ULT] = 0;
    for (int i = 0; i < NB_FLACON; i++) {
        flacons[i] = save->getFlacons(i);
    }
    for (int i = 0; i < NB_QUART_COEUR; i++) {
        quartCoeur[i] = save->getQuartCoeur(i);
    }
    for (int i = 0; i < NB_COEUR; i++) {
        coeur[i] = save->getCoeur(i);
    }
    for (int i = 0; i < NB_GEMME_FORCE; i++) {
        gemmeForce[i] = save->getGemmeForce(i);
    }
    for (int i = 0; i < NB_COQUILLAGES; i++) {
        coquillages[i] = save->getCoquillages(i);
    }
    for (int i = 0; i < NB_MEDAILLONS; i++) {
        medaillons[i] = save->getMedaillons(i);
    }
    for (int i = 0; i < NB_TROC; i++) {
        troc[i] = save->getTroc(i);
    }
}

void Inventory::draw(Equipment e, int x, int y) {
    switch (e) {
        case ARC : WindowManager::getInstance()->draw(inventaire, 16, 68, 16, 17, x, y); break;
        case ARC_FEE : WindowManager::getInstance()->draw(inventaire, 0, 0, 16, 17, x, y); break;
        case BOOMERANG : WindowManager::getInstance()->draw(inventaire, 48, 0, 16, 17, x, y); break;
        case BOOMERANG_FEE : WindowManager::getInstance()->draw(inventaire, 48, 17, 16, 17, x, y); break;
        case BOMBES : WindowManager::getInstance()->draw(inventaire, 32, 0, 16, 17, x, y); break;
        case GRAPPIN : WindowManager::getInstance()->draw(inventaire, 16, 0, 16, 17, x, y); break;
        case BAGUETTE_FEU : WindowManager::getInstance()->draw(inventaire, 0, 17, 16, 17, x, y); break;
        case BAGUETTE_GLACE : WindowManager::getInstance()->draw(inventaire, 16, 17, 16, 17, x, y); break;
        case LANTERNE : WindowManager::getInstance()->draw(inventaire, 32, 17, 16, 17, x, y); break;
        case CAPE_INVISIBILITE : WindowManager::getInstance()->draw(inventaire, 32, 68, 16, 17, x, y); break;
        case MARTEAU : WindowManager::getInstance()->draw(inventaire, 0, 34, 16, 17, x, y); break;
        case OCARINA : WindowManager::getInstance()->draw(inventaire, 0, 68, 16, 17, x, y); break;
        case CANNE_SOMARIA : WindowManager::getInstance()->draw(inventaire, 0, 85, 16, 17, x, y); break;
        case CANNE_BYRNA : WindowManager::getInstance()->draw(inventaire, 16, 85, 16, 17, x, y); break;
        case FLACON_1 : drawFlacon(x, y, flacons[0]); break;
        case FLACON_2 : drawFlacon(x, y, flacons[1]); break;
        case FLACON_3 : drawFlacon(x, y, flacons[2]); break;
        case FLACON_4 : drawFlacon(x, y, flacons[3]); break;
        case DETECTEUR : WindowManager::getInstance()->draw(inventaire, 48, 68, 16, 17, x, y); break;
        case PERLE_LUNE : WindowManager::getInstance()->draw(inventaire, 32, 85, 16, 17, x, y); break;
        case BOTTES : WindowManager::getInstance()->draw(inventaire, 48, 85, 16, 17, x, y); break;
        case GANTS : WindowManager::getInstance()->draw(inventaire, 48, 34, 16, 17, x, y); break;
        case GANTS_2 : WindowManager::getInstance()->draw(inventaire, 32, 34, 16, 17, x, y); break;
        case PALMES : WindowManager::getInstance()->draw(inventaire, 16, 34, 16, 17, x, y); break;
        case CARTE_ILE : WindowManager::getInstance()->draw(inventaire, 48, 102, 16, 17, x, y); break;
        case CHANT_1 : WindowManager::getInstance()->draw(inventaire, 0, 102, 16, 17, x, y); break;
        case CHANT_2 : WindowManager::getInstance()->draw(inventaire, 16, 102, 16, 17, x, y); break;
        case CHANT_3 : WindowManager::getInstance()->draw(inventaire, 32, 102, 16, 17, x, y); break;
        default : break;
    }
}

void Inventory::drawTroc(TypeTroc t, int x, int y) {
    switch (t) {
        case TT_SAC_RIZ : WindowManager::getInstance()->draw(imageObjects, 0, 0, 16, 16, x, y); break;
        case TT_BOCAL_EPICES : WindowManager::getInstance()->draw(imageObjects, 16, 0, 16, 16, x, y); break;
        case TT_SAC_OLIVES : WindowManager::getInstance()->draw(imageObjects, 32, 0, 16, 16, x, y); break;
        case TT_PERLES : WindowManager::getInstance()->draw(imageObjects, 48, 0, 16, 16, x, y); break;
        case TT_PLUME : WindowManager::getInstance()->draw(imageObjects, 64, 0, 16, 16, x, y); break;
        case TT_POULET : WindowManager::getInstance()->draw(imageObjects, 80, 0, 16, 16, x, y); break;
        case TT_TAPISSERIE : WindowManager::getInstance()->draw(imageObjects, 96, 0, 16, 16, x, y); break;
        case TT_AMULETTE : WindowManager::getInstance()->draw(imageObjects, 112, 0, 16, 16, x, y); break;
        case TT_PARFUM : WindowManager::getInstance()->draw(imageObjects, 128, 0, 16, 16, x, y); break;
        case TT_ROC_GIVRE : WindowManager::getInstance()->draw(imageObjects, 144, 0, 16, 16, x, y); break;
        default : break;
    }
}

void Inventory::drawFlacon(int x, int y, int value) {
    WindowManager::getInstance()->draw(inventaire, (value%4) * 16, 51 + 68 * (value / 4), 16, 17, x, y);
}

void Inventory::setObject(Equipment e, int value) {
    equipment[e] = value;
}

bool Inventory::hasObject(Equipment e) {
    return (equipment[e] == 1);
}

bool Inventory::hasObject() {
    for (int i = 0; i < NB_EQPMT; i++) {
        if (equipment[i]
            && i != NAVI_HEAL
            && i != SAC_TROC
            && i != CARTE_ILE) return true;
    }
    return false;
}

Equipment Inventory::getCurrent() {
    return current;
}

void Inventory::setCurrent(Equipment e) {
    current = e;
    if (current == ARC && hasObject(ARC_FEE)) {
        current = ARC_FEE;
    }
    if (current == BOOMERANG && hasObject(BOOMERANG_FEE)) {
        current = BOOMERANG_FEE;
    }
}

bool Inventory::hasQuartCoeur(int i) {
    return (quartCoeur[i] == 1);
}

void Inventory::setQuartCoeur(int i, int value) {
    quartCoeur[i] = value;
}

int Inventory::nbQuartCoeur() {
    return totalQuartCoeur() % 4;
}

int Inventory::totalQuartCoeur() {
    int total = 0;
    for (int i = 0; i < NB_QUART_COEUR; i++) {
        if (quartCoeur[i]) total++;
    }
    return total;
}

bool Inventory::hasCoeur(int i) {
    return (coeur[i] == 1);
}

void Inventory::setCoeur(int i, int value) {
    coeur[i] = value;
}

void Inventory::setGemmeForce(int i, int value) {
    gemmeForce[i] = value;
}

bool Inventory::hasGemmeForce(int i) {
    return gemmeForce[i];
}

int Inventory::nbGemmeForce() {
    int total = 0;
    for (int i = 0; i < NB_GEMME_FORCE; i++) {
        if (gemmeForce[i] == 1) total++; // == 2 if already used
    }
    return total;
}

int Inventory::totalGemmeForce() {
    int total = 0;
    for (int i = 0; i < NB_GEMME_FORCE; i++) {
        if (gemmeForce[i]) total++;
    }
    return total;
}

void Inventory::removeGemme(int nb) {
    int total = nb;
    for (int i = 0; i < NB_GEMME_FORCE; i++) {
        if (!total) return;
        if (gemmeForce[i] == 1) {
            gemmeForce[i] = 2;
            total--;
        }
    }
}

void Inventory::setCoquillages(int i, int value) {
    coquillages[i] = value;
}

bool Inventory::hasCoquillages(int i) {
    return coquillages[i]; // can be 1 or 2 if already used
}

int Inventory::nbCoquillages() {
    int total = 0;
    for (int i = 0; i < NB_COQUILLAGES; i++) {
        if (coquillages[i] == 1) total++; // == 2 if already used
    }
    return total;
}

int Inventory::totalCoquillages() {
    int total = 0;
    for (int i = 0; i < NB_COQUILLAGES; i++) {
        if (coquillages[i]) total++;
    }
    return total;
}

void Inventory::removeCoquillages(int nb) {
    int total = nb;
    for (int i = 0; i < NB_COQUILLAGES; i++) {
        if (!total) return;
        if (coquillages[i] == 1) {
            coquillages[i] = 2;
            total--;
        }
    }
}

void Inventory::setMedaillons(int i, int value) {
    medaillons[i] = value;
}

bool Inventory::hasMedaillons(int i) {
    return (medaillons[i] == 1);
}

int Inventory::nbMedaillons() {
    int total = 0;
    for (int i = 0; i < NB_MEDAILLONS; i++) {
        if (medaillons[i]) total++;
    }
    return total;
}

int Inventory::getFlacon(int i) {
    return flacons[i];
}

void Inventory::setFlacon(int i, int value) {
    flacons[i] = value;
}

int Inventory::getCurrentFlacon() {
    switch (current) {
        case FLACON_1 : return flacons[0];
        case FLACON_2 : return flacons[1];
        case FLACON_3 : return flacons[2];
        case FLACON_4 : return flacons[3];
        default : return 0;
    }
}

void Inventory::setCurrentFlacon(int value) {
    switch (current) {
        case FLACON_1 : flacons[0] = value; break;
        case FLACON_2 : flacons[1] = value; break;
        case FLACON_3 : flacons[2] = value; break;
        case FLACON_4 : flacons[3] = value; break;
        default : break;
    }
}

int Inventory::getTroc(int i) {
    return troc[i];
}

void Inventory::setTroc(int i, int value) {
    troc[i] = value;
}

void Inventory::addTroc(int i) {
    if (troc[i] == -1) {
        troc[i] = 1;
    } else {
        troc[i]++;
    }
}

void Inventory::removeTroc(int i, int val) {
    troc[i] -= val;
}
