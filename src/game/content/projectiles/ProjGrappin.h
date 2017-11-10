/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJGRAPPIN_H__
#define __PROJGRAPPIN_H__

#include "../types/Projectile.h"

class Item;

class ProjGrappin : public Projectile {
    public :
        ProjGrappin(int x, int y, Direction dir);
        ~ProjGrappin();

        void projLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

        void slide(int dx, int dy);

    private :

        int level;
        int step;
        bool aggripe;
        int currentLength;
        int maxLength;

        int speed;
        int dx;
        int dy;

        Item* item;

        int dstLink;
};

#endif  // ProjGrappin.h
