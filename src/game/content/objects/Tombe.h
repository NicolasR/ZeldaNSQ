/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TOMBE_H__
#define __TOMBE_H__

#include "../types/Poussable.h"

class Tombe : public Poussable {
    public :
        Tombe(int x, int y);
        ~Tombe();

        void loop();
        void draw(int offsetX, int offsetY);

        void pousse(Direction d, int v = 1);
    private :
        bool moved;
};

#endif  // Tombe.h
