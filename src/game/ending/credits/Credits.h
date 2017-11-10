/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CREDITS_H__
#define __CREDITS_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"
#include "../../../engine/renderer/Surface.h"

class Credits {
    public :
        Credits();
        ~Credits();

        void init();

        void handleEvents(Event* event);
        void loop();
        void draw();

    private :

        bool over;
        int step;
        int anim;

        Surface* surface;
        Surface* surface2;
        WImage* image;
};

#endif  // Credits.h
