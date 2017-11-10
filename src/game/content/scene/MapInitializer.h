/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAPINITIALIZER_H__
#define __MAPINITIALIZER_H__

#include "../types/Ennemi.h"
#include "../types/Item.h"
#include "../types/Object.h"

class Map;

class MapInitializer {
    public :
        MapInitializer(Map* map);
        ~MapInitializer();

        void init();

    private :

        Map* map;

        void addEnnemi(Ennemi* ennemi);
        void addItem(Item* item);
        void addObject(Object* object);
        void ouvrePorte(int x, int y, int type);

};

#endif  // MapInitializer.h
