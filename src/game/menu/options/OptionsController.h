/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OPTIONSCONTROLLER_H__
#define __OPTIONSCONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "../../../engine/renderer/SimpleTransition.h"

#include "global/GlobalOptionsController.h"
#include "audio/AudioOptionsController.h"

enum OptionsStep {OPTIONS_MAIN, OPTIONS_AUDIO};

class OptionsController {
    public :
        OptionsController();
        ~OptionsController();

        void launch();

        void handleEvents(Event* event);
        void loop();
        void draw();

        void setStep(OptionsStep newStep);

        void setInternalStep(OptionsStep newStep); // set step without transition

    private :

        OptionsStep step;
        OptionsStep nextStep;

        SimpleTransition transition;

        GlobalOptionsController global;
        AudioOptionsController audio;
};

#endif  // OptionsController.h
