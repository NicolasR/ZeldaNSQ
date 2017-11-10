/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __STELE_H__
#define __STELE_H__

#include "../types/Object.h"

class Stele : public Object {
    public :
        Stele(int x, int y, int texte, bool broken);
        ~Stele();

        void draw(int offsetX, int offsetY);
        bool action(Direction dir);

        void explose();

        int getDown();

    private :

        int getDynamicText();

        int texte;
        bool broken;
};

#endif  // Stele.h
