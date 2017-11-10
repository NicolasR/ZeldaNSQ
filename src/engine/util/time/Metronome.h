/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __METRONOME_H__
#define __METRONOME_H__

#include "Chrono.h"

class Metronome {
    public :
        Metronome(int max, int delay);
        ~Metronome();

        void reset();
        void loop();
        int getValue();

    private :

        int value;
        int max;
        float delay;

        Chrono chrono;
};

#endif  // Metronome.h


