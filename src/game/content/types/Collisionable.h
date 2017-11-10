/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __COLLISIONABLE_H__
#define __COLLISIONABLE_H__

#include "../CommonGame.h"

#include "../../../engine/util/geometry/BoundingBox.h"

class Collisionable {
    public :
        Collisionable();
        virtual ~Collisionable() = 0;

        virtual BoundingBox* getBoundingBox() = 0;
        virtual bool isCollision(Collision c);
        virtual bool isToAvoid(Collision c);
        virtual bool isIdle();

    protected :

        bool isOnIce();
        bool isOnWater();
        bool isOnGap();
};

#endif  // Collisionable.h


