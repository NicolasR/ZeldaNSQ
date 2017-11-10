/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CIBLE_H__
#define __CIBLE_H__

#include "../types/Ennemi.h"

class Cible : public Ennemi {
    public :
        Cible(int type, int delay);
        ~Cible();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

        int getX();
        int getY();

        void underAttack(Direction dir, int force, TypeAttack type, TypeEffect effect, BoundingBox* box);

        void start();
        void init();

        void reset();

    private :

        int type;
        int delay;
        int delayOnBegin;
        int nbTours;

        WImage* image;
        BoundingBox box;

};

#endif  // Cible.h
