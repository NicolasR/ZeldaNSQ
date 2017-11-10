/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __HELP_H__
#define __HELP_H__

#include "../../../engine/resources/WImage.h"
#include "../../../engine/renderer/Surface.h"
#include "../../../config/Action.h"


class Help {
    public :
        Help();
        ~Help();

        void init();

        void handleActions(Action* action);
        void draw();

    private :

        void refresh();

        void cadre(int x, int y, int w, int h, Surface* surf);

        int page;

        WImage* image;
        Surface* surface;
        Surface* surfaceTmp;
};

#endif  // Help.h
