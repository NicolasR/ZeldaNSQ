/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI110_H__
#define __ENNEMI110_H__

#include "../../../engine/resources/WImage.h"

#include "../types/Ennemi.h"

class Map;

class Ennemi110 : public Ennemi {
    public :
        Ennemi110(int x, int y);
        ~Ennemi110();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        void reset();
        bool isResetable();

    private :

        void snipe();

        void moveX(int dx);
        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;
        int step;

        WImage* image;

        Chrono chrono;

        BoundingBox box;

        int cooldown;
};

#endif  // Ennemi110.h
