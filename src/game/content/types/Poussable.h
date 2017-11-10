/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __POUSSABLE_H__
#define __POUSSABLE_H__

#include "Object.h"

class Poussable : public Object {
    public :
        Poussable();
        virtual ~Poussable() = 0;

        void loop();

        virtual void pousse(Direction d, int v = 2) = 0;

        int getPoids();

    protected :

        bool isOnWater(BoundingBox* b);

        virtual bool isOnGap(BoundingBox* b);

        bool moving;

        int dx;

        int dy;

        int poids;
};

#endif  // Poussable.h
