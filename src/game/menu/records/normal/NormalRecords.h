/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __NORMALRECORDS_H__
#define __NORMALRECORDS_H__

#include "../../../../engine/window/Event.h"
#include "../../../../engine/resources/WImage.h"
#include "../../../../engine/renderer/Surface.h"

#include "../../../GlobalSave.h"

class NormalRecords {
    public :
        NormalRecords();
        ~NormalRecords();

        void init();

        void handleEvents(Event* event);
        void draw();

    private :

        void cadre(int x, int y, int w, int h, Surface* surf);

        int line;
        bool full;
        bool ulti;
        bool rush;

        WImage* image;
        Surface* surface;
        Surface* surfaceFull;
        Surface* surfaceUlti;
        Surface* surfaceRush;
        WImage* link;
        WImage* rang;
        int skin;

        GlobalSave* globalSave;
};

#endif  // NormalRecords.h
