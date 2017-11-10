/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TELEPORTCONTROLLER_H__
#define __TELEPORTCONTROLLER_H__

#include "../../../engine/common/Common.h"

#include "Teleport.h"

class TeleportController {
    public :
        TeleportController();
        ~TeleportController();

        void launch();

        void loop();
        void draw();

        void setTeleport(int map, int x, int y, Direction dir, bool instant, bool reload, bool etage = false);

    private :

        Teleport teleport;
};

#endif  // TeleportController.h
