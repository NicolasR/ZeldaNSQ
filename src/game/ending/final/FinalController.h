/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __FINALCONTROLLER_H__
#define __FINALCONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "Final.h"

class FinalController {
    public :
        FinalController();
        ~FinalController();

        void launch();

        void handleEvents(Event* event);
        void loop();
        void draw();

    private :
        Final final;
};

#endif  // FinalController.h
