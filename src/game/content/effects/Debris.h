/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __DEBRIS_H__
#define __DEBRIS_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../CommonGame.h"

#include "../types/Effect.h"

class Debris : public Effect {
    public :
        Debris(int x, int y, Direction direction, int force, bool onLink = false, bool degat = true, bool top = false);
        ~Debris();

        void loop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        int getDown();

    private :

        int x;
        int y;

        int anim;
        int animMax;
        int vanim;

        Direction direction;
        int force;
        bool onLink;
        bool degat;
        bool top;

        WImage* image;

        Chrono chrono;
};

#endif  // Debris.h
