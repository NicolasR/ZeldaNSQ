/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PNJ_H__
#define __PNJ_H__

#include "../types/Object.h"

#include "../../../engine/util/time/Chrono.h"

class Pnj : public Object {
    public :
        Pnj(int x, int y, int type, int texte);
        ~Pnj();

        void loop();

        void draw(int offsetX, int offsetY);

        bool action(Direction dir);

        BoundingBox* getBoundingBox();

        int getDown();

    private :

        void init();

        int getDynamicText();

        void moveX(int dx);
        void moveY(int dy);

        int type;
        int texte;

        int anim;
        int animMax;
        int vanim;

        Chrono chrono;

        int w;
        int h;

        int displayW;
        int displayH;

        bool idle;

        Direction direction;

        bool stop;
};

#endif  // Pnj.h
