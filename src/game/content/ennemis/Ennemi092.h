/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI092_H__
#define __ENNEMI092_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi092 : public Ennemi {
    public :
        Ennemi092(int x, int y);
        ~Ennemi092();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();
        bool isResetable();

    protected :

        void giveItem(int x, int y);

    private :

        void snipe();

        void moveX(int dx);

        int anim;
        int animMax;
        int vanim;

        int step;
        int cooldown;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi092.h
