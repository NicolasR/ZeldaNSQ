/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GLOBALRECORDS_H__
#define __GLOBALRECORDS_H__

#include "../../../../engine/window/Event.h"
#include "../../../../engine/resources/WImage.h"
#include "../../../../engine/renderer/Surface.h"

#include "../../../GlobalSave.h"

class GlobalRecords {
    public :
        GlobalRecords();
        ~GlobalRecords();

        void init();

        void handleEvents(Event* event);
        void draw();

    private :

        void cadre(int x, int y, int w, int h, Surface* surf);

        int line;
        bool tir;

        WImage* image;
        Surface* surface;
        Surface* surfaceArc;
        WImage* link;
        int skin;

        GlobalSave* globalSave;
};

#endif  // GlobalRecords.h
