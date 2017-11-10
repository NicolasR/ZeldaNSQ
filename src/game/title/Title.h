/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TITLE_H__
#define __TITLE_H__

#include "../../engine/common/Common.h"
#include "../../engine/window/Event.h"
#include "../../engine/resources/WImage.h"
#include "../../engine/texts/Text.h"
#include "../../engine/util/time/Metronome.h"

class Title {
    public :
        Title();
        ~Title();

        void init();

        void handleEvents(Event* event);
        void loop();
        void draw();

    private :

        int animCiel;
        int animMaxCiel;
        int vanimCiel;
        Chrono chronoCiel;

        int anim;
        int animMax;
        int vanim;
        Chrono chrono;

        WImage* images[44];
        WImage* navi[23];
        WImage* ciel;
        WImage* logo;
        Text* text;
        int textSize;
        bool ready;
        int alpha;

        Metronome* metronomeText;
};

#endif  // Title.h
