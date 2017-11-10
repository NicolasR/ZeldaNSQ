/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __JARRE_H__
#define __JARRE_H__

#include "../types/Portable.h"
#include "../scene/Map.h"

class Jarre : public Portable {
    public :
        Jarre(int x, int y, int id, TypeItem item, Map* map);
        ~Jarre();

        void portLoop();
        void draw(int offsetX, int offsetY);

        void impact();

        void onLift();

        bool isResetable();
        void reset();

        void removeItem();

    private :

        int type;
        int x0;
        int y0;
        TypeItem item;
        Map* map;
};

#endif  // Jarre.h
