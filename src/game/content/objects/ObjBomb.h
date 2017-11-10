/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OBJBOMB_H__
#define __OBJBOMB_H__

#include "../types/Portable.h"

#include "../../../engine/util/time/Chrono.h"

class ObjBomb : public Portable {
    public :
        ObjBomb(int x, int y);
        ~ObjBomb();

        void portLoop();
        void draw(int offsetX, int offsetY);

        void impact();

        void reset();
        bool isResetable();

    private :

        int anim;
        int animMax;
        int vanim;

        Chrono chrono;

};

#endif  // ObjBomb.h
