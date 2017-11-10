/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __UNLOCKCONTROLLER_H__
#define __UNLOCKCONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "Unlock.h"

class UnlockController {
    public :
        UnlockController();
        ~UnlockController();

        void launch();

        void handleEvents(Event* event);
        void loop();
        void draw();

    private :
        Unlock unlock;
};

#endif  // UnlockController.h
