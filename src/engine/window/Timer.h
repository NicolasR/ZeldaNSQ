/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TIMER_H__
#define __TIMER_H__

#include <SDL/SDL.h>

class Timer {
    public :
        Timer();
        ~Timer();

        void reset();
        int getElapsedTime();

    private :

        Uint32 lastAnimTime;
};

#endif  // Timer.h
