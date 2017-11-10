/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PORTABLE_H__
#define __PORTABLE_H__

#include "Object.h"

class Portable : public Object {
    public :
        Portable();
        virtual ~Portable() = 0;

        void loop();

        BoundingBox* getHandableBox();

        void lance(int down, Direction d, int v);

        void lache(int down);

        bool isPosable();

        bool isCarriable();

        void drawWhenCarried(int xCenter, int yDown, int offsetX, int offsetY);

        void setDirection(Direction dir);

        int getPoids();

        virtual void onLift();

        virtual void portLoop() = 0;

        virtual bool isCollision(Collision c);

        virtual void stopBeforeUp();

    protected :

        void setPosable(bool p);

        virtual void impact();

        virtual void onContact();

        bool carried;

        bool carriable;

        bool moving;

        Direction direction;

        int dx;

        int dy;

        int air; // = height

        int poids;

        bool safeMode;

        int maxDist;

    private :

        bool isOnWater(BoundingBox* b);

        bool isOnGap(BoundingBox* b);

        bool posable;

        BoundingBox handableBox;

        int speed;

        int level;
};

#endif  // Portable.h
