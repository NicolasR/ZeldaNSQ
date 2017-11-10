/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CAISSE_H__
#define __CAISSE_H__

#include "../types/Poussable.h"

class Caisse : public Poussable {
    public :
        Caisse(int x, int y, int id = 0);
        ~Caisse();

        void loop();
        void draw(int offsetX, int offsetY);

        void pousse(Direction d, int v = 1);
        int getDown();

        bool isResetable();
        void reset();

    private :

        int x0;
        int y0;

        int type;

};

#endif  // Caisse.h
