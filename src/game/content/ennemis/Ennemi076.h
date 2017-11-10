/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI076_H__
#define __ENNEMI076_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi076 : public Ennemi {
    public :
        Ennemi076(int x, int y);
        ~Ennemi076();

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
        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;

        int cooldown;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi076.h
