/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI006_H__
#define __ENNEMI006_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi006 : public Ennemi {
    public :
        Ennemi006(int x, int y);
        ~Ennemi006();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        void reset();

    private :

        int anim;
        int animMax;
        int vanim;

        bool actif;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi006.h

