/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __AUDIOOPTIONSCONTROLLER_H__
#define __AUDIOOPTIONSCONTROLLER_H__

#include "../../../../engine/common/Common.h"
#include "../../../../engine/window/Event.h"

#include "AudioOptions.h"

class AudioOptionsController {
    public :
        AudioOptionsController();
        ~AudioOptionsController();

        void launch();

        void handleEvents(Event* event);
        void draw();

    private :

        AudioOptions audio;
};

#endif  // AudioOptionsController.h
