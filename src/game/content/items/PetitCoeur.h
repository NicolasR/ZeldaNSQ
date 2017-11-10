/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PETITCOEUR_H__
#define __PETITCOEUR_H__

#include "../types/Item.h"

class PetitCoeur : public Item {
    public :
        PetitCoeur(int x, int y);
        ~PetitCoeur();

        void loop();
        void draw(int offsetX, int offsetY);

        void action();

};

#endif  // PetitCoeur.h
