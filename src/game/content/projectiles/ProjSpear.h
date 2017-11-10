/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJSPEAR_H__
#define __PROJSPEAR_H__

#include "../types/Projectile.h"

class ProjSpear : public Projectile {
    public :
        ProjSpear(int x, int y, Direction dir);
        ~ProjSpear();

        void projLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

    private :

        int speed;
        int dx;
        int dy;
        int force;
        int dist;

        bool wholeSpear;
};

#endif  // ProjSpear.h
