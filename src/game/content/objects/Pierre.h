/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PIERRE_H__
#define __PIERRE_H__

#include "../types/Portable.h"
#include "../scene/Map.h"

class Pierre : public Portable {
    public :
        Pierre(int x, int y, int id, Map* map, bool resetable = true);
        ~Pierre();

        void portLoop();
        void draw(int offsetX, int offsetY);

        void impact();

        void onLift();

        bool isResetable();
        void reset();

    private :

        int type;
        int x0;
        int y0;
        Map* map;
        bool resetable;
};

#endif  // Pierre.h
