/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAGIEGRAND_H__
#define __MAGIEGRAND_H__

#include "../types/Item.h"

class MagieGrand : public Item {
    public :
        MagieGrand(int x, int y);
        ~MagieGrand();

        void loop();
        void draw(int offsetX, int offsetY);

        void action();

};

#endif  // MagieGrand.h
