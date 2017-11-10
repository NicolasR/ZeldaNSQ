/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI153_H__
#define __ENNEMI153_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi153 : public Ennemi {
    public :
        Ennemi153(int x, int y);
        ~Ennemi153();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        void reset();

        bool finalForm();
        void stop();
        void ulti();

    protected :

        void giveItem(int x, int y);

    private :

        void moveX(int dx);
        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;

        int anim2;
        int animMax2;
        int vanim2;

        int step;
        int cooldown;

        int debuffUlti;

        WImage* image;

        Chrono chrono;
        Chrono chrono2;

        BoundingBox box;
};

#endif  // Ennemi153.h
