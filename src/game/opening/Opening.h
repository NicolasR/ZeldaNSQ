/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OPENING_H__
#define __OPENING_H__

#include "../../engine/common/Common.h"
#include "../../engine/window/Event.h"
#include "../../engine/resources/WImage.h"
#include "../../engine/util/time/Metronome.h"

class Opening {
    public :
        Opening();
        ~Opening();

        void init();

        void loop();
        void draw();

        void nextStep();

    private :

        WImage* imageFond1;
        WImage* imageFond2;
        WImage* imageIntro[6];

        Metronome* metronome;

        int etape;
};

#endif  // Opening.h
