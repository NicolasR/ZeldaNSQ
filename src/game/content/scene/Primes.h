/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PRIMES_H__
#define __PRIMES_H__

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

class Primes {
    public :
        Primes();
        ~Primes();

        void init(int current);

        void draw(int x, int y);

        void start();
        void stop(bool successful);

        bool isRunning();
        bool isSuccessful();
        int getTime();
        int getCurrent();

        void reset();

    private :

        int current;
        int maxTime;

        bool running;
        bool success;

        Chrono chrono;

        WImage* image;

};

#endif  // Primes.h
