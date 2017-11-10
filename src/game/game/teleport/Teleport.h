/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TELEPORT_H__
#define __TELEPORT_H__

#include "../../content/CommonGame.h"
#include "../../content/scene/Map.h"

#include "../../../engine/renderer/CircularTransition.h"

class Teleport {
    public :
        Teleport();
        ~Teleport();

        void loop();
        void draw();

        void init();

        void setTeleport(int map, int x, int y, Direction dir, bool instant, bool reload, bool etage = false);

    private :

        void mid();
        void end();

        int dstMap;
        int dstX;
        int dstY;
        Direction direction;
        bool instant;
        bool reload;
        bool etage;

        CircularTransition transition;

};

#endif  // Teleport.h
