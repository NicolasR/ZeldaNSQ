/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "../../../engine/util/tree/Node.h"

#include "../CommonGame.h"

class Drawable : public Node {
    public :
        Drawable();
        virtual ~Drawable();

        virtual void draw(int offsetX, int offsetY) = 0;

        int compareTo(Listable* other);

        virtual int getDown(); // return y + h
};

#endif  // Drawable.h


