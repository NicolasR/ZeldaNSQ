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

#ifdef __vita__
#include <psp2/power.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
int _newlib_heap_size_user = 192 * 1024 * 1024;
#endif

int main(int argc, char** argv) {
#ifdef __vita__
    sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND);
    scePowerSetArmClockFrequency(444);

    sceIoMkdir("ux0:data/znsq", 0777);
    sceIoMkdir("ux0:data/znsq/config", 0777);
    sceIoMkdir("ux0:data/znsq/saves", 0777);
#endif
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
