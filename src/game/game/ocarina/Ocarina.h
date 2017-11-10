/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OCARINA_H__
#define __OCARINA_H__

#include "../../../config/Action.h"

class Ocarina {
    public :
        Ocarina();
        ~Ocarina();

        void handleActions(Action* action);
        void loop();

        void start();
        void stop();

        void setSong(int id);

    private :

        int id;
};

#endif  // Ocarina.h
