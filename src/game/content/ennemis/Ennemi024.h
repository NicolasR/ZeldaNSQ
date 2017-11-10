/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI024_H__
#define __ENNEMI024_H__

#include "../../../engine/resources/WImage.h"

#include "../types/Ennemi.h"

#include "../effects/FlammeCirculaire.h"

class Map;

class Ennemi024 : public Ennemi {
    public :
        Ennemi024(int x, int y, bool water);
        ~Ennemi024();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();
        bool isResetable();

        bool isToAvoid(Collision c);

    protected :

        void giveItem(int x, int y);

    private :

        void moveX(int dx);
        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;

        WImage* image;

        Chrono chrono;

        BoundingBox box;

        int cooldown;
        bool inWater;
        int vitesse;
};

#endif  // Ennemi024.h
