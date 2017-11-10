/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../CommonGame.h"

#include "Collisionable.h"
#include "Loopable.h"

class Object : public Collisionable, public Loopable {
    public :
        Object();
        virtual ~Object() = 0;

        virtual void loop();
        virtual void draw(int offsetX, int offsetY) = 0;

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        virtual BoundingBox* getSecondBoundingBox(); // can be used for colisions

        bool isAttackable();

        virtual void underAttack(Direction dir, int force, TypeAttack type, TypeEffect effect);

        virtual bool action(Direction dir);

    protected :

        WImage* image;

        int x;
        int y;

        bool attackable;

        BoundingBox box;

};

#endif  // Object.h
