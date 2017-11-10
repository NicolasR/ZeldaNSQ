/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MENUCONTROLLER_H__
#define __MENUCONTROLLER_H__

#include "../../engine/common/Common.h"
#include "../../engine/window/Event.h"

#include "../../engine/renderer/SimpleTransition.h"

#include "main/MainMenuController.h"
#include "records/RecordsController.h"
#include "options/OptionsController.h"

enum MenuStep {MENU_MAIN, MENU_OPTIONS, MENU_RECORDS};

class MenuController {
    public :
        MenuController();
        ~MenuController();

        void launch();

        void handleEvents(Event* event);
        void loop();
        void draw();

        void setStep(MenuStep newStep);

        void setInternalStep(MenuStep newStep); // set step without transition

        RecordsController* getRecordsController();
        OptionsController* getOptionsController();

    private :

        MenuStep step;
        MenuStep nextStep;

        SimpleTransition transition;

        MainMenuController main;
        RecordsController records;
        OptionsController options;
};

#endif  // MenuController.h
