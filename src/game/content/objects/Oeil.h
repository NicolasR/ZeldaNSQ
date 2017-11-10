/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OEIL_H__
#define __OEIL_H__

#include "../types/Object.h"
#include "../scene/Map.h"

class Oeil : public Object {
    public :
        Oeil(int x, int y, Direction dir, Map* map);
        ~Oeil();

        void loop();
        void draw(int offsetX, int offsetY);

        bool isCollision(Collision c);

    private :

        Direction direction;
        bool actif;
        Map* map;
};

#endif  // Oeil.h
