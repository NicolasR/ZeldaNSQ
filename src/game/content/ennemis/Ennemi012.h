/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI012_H__
#define __ENNEMI012_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi012 : public Ennemi {
    public :
        Ennemi012(int x, int y);
        ~Ennemi012();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        void reset();

        bool isCollision(Collision c);
        bool isToAvoid(Collision c);

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

    protected :

        void giveItem(int x, int y);

    private :

        int anim;
        int animMax;
        int vanim;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi012.h

