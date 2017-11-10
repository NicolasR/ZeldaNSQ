/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __HELPCONTROLLER_H__
#define __HELPCONTROLLER_H__

#include "../../../engine/common/Common.h"

#include "Help.h"

class HelpController {
    public :
        HelpController();
        ~HelpController();

        void launch();

        void handleActions(Action* action);
        void loop();
        void draw();

    private :
        Help help;
};

#endif  // HelpController.h
