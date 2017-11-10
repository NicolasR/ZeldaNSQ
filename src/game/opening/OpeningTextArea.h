/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OPENINGTEXTAREA_H__
#define __OPENINGTEXTAREA_H__

#include "../../engine/common/Common.h"
#include "../../engine/window/Event.h"
#include "../../engine/util/time/Chrono.h"
#include "../../engine/texts/Text.h"

#include "../content/CommonGame.h"

#include "../../config/Action.h"

class OpeningTextArea {
    public :
        OpeningTextArea();
        ~OpeningTextArea();

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

        Text* text;

        int id;
};

#endif  // OpeningTextArea.h
