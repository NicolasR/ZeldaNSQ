/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GEMMEFORCE_H__
#define __GEMMEFORCE_H__

#include "../types/Item.h"

class GemmeForce : public Item {
    public :
        GemmeForce(int x, int y, int id, bool withAnim);
        ~GemmeForce();

        void loop();
        void draw(int offsetX, int offsetY);

        void action();

    private :

        int id;
};

#endif  // GemmeForce.h
