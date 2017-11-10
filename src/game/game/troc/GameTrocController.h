/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GAMETROCCONTROLLER_H__
#define __GAMETROCCONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "Troc.h"

class GameTrocController {
    public :
        GameTrocController();
        ~GameTrocController();

        void launch();

        void handleActions(Action* action);
        void loop();
        void draw();

        Troc* getTroc();

    private :
        Troc troc;
};

#endif  // GameTrocController.h
