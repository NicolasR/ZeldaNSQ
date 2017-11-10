/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "../../engine/common/Common.h"
#include "../../engine/resources/WImage.h"

#include <SDL/SDL.h>

class Surface {
    public :
        Surface(int w, int h, bool alpha = false);
        ~Surface();

        void put(WImage* im, int dstX, int dstY, int srcX = 0, int srcY = 0, int srcW = 0, int srcH = 0);

        WImage* toImage();

    private :

        WImage* image;
};

#endif  // Surface.h
