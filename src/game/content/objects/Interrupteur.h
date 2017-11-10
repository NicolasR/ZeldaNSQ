/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __INTERRUPTEUR_H__
#define __INTERRUPTEUR_H__

#include "../types/Object.h"
#include "../scene/Map.h"

class Interrupteur : public Object {
    public :
        Interrupteur(int x, int y, int type, bool used, Map* map);
        ~Interrupteur();

        void loop();
        void draw(int offsetX, int offsetY);

        bool isResetable();
        void reset();

        void activate();

        void setUsed();

    private :

        int type;
        bool used;
        bool usedTmp;
        Map* map;
};

#endif  // Interrupteur.h
