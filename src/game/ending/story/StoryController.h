/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __STORYCONTROLLER_H__
#define __STORYCONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "StoryTextArea.h"

class StoryController {
    public :
        StoryController();
        ~StoryController();

        void launch();

        void handleEvents(Event* event);
        void loop();
        void draw();

    private :
        StoryTextArea story;
};

#endif  // StoryController.h
