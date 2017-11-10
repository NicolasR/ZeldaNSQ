/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __SCORE_H__
#define __SCORE_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"
#include "../../../engine/renderer/Surface.h"

class Score {
    public :
        Score();
        ~Score();

        void init();

        void handleEvents(Event* event);
        void draw();

    private :

        Surface* surface;
};

#endif  // Score.h
