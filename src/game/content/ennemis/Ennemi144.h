/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI144_H__
#define __ENNEMI144_H__

#include "../../../engine/resources/WImage.h"

#include "../types/Ennemi.h"

#include "../effects/FlammeCirculaire.h"

class Map;

class Ennemi144 : public Ennemi {
    public :
        Ennemi144(int x, int y, Map* map);
        ~Ennemi144();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();
        bool isResetable();

    protected :

        void giveItem(int x, int y);

    private :

        void setFlammes();
        void snipe();

        void moveX(int dx);
        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;

        WImage* image;

        Chrono chrono;

        BoundingBox box;

        FlammeCirculaire* flammes[7];

        int cooldown;
        double angle;
        int radius;

        Map* map;
};

#endif  // Ennemi144.h
