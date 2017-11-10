/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TEXTCONTROLLER_H__
#define __TEXTCONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "TextArea.h"

class TextController {
    public :
        TextController();
        ~TextController();

        void launch();

        void setTextId(int textId);

        void handleActions(Action* action);
        void loop();
        void draw();

    private :

        TextArea text;
};

#endif  // TextController.h
