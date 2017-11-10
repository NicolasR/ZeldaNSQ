/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI148_H__
#define __ENNEMI148_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi148 : public Ennemi {
    public :
        Ennemi148(int x, int y);
        ~Ennemi148();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        void reset();
        bool isResetable();

    private :

        void moveX(int dx);
        void moveY(int dy);

        void jump(Direction d);

        int anim;
        int animMax;
        int vanim;

        int step;
        int cooldown;
        int nbJump;
        int toJump;
        Direction dirJump;

        WImage* image;

        Chrono chrono;

        BoundingBox box;
};

#endif  // Ennemi148.h
