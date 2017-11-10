/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJBAGUETTEGLACE_H__
#define __PROJBAGUETTEGLACE_H__

#include "../types/Projectile.h"

class ProjBaguetteGlace : public Projectile {
    public :
        ProjBaguetteGlace(int x, int y, Direction dir);
        ~ProjBaguetteGlace();

        void projLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

        int getX();
        int getY();

    private :

        int speed;
        int dx;
        int dy;
        int level;
        int force;
        int parcouru;
        int x0;
        int y0;
};

#endif  // ProjBaguetteGlace.h
