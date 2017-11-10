/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CAISSESOMARIA_H__
#define __CAISSESOMARIA_H__

#include "Caisse.h"

class CaisseSomaria : public Caisse {
    public :
        CaisseSomaria(int x, int y);
        ~CaisseSomaria();

        void draw(int offsetX, int offsetY);

        void explose();

        bool isOnGap(BoundingBox* b);
        bool isResetable();

};

#endif  // CaisseSomaria.h
