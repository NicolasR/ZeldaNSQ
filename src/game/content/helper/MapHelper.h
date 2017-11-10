/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAPHELPER_H__
#define __MAPHELPER_H__

#include "ItemRandomizer.h"

enum TypeDons{TD_HERBE_HAUTE, TD_BUISSON_VERT, TD_BUISSON_ROSE, TD_BUISSON_OCRE, TD_BUISSON_NEIGE, TD_NB_MAX};

class MapHelper {
    public :
        static MapHelper* getInstance();

        ItemRandomizer* getItemRandomizer(TypeDons type);

    private :
        MapHelper();
        ~MapHelper();
        static MapHelper instance;

        ItemRandomizer herbeHaute;
        ItemRandomizer buissonVert;
        ItemRandomizer buissonRose;
        ItemRandomizer buissonOcre;
        ItemRandomizer buissonNeige;
};

#endif  // MapHelper.h
