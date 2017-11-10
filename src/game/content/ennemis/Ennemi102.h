/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI102_H__
#define __ENNEMI102_H__

#include "../../../engine/resources/WImage.h"

#include "../types/Ennemi.h"

#include "../effects/FlammeCirculaire.h"

class Map;

class Ennemi102 : public Ennemi {
    public :
        Ennemi102(int x, int y);
        ~Ennemi102();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        void reset();
        bool isResetable();

        void allume();

    private :

        void snipe();
        void eteint();

        void moveX(int dx);
        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;

        WImage* image;

        Chrono chrono;

        BoundingBox box;

        int cooldown;
        int nbTorches;
        bool visible;
};

#endif  // Ennemi102.h
