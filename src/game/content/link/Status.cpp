#include "Status.h"

#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../effects/FumeeBlanche.h"

#include "../../MainController.h"

Status::Status(Save* save) : timer(0), invincible(0), visible(true), regenMagie(0), gel(0), maxGel(0), oniLife(0), onilink(false) {
    loadFromSave(save);
    chrono.reset();
}

Status::~Status() {
}

void Status::loadFromSave(Save* save) {
    life = save->getLife();
    maxLife = save->getMaxLife();
    magic = save->getMagic();
    maxMagic = save->getMaxMagic();
    rupees = save->getRupees();
    maxRupees = save->getMaxRupees();
    arrows = save->getArrows();
    maxArrows = save->getMaxArrows();
    recupArrows = save->getRecupArrows();
    bombs = save->getBombs();
    maxBombs = save->getMaxBombs();
    recupBombs = save->getRecupBombs();
    lifeToAdd = save->getLifeToAdd();
    magicToAdd = save->getMagicToAdd();
    rupeesToAdd = save->getRupeesToAdd();
    deaths = save->getDeaths();
}

void Status::saveData(Save* save) {
    save->setLife(life);
    save->setMaxLife(maxLife);
    save->setMagic(magic);
    save->setMaxMagic(maxMagic);
    save->setRupees(rupees);
    save->setMaxRupees(maxRupees);
    save->setArrows(arrows);
    save->setMaxArrows(maxArrows);
    save->setRecupArrows(recupArrows);
    save->setBombs(bombs);
    save->setMaxBombs(maxBombs);
    save->setRecupBombs(recupBombs);
    save->setLifeToAdd(lifeToAdd);
    save->setMagicToAdd(magicToAdd);
    save->setRupeesToAdd(rupeesToAdd);
    save->setDeaths(deaths);
}

void Status::loop() {

    if (gel > 0) gel--;

    if (rupeesToAdd > 0) {
        if (rupees < maxRupees) {
            rupeesToAdd--;
            setRupees(rupees + 1);
        } else {
            rupeesToAdd = 0;
        }
    } else if (rupeesToAdd < 0) {
        if (rupees > 0) {
            rupeesToAdd++;
            setRupees(rupees - 1);
        } else {
            rupeesToAdd = 0;
        }
    }

    if (chrono.getElapsedTime() >= 80) {

        if (getMaxMagic() == 64) {
            regenMagie++;
            if (regenMagie == 32) {
                regenMagie = 0;
                magicToAdd++;
            }
        }

        if (!visible && timer % 10 == 0) {
            magicToAdd--;
            if (getVirtualMagic() <= 0) {
                setVisible(true);
            }
        }

        if (timer == 0 && recupArrows && arrows < maxArrows) {
            arrows++;
        }
        if (timer == 0 && recupBombs && bombs < maxBombs) {
            bombs++;
        }

        if (lifeToAdd > 0) {
            if (life < maxLife) {
                lifeToAdd--;
                setLife(life + 1);
                if (life % 2 == 0) {
                    AudioManager::getInstance()->playSound(TS_HEART);
                }
            } else {
                lifeToAdd = 0;
            }
        } else if (lifeToAdd < 0) {
            if (life > 0) {
                lifeToAdd++;
                setLife(life - 1);
            } else {
                lifeToAdd = 0;
            }
        }

        if (magicToAdd > 0) {
            if (magic < maxMagic) {
                magicToAdd--;
                setMagic(magic + 1);
            } else {
                magicToAdd = 0;
            }
        } else if (magicToAdd < 0) {
            if (magic > 0) {
                magicToAdd++;
                setMagic(magic - 1);
            } else {
                magicToAdd = 0;
            }
        }

        if (isInvincible()) {
            invincible--;
        }

        chrono.reset();

        timer++;
        if (timer == 60) timer = 0;
    }
}

void Status::updateLife(int i) {
    lifeToAdd += i;
    if (life + lifeToAdd > maxLife) lifeToAdd = maxLife - life;
}

void Status::updateMagic(int i) {
    magicToAdd += i;
    if (magic + magicToAdd > maxMagic) magicToAdd = maxMagic - magic;
}

void Status::updateRupees(int i) {
    rupeesToAdd += i;
    if (rupees + rupeesToAdd > maxRupees) rupeesToAdd = maxRupees - rupees;
}

int Status::getVirtualLife() {
    return life + lifeToAdd;
}

int Status::getVirtualMagic() {
    return magic + magicToAdd;
}

int Status::getVirtualRupees() {
    return rupees + rupeesToAdd;
}

int Status::getLife() {return life;}
int Status::getMaxLife() {return maxLife;}
int Status::getMagic() {return magic;}
int Status::getMaxMagic() {return maxMagic;}
int Status::getRupees() {return rupees;}
int Status::getMaxRupees() {return maxRupees;}
int Status::getArrows() {return arrows;}
int Status::getMaxArrows() {return maxArrows;}
int Status::getRecupArrows() {return recupArrows;}
int Status::getBombs() {return bombs;}
int Status::getMaxBombs() {return maxBombs;}
int Status::getRecupBombs() {return recupBombs;}

void Status::setLife(int i) {
    if (i > maxLife) i = maxLife;
    life = i;
    if (life + lifeToAdd > 0 && life <= maxLife / 3) {
        AudioManager::getInstance()->playSound(TS_DANGER);
    }

    if (oniLife > 0 && oniLife >= life && getVirtualLife() > 0) {
        oniLife = 0;
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        Link* link = scene->getLink();
        link->setOniLink(true);
    }
}
void Status::setMaxLife(int i) {maxLife = i;}
void Status::setMagic(int i) {if (i > maxMagic) i = maxMagic; magic = i;}
void Status::setMaxMagic(int i) {maxMagic = i;}
void Status::setRupees(int i) {if (i > maxRupees) i = maxRupees; rupees = i;}
void Status::setMaxRupees(int i) {maxRupees = i;}
void Status::setArrows(int i) {if (i > maxArrows) i = maxArrows; arrows = i;}
void Status::setMaxArrows(int i) {maxArrows = i;}
void Status::setRecupArrows(int i) {recupArrows = i;}
void Status::setBombs(int i) {if (i > maxBombs) i = maxBombs; bombs = i;}
void Status::setMaxBombs(int i) {maxBombs = i;}
void Status::setRecupBombs(int i) {recupBombs = i;}

void Status::setInvincible() {invincible = 10;}
bool Status::isInvincible() {return (invincible > 0);}

void Status::setVisible(bool v) {
    if (visible != v) {
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        Link* link = scene->getLink();
        scene->getMap()->addEffect(new FumeeBlanche(link->getX() + 8, link->getY() + 12, 4));
        AudioManager::getInstance()->playSound(v ? TS_CAPE_OFF : TS_CAPE_ON);
        visible = v;
    }
}

bool Status::isVisible() {return visible;}

void Status::increaseDeaths() {deaths ++;}

int Status::getNbDeaths() {
    return deaths;
}

int Status::getMaxWeight() {
    Inventory* inventory = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink()->getInventory();
    if (inventory->hasObject(GANTS_2)) return 4;
    if (inventory->hasObject(GANTS)) return 2;
    return 1;
}

void Status::updateGel(int value) {
    gel += value;
    if (gel > 5000) gel = 5000;
    maxGel = gel;
}
void Status::resetGel() {
    gel = 0;
}
int Status::getGel() {
    return gel;
}
int Status::getMaxGel() {
    return maxGel;
}

void Status::updateOniLife(int value) {
    oniLife += value;

    if (oniLife > 0 && oniLife >= life && getVirtualLife() > 0) {
        oniLife = 0;
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        Link* link = scene->getLink();
        link->setOniLink(true);
    }
}

void Status::resetOniLife() {
    oniLife = 0;
}

int Status::getOniLife() {
    return oniLife;
}

bool Status::isOniLink() {
    return onilink;
}

void Status::setOniLink(bool b) {
    onilink = b;
}
