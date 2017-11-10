/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAPOVERWORLD_H__
#define __MAPOVERWORLD_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/resources/WImage.h"
#include "../../../engine/util/time/Chrono.h"

#include "../../../config/Action.h"

#include "../../content/CommonGame.h"

class MapOverworld {
    public :
        MapOverworld();
        ~MapOverworld();

        void handleActions(Action* action);
        void loop();
        void draw();

        void launch();

    private :

        int getOffsetXForMap(int mapId);
        int getOffsetYForMap(int mapId);

        int anim;
        int animMax;
        int vanim;

        bool started;

        int xLink;
        int yLink;

        int offsetSkin;

        WImage* imageMap;
        WImage* imageLink;

        Chrono chrono;
};

#endif  // MapOverworld.h
