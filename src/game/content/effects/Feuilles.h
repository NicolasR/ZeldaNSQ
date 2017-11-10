/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __FEUILLES_H__
#define __FEUILLES_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../CommonGame.h"

#include "../types/Effect.h"

class Feuilles : public Effect {
    public :
        Feuilles(int x, int y, int type);
        ~Feuilles();

        void loop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

    private :

        int x;
        int y;
        int type;

        int anim;
        int animMax;
        int vanim;

        BoundingBox box;

        WImage* image;

        Chrono chrono;
};

#endif  // Feuilles.h
