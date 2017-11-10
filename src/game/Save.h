/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __SAVE_H__
#define __SAVE_H__

#include "../engine/common/Common.h"

#include "content/CommonGame.h"

class Save {
    public :
        Save(int i);
        ~Save();

        void save(bool endGame = false);
        void erase();

        bool isLoaded();

        int getCompletion();

        int getMap();
        int getX();
        int getY();
        Direction getDirection();
        int getTunique();
        int getEpee();
        int getBouclier();
        int getLife();
        int getMaxLife();
        int getMagic();
        int getMaxMagic();
        int getNaviMagic();
        int getNaviMaxMagic();
        int getLvlHeal();
        int getLvlAtt();
        int getLvlDef();
        int getRupees();
        int getMaxRupees();
        int getLifeToAdd();
        int getMagicToAdd();
        int getRupeesToAdd();
        int getDeaths();
        int getEquipment(int i);
        int getFlacons(int i);
        int getQuartCoeur(int i);
        int getCoeur(int i);
        int getGemmeForce(int i);
        int getCoquillages(int i);
        int getMedaillons(int i);
        int getTroc(int i);
        int getArrows();
        int getMaxArrows();
        int getRecupArrows();
        int getBombs();
        int getMaxBombs();
        int getRecupBombs();
        Avancement getAvancement();
        int getTime();
        int getCoffre(int i, int j);
        int getFees(int i);
        int getRooms(int d, int i, int j);
        bool isExpert();

        void setMap(int i);
        void setX(int i);
        void setY(int i);
        void setDirection(Direction dir);
        void setTunique(int i);
        void setEpee(int i);
        void setBouclier(int i);
        void setLife(int i);
        void setMaxLife(int i);
        void setMagic(int i);
        void setMaxMagic(int i);
        void setNaviMagic(int i);
        void setNaviMaxMagic(int i);
        void setLvlHeal(int i);
        void setLvlAtt(int i);
        void setLvlDef(int i);
        void setRupees(int i);
        void setMaxRupees(int i);
        void setLifeToAdd(int i);
        void setMagicToAdd(int i);
        void setRupeesToAdd(int i);
        void setDeaths(int i);
        void setEquipment(int i, int value);
        void setFlacons(int i, int value);
        void setQuartCoeur(int i, int value);
        void setCoeur(int i, int value);
        void setGemmeForce(int i, int value);
        void setCoquillages(int i, int value);
        void setMedaillons(int i, int value);
        void setTroc(int i, int value);
        void setArrows(int i);
        void setMaxArrows(int i);
        void setRecupArrows(int i);
        void setBombs(int i);
        void setMaxBombs(int i);
        void setRecupBombs(int i);
        void setAvancement(Avancement avancement);
        void setTime(int t);
        void setCoffre(int i, int j, int val);
        void setFees(int i, int value);
        void setRooms(int d, int i, int j, int val);
        void setExpert(bool b);

    private :

        void init();
        void load();

        int id;
        bool loaded;


        int map;
        int x;
        int y;
        Direction direction;
        int tunique;
        int epee;
        int bouclier;

        int life;
        int maxLife;
        int magic;
        int maxMagic;
        int naviMagic;
        int naviMaxMagic;
        int lvlHeal;
        int lvlAtt;
        int lvlDef;
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

        int deaths;

        Avancement avancement;

        int time;

        int equipment[NB_EQPMT];
        int flacons[NB_FLACON];
        int quartCoeur[NB_QUART_COEUR];
        int coeur[NB_COEUR];
        int gemmeForce[NB_GEMME_FORCE];
        int coquillages[NB_COQUILLAGES];
        int medaillons[NB_MEDAILLONS];
        int troc[NB_TROC];
        int coffres[NB_COFFRES][COFFRES_MAX_SIZE];
        int fees[NB_FAIRIES];
        int rooms[NB_DONJON][MAX_ROOMS_X][MAX_ROOMS_Y];

        bool expert;

};

#endif  // Save.h
