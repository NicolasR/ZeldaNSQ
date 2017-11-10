/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI146_H__
#define __ENNEMI146_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi146 : public Ennemi {
    public :
        Ennemi146(int x, int y);
        ~Ennemi146();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        void reset();
        bool isResetable();

    protected :

        void giveItem(int x, int y);

    private :

        void snipe();
        void multi();

        void moveX(int dx);
        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;

        int step;
        int cooldown;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi146.h
