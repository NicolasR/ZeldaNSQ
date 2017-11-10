/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI151_H__
#define __ENNEMI151_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi151 : public Ennemi {
    public :
        Ennemi151(int x, int y);
        ~Ennemi151();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();

    private :

        int anim;
        int animMax;
        int vanim;

        int anim2;
        int animMax2;
        int vanim2;

        WImage* image;

        Chrono chrono;
        Chrono chrono2;

        BoundingBox box;
};

#endif  // Ennemi151.h
