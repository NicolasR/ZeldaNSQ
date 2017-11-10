/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __STATUE_H__
#define __STATUE_H__

#include "../types/Poussable.h"

class Statue : public Poussable {
    public :
        Statue(int x, int y);
        ~Statue();

        void loop();
        void draw(int offsetX, int offsetY);

        void pousse(Direction d, int v = 1);

        bool isResetable();
        void reset();

    private :

        int x0;
        int y0;
};

#endif  // Statue.h
