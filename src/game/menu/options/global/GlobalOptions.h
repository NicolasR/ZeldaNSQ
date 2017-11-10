/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GLOBALOPTIONS_H__
#define __GLOBALOPTIONS_H__

#include "../../../../engine/window/Event.h"
#include "../../../../engine/resources/WImage.h"
#include "../../../../engine/renderer/Surface.h"


class GlobalOptions {
    public :
        GlobalOptions();
        ~GlobalOptions();

        void init();

        void handleEvents(Event* event);
        void draw();

    private :

        void refresh();

        void cadre(int x, int y, int w, int h, Surface* surf);

        int getTextForSkin();

        int line;

        WImage* image;
        Surface* surface;
        Surface* surfaceTmp;
        WImage* link;
        int skin;
};

#endif  // GlobalOptions.h
