/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include "../common/Common.h"
#include "Event.h"

#include "../resources/WImage.h"

#ifdef __PSP2__
#define BTN_TRIANGLE 0
#define BTN_CIRCLE 1
#define BTN_CROSS 2
#define BTN_SQUARE 3
#define BTN_LTRIGGER 4
#define BTN_RTRIGGER 5
#define BTN_DOWN 6
#define BTN_LEFT 7
#define BTN_UP 8
#define BTN_RIGHT 9
#define BTN_SELECT 10
#define BTN_START 11
#define LSTICK 0
#define RSTICK 1
#define STICK_UP 0
#define STICK_DOWN 1
#define STICK_LEFT 2
#define STICK_RIGHT 3
#endif

class WindowManager {
    public :
        static WindowManager* getInstance();
        int createWindow(string title, string icon);
        bool isOpened();
        Event* getEvent();
        void close();
        void display();

        void draw(WImage* image, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int alpha = 255);
        void draw(SDL_Surface* object);

        void exit();

    private :
        WindowManager();
        ~WindowManager();
        static WindowManager instance;

        bool isFullScreen;
        string name;
        Event* event;

        SDL_Surface* window;
        SDL_Surface* windowTmp;

        Uint32 lastAnimTime;
        bool open;

#ifdef __PSP2__
        SDL_Joystick* joystick;
        int button(int buttonId);
        int stick(int stickId, int direction);
#endif
};

#endif  // WindowManager.h
