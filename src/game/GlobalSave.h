/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GLOBALSAVE_H__
#define __GLOBALSAVE_H__

#include "Save.h"

class GlobalSave {
    public :
        static GlobalSave* getInstance();

        void save();

        void update(Save* save, bool endGame);

        int getScoreArc();
        int getBestNormalTime();
        bool isNormalFull();
        bool isNormalUlti();
        bool isNormalRush();
        int getBestExpertTime();
        bool isExpertFull();
        bool isExpertUlti();
        bool isExpertRush();

        bool isHolydaysLinkUnlock();
        bool isExpertUnlocked();
        bool isOniLinkUnlocked();

        bool haveUnlockLink();
        bool haveUnlockExpert();
        bool haveUnlockOniLink();
        void resetUnlock();

    private :
        GlobalSave();
        ~GlobalSave();
        static GlobalSave instance;

        void init();
        void load();

        int scoreArc;
        int bestNormalTime;
        bool normalFull;
        bool normalUlti;
        bool normalRush;
        int bestExpertTime;
        bool expertFull;
        bool expertUlti;
        bool expertRush;
        bool glass;

        bool unlockLink;
        bool unlockExpert;
        bool unlockOniLink;

};

#endif  // GlobalSave.h
