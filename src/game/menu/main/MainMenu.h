/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"
#include "../../../engine/resources/WImage.h"
#include "../../../engine/renderer/Surface.h"

#include "../../Save.h"

class MainMenu {
    public :
        MainMenu();
        ~MainMenu();

        void init();

        void handleEvents(Event* event);
        void draw();

    private :

        void cadre(int x, int y, int w, int h, Surface* surf);

        int line;
        int column;

        bool load;
        bool confirm;
        bool expert;

        int subLine;

        WImage* image;
        Surface* surface;
        Surface* surfaceSave;
        Surface* surfaceConfirm;
        Surface* surfaceExpert;
        WImage* link;
        WImage* status;
        WImage* fairy;
        WImage* medaillons;
        WImage* rangs;
        int skin;

        Save* saves[3];
};

#endif  // MainMenu.h
