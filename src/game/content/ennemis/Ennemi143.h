/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI143_H__
#define __ENNEMI143_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi143 : public Ennemi {
    public :
        Ennemi143(int x, int y);
        ~Ennemi143();

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
        void snipe();

        int anim;
        int animMax;
        int vanim;

        int beforeJump;
        int jump;
        int beforeShoot;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi143.h
