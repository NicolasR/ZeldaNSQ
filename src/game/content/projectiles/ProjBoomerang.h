/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJBOOMERANG_H__
#define __PROJBOOMERANG_H__

#include "../types/Projectile.h"

class Item;

class ProjBoomerang : public Projectile {
    public :
        ProjBoomerang(int x, int y, Direction dir, int type);
        ~ProjBoomerang();

        void projLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

    private :

        int step;
        int speed;
        int dx;
        int dy;
        int level;
        int force;
        int currentLength;
        int maxLength;

        Item* item;
};

#endif  // ProjBoomerang.h
