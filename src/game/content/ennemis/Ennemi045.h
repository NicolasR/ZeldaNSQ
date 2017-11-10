/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI045_H__
#define __ENNEMI045_H__

#include "../../../engine/resources/WImage.h"

#include "../types/Ennemi.h"

class Map;

class Ennemi045 : public Ennemi {
    public :
        Ennemi045(int x, int y);
        ~Ennemi045();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        void reset();
        bool isResetable();

    protected :

        BoundingBox* getBoundingBoxForDamage();
        void giveItem(int x, int y);

    private :

        void snipe();

        void moveX(int dx);
        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;
        int mask;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
        BoundingBox box2;

        int cooldown;
};

#endif  // Ennemi045.h
