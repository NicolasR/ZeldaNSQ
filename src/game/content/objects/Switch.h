/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "../types/Object.h"

class Switch : public Object {
    public :
        Switch(int x, int y);
        ~Switch();

        void loop();
        void draw(int offsetX, int offsetY);

        void underAttack(Direction dir, int force, TypeAttack type, TypeEffect effect);

        void setLeft(bool left);

    private :

        bool left;
        int invul; // 16 to 0 when activated
};

#endif  // Switch.h
