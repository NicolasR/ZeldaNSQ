/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __LOGO_H__
#define __LOGO_H__

#include "../../engine/common/Common.h"
#include "../../engine/window/Event.h"
#include "../../engine/resources/WImage.h"
#include "../../engine/util/time/Chrono.h"

class Logo {
    public :
        Logo();
        ~Logo();

        void init();

        void handleEvents(Event* event);
        void loop();
        void draw();

    private :

        int anim;
        int animMax;
        int vanim;

        bool first;
        bool wasFrench;

        WImage* image;
        WImage* imageLogo;
        Chrono chrono;
};

#endif  // Logo.h
