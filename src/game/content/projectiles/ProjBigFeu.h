/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJBIGFEU_H__
#define __PROJBIGFEU_H__

#include "../types/Projectile.h"

class ProjBigFeu : public Projectile {
    public :
        ProjBigFeu(int x, int y, double dx, double dy);
        ~ProjBigFeu();

        void projLoop();
        void draw(int offsetX, int offsetY);

        BoundingBox* getBoundingBox();

        int getX();
        int getY();
        int getDown();

        void renvoie(Direction dir);

    private :

        int speed;
        double dx;
        double dy;
        int force;
        double longX;
        double longY;
        bool retour;
};

#endif  // ProjBigFeu.h
