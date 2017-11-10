/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJARCMAGIQUE_H__
#define __PROJARCMAGIQUE_H__

#include "../types/Projectile.h"

class ProjArcMagique : public Projectile {
    public :
        ProjArcMagique(int x, int y, Direction dir);
        ~ProjArcMagique();

        void projLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

        void renvoie(Direction dir);

    private :

        int speed;
        int dx;
        int dy;
        int level;
        int force;
        bool retour;
};

#endif  // ProjArcMagique.h
