/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TEXTAREA_H__
#define __TEXTAREA_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"
#include "../../../engine/resources/WImage.h"
#include "../../../engine/util/time/Chrono.h"
#include "../../../engine/texts/Text.h"

#include "../../content/CommonGame.h"

#include "../../../config/Action.h"

enum TextStep {TEXT_START, TEXT_ON, TEXT_END, TEXT_OFF};

class TextArea {
    public :
        TextArea();
        ~TextArea();

        void handleActions(Action* action);
        void loop();
        void draw();

        void start();
        void stop();

        void setTextId(int textId);

    private :

        bool hasLogicalNext();

        void drawAdditionalContent();

        TextStep step;
        int anim;
        int animMax;
        int vanim;

        bool ready;

        WImage* image;
        WImage* imageCoeur;

        Chrono chrono;

        Text* text;

        int id;
};

#endif  // TextArea.h
