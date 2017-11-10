/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __EVENT_H__
#define __EVENT_H__

#include <SDL/SDL.h>

enum Keys {kA, kZ, kE, kR, kT, kY, kU, kI, kO, kP, kQ, kS, kD, kF, kG, kH, kJ,
    kK, kL, kM, kW, kX, kC, kV, kB, kN, kComma, kEscape, kTab, kShift, kCtrl,
    kAlt, kSpace, kReturn, k0, k1, k2, k3, k4, k5, k6, k7, k8, k9, QUIT, RESIZE,
    kUp, kDown, kLeft, kRight, QUIT_FORCED, kF1, NB_KEYS};

class Event {
    public :
        Event();
        ~Event();

        void update(Uint8* keys);

        bool isDown(Keys key);
        bool isPushed(Keys key);

        void setEvent(Keys key, bool b);
    private :
        bool current[NB_KEYS];
        bool past[NB_KEYS];
};

#endif  // Event.h
