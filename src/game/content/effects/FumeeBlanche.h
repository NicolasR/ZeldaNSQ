/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __FUMEEBLANCHE_H__
#define __FUMEEBLANCHE_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../CommonGame.h"

#include "../types/Effect.h"

class FumeeBlanche : public Effect {
    public :
        FumeeBlanche(int x, int y, int step = 0);
        ~FumeeBlanche();

        void loop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

    private :

        int x;
        int y;

        int anim;
        int animMax;
        int vanim;

        WImage* image;

        Chrono chrono;
};

#endif  // FumeeBlanche.h
