/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __RUBISROUGE_H__
#define __RUBISROUGE_H__

#include "../types/Item.h"

class RubisRouge : public Item {
    public :
        RubisRouge(int x, int y);
        ~RubisRouge();

        void loop();
        void draw(int offsetX, int offsetY);

        void action();

};

#endif  // RubisRouge.h
