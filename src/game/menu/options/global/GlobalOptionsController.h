/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GLOBALOPTIONSCONTROLLER_H__
#define __GLOBALOPTIONSCONTROLLER_H__

#include "../../../../engine/common/Common.h"
#include "../../../../engine/window/Event.h"

#include "GlobalOptions.h"

class GlobalOptionsController {
    public :
        GlobalOptionsController();
        ~GlobalOptionsController();

        void launch();

        void handleEvents(Event* event);
        void draw();

    private :

        GlobalOptions global;
};

#endif  // GlobalOptionsController.h
