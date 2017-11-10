/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __STATUS_H__
#define __STATUS_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/util/time/Chrono.h"

#include "../../Save.h"

class Status {
    public :
        Status(Save* save);
        ~Status();

        void saveData(Save* save);

        void loop();

        int getLife();
        int getMaxLife();
        int getMagic();
        int getMaxMagic();
        int getRupees();
        int getMaxRupees();
        int getArrows();
        int getMaxArrows();
        int getRecupArrows();
        int getBombs();
        int getMaxBombs();
        int getRecupBombs();

        void setLife(int i);
        void setMaxLife(int i);
        void setMagic(int i);
        void setMaxMagic(int i);
        void setRupees(int i);
        void setMaxRupees(int i);
        void setArrows(int i);
        void setMaxArrows(int i);
        void setRecupArrows(int i);
        void setBombs(int i);
        void setMaxBombs(int i);
        void setRecupBombs(int i);

        void updateLife(int i);
        void updateRupees(int i);
        void updateMagic(int i);

        int getVirtualLife();
        int getVirtualRupees();
        int getVirtualMagic();

        void setInvincible();
        bool isInvincible();

        void setVisible(bool visible);
        bool isVisible();

        void increaseDeaths();
        int getNbDeaths();

        int getMaxWeight();

        void updateGel(int value);
        void resetGel();
        int getGel();
        int getMaxGel();

        void updateOniLife(int value);
        void resetOniLife();
        int getOniLife();
        bool isOniLink();
        void setOniLink(bool onilink);

    private :

        void loadFromSave(Save* save);

        Chrono chrono;
        int timer;

        int life;
        int maxLife;
        int magic;
        int maxMagic;
        int rupees;
        int maxRupees;
        int arrows;
        int maxArrows;
        int recupArrows;
        int bombs;
        int maxBombs;
        int recupBombs;

        int lifeToAdd;
        int magicToAdd;
        int rupeesToAdd;

        int invincible;
        int visible;

        int deaths;

        int regenMagie;

        int gel;
        int maxGel;

        int oniLife;
        bool onilink;
};

#endif  // Status.h
