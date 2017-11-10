/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PIEGEMAIN_H__
#define __PIEGEMAIN_H__

#include "../types/Object.h"

class PiegeMain : public Object {
    public :
        PiegeMain(int x, int y);
        ~PiegeMain();

        void loop();

        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

        bool isResetable();
        void reset();

        int getDown();

    private :

        void moveX(int dx);
        void moveY(int dy);

        int x0;
        int y0;

        int cooldown;
        int step;
};

#endif  // PiegeMain.h
