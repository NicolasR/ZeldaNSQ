/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI149_H__
#define __ENNEMI149_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi149 : public Ennemi {
    public :
        Ennemi149(int x, int y);
        ~Ennemi149();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();

    private :

        void placeProj();

        int anim;
        int animMax;
        int vanim;

        WImage* image;

        Chrono chrono;

        BoundingBox box;

        int cooldown;
        double angle;
        double radius;
        double projX;
        double projY;
        Direction projDir;

        int step;
};

#endif  // Ennemi149.h
