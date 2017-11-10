/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAINCONTROLLER_H__
#define __MAINCONTROLLER_H__

#include "../engine/common/Common.h"
#include "../engine/window/Event.h"
#include "../engine/renderer/SimpleTransition.h"

#include "logo/LogoController.h"
#include "title/TitleController.h"
#include "menu/MenuController.h"
#include "opening/OpeningController.h"
#include "game/GameController.h"
#include "ending/EndingController.h"

enum MainStep {LOGO, TITRE, MENU, OPENING, GAME, ENDING};

class MainController {
    public :
        static MainController* getInstance();

        void init();
        void close();

        void control(Event* event); // dispatch event, display scene, etc.
        void setStep(MainStep newStep);

        void setInternalStep(MainStep newStep); // set step without transition

        MenuController* getMenuController();
        GameController* getGameController();
        OpeningController* getOpeningController();
        EndingController* getEndingController();

    private :
        MainController();
        ~MainController();
        static MainController instance;

        void handleEvents(Event* event);
        void loop();
        void draw();


        MainStep step;
        MainStep nextStep;

        SimpleTransition transition;

        LogoController* logo;
        TitleController* title;
        MenuController* menu;
        GameController* game;
        OpeningController* opening;
        EndingController* ending;
};

#endif  // MainController.h
