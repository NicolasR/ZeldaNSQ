#include "GlobalSave.h"

GlobalSave GlobalSave::instance = GlobalSave();

GlobalSave* GlobalSave::getInstance() {
    return &instance;
}
GlobalSave::GlobalSave() {
    init();
    load();
}

GlobalSave::~GlobalSave() {
}

void GlobalSave::init() {
    scoreArc = 0;
    bestNormalTime = -1;
    normalFull = false;
    normalUlti = false;
    normalRush = false;
    bestExpertTime = -1;
    expertFull = false;
    expertUlti = false;
    expertRush = false;
    glass = false;
    unlockLink = false;
    unlockExpert = false;
    unlockOniLink = false;
}

void GlobalSave::load() {
    ifstream f("saves/records.dat", ios::in | ios::binary);
    if(!f.is_open()) {
        return;
    }
    f.read((char *)&scoreArc,sizeof(int));
    f.read((char *)&bestNormalTime,sizeof(int));
    f.read((char *)&normalFull,sizeof(bool));
    f.read((char *)&normalUlti,sizeof(bool));
    f.read((char *)&normalRush,sizeof(bool));
    f.read((char *)&bestExpertTime,sizeof(int));
    f.read((char *)&expertFull,sizeof(bool));
    f.read((char *)&expertUlti,sizeof(bool));
    f.read((char *)&expertRush,sizeof(bool));
    f.read((char *)&glass,sizeof(bool));

    f.close();
}

void GlobalSave::save() {
    ofstream f("saves/records.dat", ios::out | ios::binary);
    f.write((char *)&scoreArc,sizeof(int));
    f.write((char *)&bestNormalTime,sizeof(int));
    f.write((char *)&normalFull,sizeof(bool));
    f.write((char *)&normalUlti,sizeof(bool));
    f.write((char *)&normalRush,sizeof(bool));
    f.write((char *)&bestExpertTime,sizeof(int));
    f.write((char *)&expertFull,sizeof(bool));
    f.write((char *)&expertUlti,sizeof(bool));
    f.write((char *)&expertRush,sizeof(bool));
    f.write((char *)&glass,sizeof(bool));
    f.close();
}

void GlobalSave::update(Save* sv, bool endGame) {
    bool needToSave = false;

    if (sv->getCoffre(0, 11) > scoreArc) {
        scoreArc = sv->getCoffre(0, 11);
        needToSave = true;
    }

    if (endGame) {
        if (sv->getCoffre(9, 28) && !sv->isExpert() && (sv->getTime() < bestNormalTime || bestNormalTime == -1)) {
            bestNormalTime = sv->getTime();
            needToSave = true;
        }
        if (!normalFull && sv->getCoffre(9, 28) && !sv->isExpert() && sv->getCompletion() == 100) {
            normalFull = true;
            if (isExpertUnlocked()) unlockExpert = true;
            needToSave = true;
        }
        if (!normalUlti && sv->getCoffre(9, 28) && !sv->isExpert() && !sv->getDeaths()) {
            normalUlti = true;
            if (isExpertUnlocked()) unlockExpert = true;
            needToSave = true;
        }
        if (!normalRush && sv->getCoffre(9, 28) && !sv->isExpert() && sv->getTime() < 25200) {
            normalRush = true;
            if (isExpertUnlocked()) unlockExpert = true;
            needToSave = true;
        }
        if (sv->getCoffre(9, 28) && sv->isExpert() && (sv->getTime() < bestExpertTime || bestExpertTime == -1)) {
            bestExpertTime = sv->getTime();
            needToSave = true;
        }
        if (!expertFull && sv->getCoffre(9, 28) && sv->isExpert() && sv->getCompletion() == 100) {
            expertFull = true;
            if (isOniLinkUnlocked()) unlockOniLink = true;
            needToSave = true;
        }
        if (!expertUlti && sv->getCoffre(9, 28) && sv->isExpert() && !sv->getDeaths()) {
            expertUlti = true;
            if (isOniLinkUnlocked()) unlockOniLink = true;
            needToSave = true;
        }
        if (!expertRush && sv->getCoffre(9, 28) && sv->isExpert() && sv->getTime() < 25200) {
            expertRush = true;
            if (isOniLinkUnlocked()) unlockOniLink = true;
            needToSave = true;
        }
        if (!glass && sv->getCoffre(9, 28)) {
            unlockLink = true;
            glass = true;
            needToSave = true;
        }
    }

    if (needToSave) {
        save();
    }
}

int GlobalSave::getScoreArc() {return scoreArc;}
int GlobalSave::getBestNormalTime() {return bestNormalTime;}
bool GlobalSave::isNormalFull() {return normalFull;}
bool GlobalSave::isNormalUlti() {return normalUlti;}
bool GlobalSave::isNormalRush() {return normalRush;}
int GlobalSave::getBestExpertTime() {return bestExpertTime;}
bool GlobalSave::isExpertFull() {return expertFull;}
bool GlobalSave::isExpertUlti() {return expertUlti;}
bool GlobalSave::isExpertRush() {return expertRush;}
bool GlobalSave::isHolydaysLinkUnlock() {return glass;}
bool GlobalSave::isExpertUnlocked() {return normalFull && normalUlti && normalRush;}
bool GlobalSave::isOniLinkUnlocked() {return expertFull && expertUlti && expertRush;}

bool GlobalSave::haveUnlockLink() {return unlockLink;}
bool GlobalSave::haveUnlockExpert() {return unlockExpert;}
bool GlobalSave::haveUnlockOniLink() {return unlockOniLink;}

void GlobalSave::resetUnlock() {
    unlockLink = false;
    unlockExpert = false;
    unlockOniLink = false;
}
