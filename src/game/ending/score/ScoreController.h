/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __SCORECONTROLLER_H__
#define __SCORECONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "Score.h"

class ScoreController {
    public :
        ScoreController();
        ~ScoreController();

        void launch();

        void handleEvents(Event* event);
        void loop();
        void draw();

    private :
        Score score;
};

#endif  // ScoreController.h
