/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CHRONO_H__
#define __CHRONO_H__

#include <SDL/SDL.h>

class Chrono {
    public :
        Chrono();
        ~Chrono();

        void reset();
        int getElapsedTime();

    private :

        Uint32 lastAnimTime;
};

#endif  // Chrono.h


