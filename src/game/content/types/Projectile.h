/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "Collisionable.h"
#include "Loopable.h"

class Projectile : public Collisionable, public Loopable {
    public :
        Projectile();
        virtual ~Projectile() = 0;

        void loop();
        virtual void draw(int offsetX, int offsetY) = 0;

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool isCollision(Collision c);

        virtual void renvoie(Direction dir);

    protected :

        virtual void projLoop() = 0;

        int x;
        int y;
        Direction direction;

        int anim;
        int animMax;
        int vanim;

        WImage* image;

        Chrono chrono;

        BoundingBox box;

    private :

};

#endif  // Projectile.h
