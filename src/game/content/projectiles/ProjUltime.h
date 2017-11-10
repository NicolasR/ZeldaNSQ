/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJULTIME_H__
#define __PROJULTIME_H__

#include "../types/Projectile.h"

#include "../link/Link.h"

class ProjUltime : public Projectile {
    public :
        ProjUltime(int x, int y, Link* link);
        ~ProjUltime();

        void projLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

        int getX();
        int getY();
        int getDown();

        void renvoie(Direction dir);

    private :

        void computeDxDy();
        void computeDir();

        int speed;
        double dx;
        double dy;
        int force;
        int parcouru;
        double histX[16];
        double histY[16];
        double longX;
        double longY;
        bool retour;

        Link* link;
};

#endif  // ProjUltime.h
