/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI022_H__
#define __ENNEMI022_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi022 : public Ennemi {
    public :
        Ennemi022(int x, int y, int initHeight = 0);
        ~Ennemi022();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        void reset();
        bool isResetable();

        bool isToAvoid(Collision c);

    private :

        int anim;
        int animMax;
        int vanim;
        int initHeight;
        int currentHeight;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi022.h
