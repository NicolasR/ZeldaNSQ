/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "engine/window/WindowManager.h"
#include "engine/audio/AudioManager.h"

#include "config/ConfigurationManager.h"

#include "game/MainController.h"

#include "game/content/helper/ItemHelper.h"

int main(int argc, char** argv) {
    if (argc && argv); //pour éviter un warning.....

    std::srand(std::time(NULL));

    if (WindowManager::getInstance()->createWindow("Zelda Navi's Quest", "data/images/logos/navi.ico") < 0) {
        exit(-1);
    }

    ConfigurationManager::getInstance()->init("config/system.dat", "config/keys.dat");

    MainController* controller = MainController::getInstance();

    controller->init();

    controller->setInternalStep(LOGO);

    Event* event;

    while(WindowManager::getInstance()->isOpened()) {

        event = WindowManager::getInstance()->getEvent();

        controller->control(event);

        if (event->isPushed(QUIT) || event->isPushed(kEscape) || event->isPushed(QUIT_FORCED)) {

            WindowManager::getInstance()->close();
            continue;
        }

        WindowManager::getInstance()->display();
    }

    ItemHelper::getInstance()->close();

    controller->close();

    ConfigurationManager::getInstance()->close();
    AudioManager::getInstance()->close();

    WindowManager::getInstance()->exit();

    return 0;
}
