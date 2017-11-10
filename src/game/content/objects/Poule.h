/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __POULE_H__
#define __POULE_H__

#include "../types/Portable.h"

#include "../../../engine/util/time/Chrono.h"

class Poule : public Portable {
    public :
        Poule(int x, int y);
        ~Poule();

        void portLoop();
        void draw(int offsetX, int offsetY);

        void impact();

        void onLift();

        void stopBeforeUp();

        bool isCollision(Collision c);

        BoundingBox* getBoundingBox();

        BoundingBox* getSecondBoundingBox();

    private :

        void moveX(int dx);
        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;

        Chrono chrono;

        int stop;

        BoundingBox box2;
};

#endif  // Poule.h
