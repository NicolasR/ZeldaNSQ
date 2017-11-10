/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI030_H__
#define __ENNEMI030_H__

#include "../../../engine/resources/WImage.h"

#include "../types/Ennemi.h"

class Ennemi030 : public Ennemi {
    public :
        Ennemi030(int x, int y);
        ~Ennemi030();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

    private :

        int cooldown;

        WImage* image;

        BoundingBox box;
};

#endif  // Ennemi030.h
