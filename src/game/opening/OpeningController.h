/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OPENINGCONTROLLER_H__
#define __OPENINGCONTROLLER_H__

#include "../../engine/common/Common.h"
#include "../../engine/window/Event.h"

#include "Opening.h"
#include "OpeningTextArea.h"

class OpeningController {
    public :
        OpeningController();
        ~OpeningController();

        void launch();

        void handleEvents(Event* event);
        void loop();
        void draw();

        void nextStep();

    private :
        Opening opening;
        OpeningTextArea text;
};

#endif  // OpeningController.h
