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
};

#endif  // WindowManager.h
