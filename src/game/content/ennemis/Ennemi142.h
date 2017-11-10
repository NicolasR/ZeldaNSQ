/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI142_H__
#define __ENNEMI142_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi142 : public Ennemi {
    public :
        Ennemi142(int x, int y);
        ~Ennemi142();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        void reset();
        bool isResetable();

    protected :

        void giveItem(int x, int y);

    private :

        void jumpToLink();

        int anim;
        int animMax;
        int vanim;

        int beforeJump;
        int jump;
        int dstJumpX;
        int dstJumpY;
        double dx;
        double dy;
        double trueX;
        double trueY;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi142.h
