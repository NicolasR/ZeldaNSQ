/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI091_H__
#define __ENNEMI091_H__

#include "../../../engine/resources/WImage.h"

#include "../types/Ennemi.h"

class Map;

class Ennemi091 : public Ennemi {
    public :
        Ennemi091(int x, int y, Map* map);
        ~Ennemi091();

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

        void moveX(int dx);
        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;

        int animDispel;

        WImage* image;

        Chrono chrono;

        BoundingBox box;

        int cooldown;

        Map* map;
};

#endif  // Ennemi091.h
