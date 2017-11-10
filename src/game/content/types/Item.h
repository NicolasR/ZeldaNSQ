/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ITEM_H__
#define __ITEM_H__

#include "../../../engine/common/Common.h"

#include "../CommonGame.h"

#include "Collisionable.h"
#include "Loopable.h"

#include "../link/Link.h"

class Item : public Collisionable, public Loopable {
    public :
        Item();
        virtual ~Item() = 0;

        virtual void loop();
        virtual void draw(int offsetX, int offsetY) = 0;

        int getX();
        int getY();

        void setX(int i);
        void setY(int j);

        BoundingBox* getBoundingBox();

        bool isReady();
        bool isPersistent();
        virtual bool isPickable();

        void reset();
        bool isResetable();

        virtual void action() = 0;

    protected :
        Link* getLink();

        int x;
        int y;

        int anim;
        int animMax;
        int vanim;

        bool ready;
        bool persistent;

        WImage* image;

        Chrono chrono;

        BoundingBox box;

    private :
        Link* link;

};

#endif  // Item.h
