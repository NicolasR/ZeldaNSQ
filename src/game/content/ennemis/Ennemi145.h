/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI145_H__
#define __ENNEMI145_H__

#include "../../../engine/resources/WImage.h"

#include "../types/Ennemi.h"

class Map;

class Ennemi145 : public Ennemi {
    public :
        Ennemi145(int x, int y);
        ~Ennemi145();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();
        bool isResetable();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

    private :

        void snipe();

        int step;

        WImage* image;

        Chrono chrono;

        BoundingBox box;

        int cooldown;
};

#endif  // Ennemi145.h
