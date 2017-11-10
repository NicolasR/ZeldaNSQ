/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJBAGUETTEFEU_H__
#define __PROJBAGUETTEFEU_H__

#include "../types/Projectile.h"

class ProjBaguetteFeu : public Projectile {
    public :
        ProjBaguetteFeu(int x, int y, Direction dir);
        ~ProjBaguetteFeu();

        void projLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

    private :

        int speed;
        int dx;
        int dy;
        int level;
        int force;
};

#endif  // ProjBaguetteFeu.h
