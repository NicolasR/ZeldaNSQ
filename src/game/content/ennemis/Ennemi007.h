/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI007_H__
#define __ENNEMI007_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi007 : public Ennemi {
    public :
        Ennemi007(int x, int y, int initHeight = 0);
        ~Ennemi007();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();
        bool isResetable();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

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

#endif  // Ennemi007.h
