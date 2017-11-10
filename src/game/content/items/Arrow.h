/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ARROW_H__
#define __ARROW_H__

#include "../types/Item.h"

class Arrow : public Item {
    public :
        Arrow(int x, int y);
        ~Arrow();

        void loop();
        void draw(int offsetX, int offsetY);

        void action();

};

#endif  // Arrow.h
