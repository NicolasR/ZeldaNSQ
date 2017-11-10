/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI068_H__
#define __ENNEMI068_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../types/Ennemi.h"

class Ennemi068 : public Ennemi {
    public :
        Ennemi068(int x, int y);
        ~Ennemi068();

        void ennLoop();
        void draw(int offsetX, int offsetY);

        int getX();
        int getY();

        BoundingBox* getBoundingBox();

        bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        void reset();
        bool isResetable();

        void endOfLife();

        void setStep(int step);

        int getDown();

    private :

        int anim;

        WImage* image;

        BoundingBox box;
};

#endif  // Ennemi068.h
