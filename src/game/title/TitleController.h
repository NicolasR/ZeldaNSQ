/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TITLECONTROLLER_H__
#define __TITLECONTROLLER_H__

#include "../../engine/common/Common.h"
#include "../../engine/window/Event.h"

#include "Title.h"

class TitleController {
    public :
        TitleController();
        ~TitleController();

        void launch();

        void handleEvents(Event* event);
        void loop();
        void draw();

    private :
        Title title;
};

#endif  // TitleController.h
