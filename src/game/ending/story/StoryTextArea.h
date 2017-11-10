/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __STORYTEXTAREA_H__
#define __STORYTEXTAREA_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"
#include "../../../engine/resources/WImage.h"
#include "../../../engine/util/time/Chrono.h"
#include "../../../engine/texts/Text.h"

#include "../../content/CommonGame.h"

#include "../../../config/Action.h"

class StoryTextArea {
    public :
        StoryTextArea();
        ~StoryTextArea();

        void handleActions(Action* action);
        void loop();
        void draw();

        void init();

        void start();
        void stop();

    private :

        void setTextId(int textId);

        bool hasLogicalNext();

        int anim;
        int animMax;
        int vanim;

        bool ready;

        Chrono chrono;

        WImage* image;

        Text* text;

        int id;
};

#endif  // StoryTextArea.h
