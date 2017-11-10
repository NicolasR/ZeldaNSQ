/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI062_H__
#define __ENNEMI062_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi062 : public Ennemi {
    public :
        Ennemi062(int x, int y);
        ~Ennemi062();

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

        int step;
        int beforeRegen;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi062.h
