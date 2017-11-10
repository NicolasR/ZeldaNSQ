/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GAMEMENUCONTROLLER_H__
#define __GAMEMENUCONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "Menu.h"

class GameMenuController {
    public :
        GameMenuController();
        ~GameMenuController();

        void init();

        void launch();

        void handleActions(Action* action);
        void loop();
        void draw();

        Menu* getMenu();

    private :
        Menu menu;
};

#endif  // GameMenuController.h
