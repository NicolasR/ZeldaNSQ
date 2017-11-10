/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __AUDIOOPTIONS_H__
#define __AUDIOOPTIONS_H__

#include "../../../../engine/window/Event.h"
#include "../../../../engine/resources/WImage.h"
#include "../../../../engine/renderer/Surface.h"


class AudioOptions {
    public :
        AudioOptions();
        ~AudioOptions();

        void init();

        void handleEvents(Event* event);
        void draw();

    private :

        void cadre(int x, int y, int w, int h, Surface* surf);

        int line;

        int volume;
        int volson;

        WImage* image;
        Surface* surface;
        WImage* niveau;
        WImage* link;
        int skin;
};

#endif  // AudioOptions.h
