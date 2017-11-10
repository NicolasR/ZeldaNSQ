/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJARROW_H__
#define __PROJARROW_H__

#include "../types/Projectile.h"

class ProjArrow : public Projectile {
    public :
        ProjArrow(int x, int y, Direction dir, int type);
        ~ProjArrow();

        void projLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

    private :

        bool moving;
        int speed;
        int dx;
        int dy;
        int level;
        int force;
};

#endif  // ProjArrow.h
