/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __COEUR_H__
#define __COEUR_H__

#include "../types/Item.h"

class Coeur : public Item {
    public :
        Coeur(int x, int y, int id, bool withAnim);
        ~Coeur();

        void loop();
        void draw(int offsetX, int offsetY);

        void action();

        bool isPickable();

    private :

        int id;
};

#endif  // Coeur.h
