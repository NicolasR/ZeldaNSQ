#include "Save.h"

#include "GlobalSave.h"

Save::Save(int i) : id (i), loaded(false) {
    init();
    load();
}

Save::~Save() {
}

void Save::init() {
    // set default values
    map = 1;
    x = 16 * 87;
    y = 16 * 48;
    direction = S;
    tunique = 1;
    epee = 0;
    bouclier = 0;
    life = 0;//6;
    maxLife = 6;
    magic = 0;
    maxMagic = 0;//32;
    naviMagic = 0;
    naviMaxMagic = 0; // set to 20 after the tutoriel
    lvlHeal = 1;
    lvlAtt = 1;
    lvlDef = 1;
    rupees = 0;
    maxRupees = 0;//99;
    arrows = 0;
    maxArrows = 0;//30;
    recupArrows = 0;
    bombs = 0;
    maxBombs = 0;//10;
    recupBombs = 0;
    lifeToAdd = 0;
    magicToAdd = 0;
    rupeesToAdd = 0;
    deaths = 0;
    avancement = AV_START;
    time = 0;

    for (int i = 0; i < NB_EQPMT; i++) {
        equipment[i] = 0;
    }

    for (int i = 0; i < NB_FLACON; i++) {
        flacons[i] = 0;
    }

    for (int i = 0; i < NB_QUART_COEUR; i++) {
        quartCoeur[i] = 0;
    }

    for (int i = 0; i < NB_COEUR; i++) {
        coeur[i] = 0;
    }

    for (int i = 0; i < NB_GEMME_FORCE; i++) {
        gemmeForce[i] = 0;
    }

    for (int i = 0; i < NB_COQUILLAGES; i++) {
        coquillages[i] = 0;
    }

    for (int i = 0; i < NB_MEDAILLONS; i++) {
        medaillons[i] = 0;
    }

    for (int i = 0; i < NB_TROC; i++) {
        troc[i] = -1;
    }

    for (int j = 0; j < COFFRES_MAX_SIZE; j++) {
        for (int i = 0; i < NB_COFFRES; i++) {
            coffres[i][j] = 0;
        }
    }

    for (int i = 0; i < NB_FAIRIES; i++) {
        fees[i] = 0;
    }

    for (int j = 0; j < MAX_ROOMS_Y; j++) {
        for (int i = 0; i < MAX_ROOMS_X; i++) {
            for (int d = 0; d < NB_DONJON; d++) {
                rooms[d][i][j] = 0;
            }
        }
    }

    expert = false;
}

void Save::load() {
    ostringstream im;
    im << id;
    ifstream f(("saves/znq" + im.str() + ".dat").c_str(), ios::in | ios::binary);
    if(!f.is_open()) {
        return;
    }
    f.read((char *)&map,sizeof(int));
    f.read((char *)&x,sizeof(int));
    f.read((char *)&y,sizeof(int));
    f.read((char *)&direction,sizeof(int));
    f.read((char *)&tunique,sizeof(int));
    f.read((char *)&epee,sizeof(int));
    f.read((char *)&bouclier,sizeof(int));
    f.read((char *)&life,sizeof(int));
    f.read((char *)&maxLife,sizeof(int));
    f.read((char *)&magic,sizeof(int));
    f.read((char *)&maxMagic,sizeof(int));
    f.read((char *)&naviMagic,sizeof(int));
    f.read((char *)&naviMaxMagic,sizeof(int));
    f.read((char *)&lvlHeal,sizeof(int));
    f.read((char *)&lvlAtt,sizeof(int));
    f.read((char *)&lvlDef,sizeof(int));
    f.read((char *)&rupees,sizeof(int));
    f.read((char *)&maxRupees,sizeof(int));
    f.read((char *)&arrows,sizeof(int));
    f.read((char *)&maxArrows,sizeof(int));
    f.read((char *)&recupArrows,sizeof(int));
    f.read((char *)&bombs,sizeof(int));
    f.read((char *)&maxBombs,sizeof(int));
    f.read((char *)&recupBombs,sizeof(int));
    f.read((char *)&lifeToAdd,sizeof(int));
    f.read((char *)&magicToAdd,sizeof(int));
    f.read((char *)&rupeesToAdd,sizeof(int));
    f.read((char *)&deaths,sizeof(int));
    f.read((char *)&avancement,sizeof(int));
    f.read((char *)&time,sizeof(int));
    for (int i = 0; i < NB_EQPMT - 1; i++) f.read((char *)&equipment[i],sizeof(int));
    for (int i = 0; i < NB_FLACON; i++) f.read((char *)&flacons[i],sizeof(int));
    for (int i = 0; i < NB_QUART_COEUR; i++) f.read((char *)&quartCoeur[i],sizeof(int));
    for (int i = 0; i < NB_COEUR; i++) f.read((char *)&coeur[i],sizeof(int));
    for (int i = 0; i < NB_GEMME_FORCE; i++) f.read((char *)&gemmeForce[i],sizeof(int));
    for (int i = 0; i < NB_COQUILLAGES; i++) f.read((char *)&coquillages[i],sizeof(int));
    for (int i = 0; i < NB_MEDAILLONS; i++) f.read((char *)&medaillons[i],sizeof(int));
    for (int i = 0; i < NB_TROC; i++) f.read((char *)&troc[i],sizeof(int));
    for (int i = 0; i < NB_FAIRIES; i++) f.read((char *)&fees[i],sizeof(int));
    for (int j = 0; j < COFFRES_MAX_SIZE; j++) for (int i = 0; i < NB_COFFRES; i++) f.read((char *)&coffres[i][j],sizeof(int));
    for (int j = 0; j < MAX_ROOMS_Y; j++) for (int i = 0; i < MAX_ROOMS_X; i++) for (int d = 0; d < NB_DONJON; d++) f.read((char *)&rooms[d][i][j],sizeof(int));
    f.read((char *)&expert,sizeof(bool));
    f.close();
    loaded = true;
}

void Save::save(bool endGame) {
    ostringstream im;
    im << id;
    ofstream f(("saves/znq" + im.str() + ".dat").c_str(), ios::out | ios::binary);
    f.write((char *)&map,sizeof(int));
    f.write((char *)&x,sizeof(int));
    f.write((char *)&y,sizeof(int));
    f.write((char *)&direction,sizeof(int));
    f.write((char *)&tunique,sizeof(int));
    f.write((char *)&epee,sizeof(int));
    f.write((char *)&bouclier,sizeof(int));
    f.write((char *)&life,sizeof(int));
    f.write((char *)&maxLife,sizeof(int));
    f.write((char *)&magic,sizeof(int));
    f.write((char *)&maxMagic,sizeof(int));
    f.write((char *)&naviMagic,sizeof(int));
    f.write((char *)&naviMaxMagic,sizeof(int));
    f.write((char *)&lvlHeal,sizeof(int));
    f.write((char *)&lvlAtt,sizeof(int));
    f.write((char *)&lvlDef,sizeof(int));
    f.write((char *)&rupees,sizeof(int));
    f.write((char *)&maxRupees,sizeof(int));
    f.write((char *)&arrows,sizeof(int));
    f.write((char *)&maxArrows,sizeof(int));
    f.write((char *)&recupArrows,sizeof(int));
    f.write((char *)&bombs,sizeof(int));
    f.write((char *)&maxBombs,sizeof(int));
    f.write((char *)&recupBombs,sizeof(int));
    f.write((char *)&lifeToAdd,sizeof(int));
    f.write((char *)&magicToAdd,sizeof(int));
    f.write((char *)&rupeesToAdd,sizeof(int));
    f.write((char *)&deaths,sizeof(int));
    f.write((char *)&avancement,sizeof(int));
    f.write((char *)&time,sizeof(int));
    for (int i = 0; i < NB_EQPMT - 1; i++) f.write((char *)&equipment[i],sizeof(int));
    for (int i = 0; i < NB_FLACON; i++) f.write((char *)&flacons[i],sizeof(int));
    for (int i = 0; i < NB_QUART_COEUR; i++) f.write((char *)&quartCoeur[i],sizeof(int));
    for (int i = 0; i < NB_COEUR; i++) f.write((char *)&coeur[i],sizeof(int));
    for (int i = 0; i < NB_GEMME_FORCE; i++) f.write((char *)&gemmeForce[i],sizeof(int));
    for (int i = 0; i < NB_COQUILLAGES; i++) f.write((char *)&coquillages[i],sizeof(int));
    for (int i = 0; i < NB_MEDAILLONS; i++) f.write((char *)&medaillons[i],sizeof(int));
    for (int i = 0; i < NB_TROC; i++) f.write((char *)&troc[i],sizeof(int));
    for (int i = 0; i < NB_FAIRIES; i++) f.write((char *)&fees[i],sizeof(int));
    for (int j = 0; j < COFFRES_MAX_SIZE; j++) for (int i = 0; i < NB_COFFRES; i++) f.write((char *)&coffres[i][j],sizeof(int));
    for (int j = 0; j < MAX_ROOMS_Y; j++) for (int i = 0; i < MAX_ROOMS_X; i++) for (int d = 0; d < NB_DONJON; d++) f.write((char *)&rooms[d][i][j],sizeof(int));
    f.write((char *)&expert,sizeof(bool));
    f.close();
    GlobalSave::getInstance()->update(this, endGame);
    loaded = true;
}

void Save::erase() {
    ostringstream im;
    im << id;
    remove(("saves/znq" + im.str() + ".dat").c_str());
    init();
    loaded = false;
}

bool Save::isLoaded() {return loaded;}

int Save::getMap() {return map;}
int Save::getX() {return x;}
int Save::getY() {return y;}
Direction Save::getDirection() {return direction;}
int Save::getTunique() {return tunique;}
int Save::getEpee() {return epee;}
int Save::getBouclier() {return bouclier;}
int Save::getLife() {return life;}
int Save::getMaxLife() {return maxLife;}
int Save::getMagic() {return magic;}
int Save::getMaxMagic() {return maxMagic;}
int Save::getNaviMagic() {return naviMagic;}
int Save::getNaviMaxMagic() {return naviMaxMagic;}
int Save::getLvlHeal() {return lvlHeal;}
int Save::getLvlAtt() {return lvlAtt;}
int Save::getLvlDef() {return lvlDef;}
int Save::getRupees() {return rupees;}
int Save::getMaxRupees() {return maxRupees;}
int Save::getArrows() {return arrows;}
int Save::getMaxArrows() {return maxArrows;}
int Save::getRecupArrows() {return recupArrows;}
int Save::getBombs() {return bombs;}
int Save::getMaxBombs() {return maxBombs;}
int Save::getRecupBombs() {return recupBombs;}
int Save::getLifeToAdd() {return lifeToAdd;}
int Save::getMagicToAdd() {return magicToAdd;}
int Save::getRupeesToAdd() {return rupeesToAdd;}
int Save::getDeaths() {return deaths;}
Avancement Save::getAvancement() {return avancement;}
int Save::getTime() {return time;}
int Save::getEquipment(int i) {return equipment[i];}
int Save::getFlacons(int i) {return flacons[i];}
int Save::getQuartCoeur(int i) {return quartCoeur[i];}
int Save::getCoeur(int i) {return coeur[i];}
int Save::getGemmeForce(int i) {return gemmeForce[i];}
int Save::getCoquillages(int i) {return coquillages[i];}
int Save::getMedaillons(int i) {return medaillons[i];}
int Save::getTroc(int i) {return troc[i];}
int Save::getCoffre(int i, int j) {return coffres[i][j];}
int Save::getFees(int i) {return fees[i];}
int Save::getRooms(int d, int i, int j) {return rooms[d][i][j];}
bool Save::isExpert() {return expert;}

void Save::setMap(int i) {map = i;}
void Save::setX(int i) {x = i;}
void Save::setY(int i) {y = i;}
void Save::setDirection(Direction dir) {direction = dir;}
void Save::setTunique(int i) {tunique = i;}
void Save::setEpee(int i) {epee = i;}
void Save::setBouclier(int i) {bouclier = i;}
void Save::setLife(int i) {life = i;}
void Save::setMaxLife(int i) {maxLife = i;}
void Save::setMagic(int i) {magic = i;}
void Save::setMaxMagic(int i) {maxMagic = i;}
void Save::setNaviMagic(int i) {naviMagic = i;}
void Save::setNaviMaxMagic(int i) {naviMaxMagic = i;}
void Save::setLvlHeal(int i) {lvlHeal = i;}
void Save::setLvlAtt(int i) {lvlAtt = i;}
void Save::setLvlDef(int i) {lvlDef = i;}
void Save::setRupees(int i) {rupees = i;}
void Save::setMaxRupees(int i) {maxRupees = i;}
void Save::setArrows(int i) {arrows = i;}
void Save::setMaxArrows(int i) {maxArrows = i;}
void Save::setRecupArrows(int i) {recupArrows = i;}
void Save::setBombs(int i) {bombs = i;}
void Save::setMaxBombs(int i) {maxBombs = i;}
void Save::setRecupBombs(int i) {recupBombs = i;}
void Save::setLifeToAdd(int i) {lifeToAdd = i;}
void Save::setMagicToAdd(int i) {magicToAdd = i;}
void Save::setRupeesToAdd(int i) {rupeesToAdd = i;}
void Save::setDeaths(int i) {deaths = i;}
void Save::setAvancement(Avancement av) {avancement = av;}
void Save::setTime(int i) {time = i;}
void Save::setEquipment(int i, int value) {equipment[i] = value;}
void Save::setFlacons(int i, int value) {flacons[i] = value;}
void Save::setQuartCoeur(int i, int value) {quartCoeur[i] = value;}
void Save::setCoeur(int i, int value) {coeur[i] = value;}
void Save::setGemmeForce(int i, int value) {gemmeForce[i] = value;}
void Save::setCoquillages(int i, int value) {coquillages[i] = value;}
void Save::setMedaillons(int i, int value) {medaillons[i] = value;}
void Save::setTroc(int i, int value) {troc[i] = value;}
void Save::setCoffre(int i, int j, int value) {coffres[i][j] = value;}
void Save::setFees(int i, int value) {fees[i] = value;}
void Save::setRooms(int d, int i, int j, int value) {rooms[d][i][j] = value;}
void Save::setExpert(bool b) {expert = b;}

int Save::getCompletion() {
    int tmp = 0;

    for (int i = 0; i < NB_QUART_COEUR; i++) if (quartCoeur[i] != 0) tmp++;
    for (int i = 0; i < NB_GEMME_FORCE; i++) if (gemmeForce[i] != 0) tmp++;
    for (int i = 0; i < NB_COQUILLAGES; i++) if (coquillages[i] != 0) tmp++;
    for (int i = 0; i < NB_TROC; i++) if (troc[i] != -1) tmp++;
    tmp += ((naviMaxMagic - 15) / 5);
    for (int i = 0; i < NB_EQPMT - 1; i++) if (equipment[i] != 0) tmp++;
    tmp += bouclier;
    tmp += tunique - 1;
    tmp += epee;
    if (epee == 5) tmp--;
    if (maxMagic > 32) tmp++;
    tmp += (maxArrows - 30) / 5;
    tmp += (maxBombs - 10) / 2;
    if (maxRupees > 0) tmp++;
    if (maxRupees > 99) tmp++;
    if (maxRupees > 250) tmp++;
    if (maxRupees > 500) tmp++;
    for (int i = 0; i < NB_MEDAILLONS; i++) if (medaillons[i] != 0) tmp++;
    for (int j = 1; j < NB_DONJON; j++) for (int i = 0; i < 3; i++) if (coffres[j][i] != 0) tmp++;

    return ((double)((double)tmp * 100)) / 247;
}

