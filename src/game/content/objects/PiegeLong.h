/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PIEGELONG_H__
#define __PIEGELONG_H__

#include "../types/Object.h"

#include "../../../engine/util/time/Chrono.h"

class PiegeLong : public Object {
    public :
        PiegeLong(int x, int y, Direction dir);
        ~PiegeLong();

        void loop();

        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

        bool isResetable();
        void reset();

    private :

        void moveY(int dy);

        int anim;
        int animMax;
        int vanim;

        Chrono chrono;

        Direction direction;

        int x0;
        int y0;
        int y1;

        int force;
};

#endif  // PiegeLong.h
