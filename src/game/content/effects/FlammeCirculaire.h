/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __FLAMMECIRCULAIRE_H__
#define __FLAMMECIRCULAIRE_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../CommonGame.h"

#include "../types/Effect.h"

class FlammeCirculaire : public Effect {
    public :
        FlammeCirculaire(int x, int y, Direction direction);
        ~FlammeCirculaire();

        void loop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        void move(int x, int y, Direction dir);
        void end();

    private :

        int x;
        int y;
        Direction direction;

        int anim;
        int animMax;
        int vanim;

        int force;

        BoundingBox box;

        WImage* image;

        Chrono chrono;
};

#endif  // FlammeCirculaire.h
