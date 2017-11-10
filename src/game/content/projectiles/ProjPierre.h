/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJPIERRE_H__
#define __PROJPIERRE_H__

#include "../types/Projectile.h"

class ProjPierre : public Projectile {
    public :
        ProjPierre(int x, int y, double dx, double dy);
        ~ProjPierre();

        void projLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

        int getX();
        int getY();
        int getDown();

    private :

        int speed;
        double dx;
        double dy;
        int force;
        double longX;
        double longY;
};

#endif  // ProjPierre.h
