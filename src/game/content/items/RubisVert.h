/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __RUBISVERT_H__
#define __RUBISVERT_H__

#include "../types/Item.h"

class RubisVert : public Item {
    public :
        RubisVert(int x, int y);
        ~RubisVert();

        void loop();
        void draw(int offsetX, int offsetY);

        void action();

};

#endif  // RubisVert.h
