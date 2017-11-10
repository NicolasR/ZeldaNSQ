/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PETITECLE_H__
#define __PETITECLE_H__

#include "../types/Item.h"

class PetiteCle : public Item {
    public :
        PetiteCle(int x, int y, int id);
        ~PetiteCle();

        void draw(int offsetX, int offsetY);

        void action();

    private :

        int id;

};

#endif  // PetiteCle.h
