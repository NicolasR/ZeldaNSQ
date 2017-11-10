/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI066_H__
#define __ENNEMI066_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

#include "Ennemi068.h"
#include "Ennemi069.h"

class Map;

class Ennemi066 : public Ennemi {
    public :
        Ennemi066(int x, int y, Map* map);
        ~Ennemi066();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();
        bool isResetable();

        int getDown();

    protected :

        void giveItem(int x, int y);

    private :

        int anim;
        int animMax;
        int vanim;

        WImage* image;

        Chrono chrono;

        BoundingBox box;

        Ennemi068* tentaculeHG;
        Ennemi068* tentaculeBG;
        Ennemi069* tentaculeHD;
        Ennemi069* tentaculeBD;

        int animTentacules;
        int steps[4];

        void wakeUp();
        void wakeUp(int nb);
        void setSteps(int value);
};

#endif  // Ennemi066.h
