/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI001_H__
#define __ENNEMI001_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi001 : public Ennemi {
    public :
        Ennemi001(int x, int y);
        ~Ennemi001();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

    private :

        int anim;
        int animMax;
        int vanim;

        int cooldown;
        bool jump;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi001.h
