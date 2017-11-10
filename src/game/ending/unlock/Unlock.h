/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __UNLOCK_H__
#define __UNLOCK_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"
#include "../../../engine/renderer/Surface.h"

class Unlock {
    public :
        Unlock();
        ~Unlock();

        void init();

        void handleEvents(Event* event);
        void draw();

    private :

        Surface* surface;
};

#endif  // Unlock.h
