/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OCARINACONTROLLER_H__
#define __OCARINACONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "Ocarina.h"

class OcarinaController {
    public :
        OcarinaController();
        ~OcarinaController();

        void launch();

        void setSong(int id);

        void handleActions(Action* action);
        void loop();

    private :

        Ocarina ocarina;
};

#endif  // OcarinaController.h
