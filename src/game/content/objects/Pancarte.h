/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PANCARTE_H__
#define __PANCARTE_H__

#include "../types/Portable.h"
#include "../scene/Map.h"

class Pancarte : public Portable {
    public :
        Pancarte(int x, int y, int texte, Map* map);
        ~Pancarte();

        void portLoop();
        void draw(int offsetX, int offsetY);

        void impact();

        bool action(Direction dir);

        void onLift();

        bool isResetable();
        void reset();

    private :

        int texte;
        int x0;
        int y0;
        Map* map;
};

#endif  // Pancarte.h
