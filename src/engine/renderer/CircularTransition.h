/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CIRCULARTRANSITION_H__
#define __CIRCULARTRANSITION_H__

#include "../common/Common.h"

#include <SDL/SDL.h>

class CircularTransition {
    public :
        CircularTransition();
        ~CircularTransition();

        void loop();
        void draw();

        bool isRunning();

        void start(int x, int y);

        bool isWaiting();

        void restart(int x, int y);

        bool isAchieved();

        void reset();

    private :

        SDL_Surface* rect;
        bool run;
        int step;
        int radius;
        int x;
        int y;
};

#endif  // CircularTransition.h
