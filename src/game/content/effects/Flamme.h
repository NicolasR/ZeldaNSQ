/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __FLAMME_H__
#define __FLAMME_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../CommonGame.h"

#include "../types/Effect.h"

class Flamme : public Effect {
    public :
        Flamme(int x, int y, Direction direction);
        ~Flamme();

        void loop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        void slide(int dx, int dy);

    private :

        int x;
        int y;
        Direction direction;

        int anim;
        int animMax;
        int vanim;

        BoundingBox box;

        WImage* image;

        Chrono chrono;
};

#endif  // Flamme.h
