/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAPOCARINA_H__
#define __MAPOCARINA_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/resources/WImage.h"

#include "../../../config/Action.h"

#include "../../content/CommonGame.h"

class MapOcarina {
    public :
        MapOcarina();
        ~MapOcarina();

        void handleActions(Action* action);
        void loop();
        void draw();

        void launch();

    private :

        bool started;
        bool teleport;

        int position;

        int offsetSkin;

        WImage* imageMap;
        WImage* imageLink;
};

#endif  // MapOcarina.h
