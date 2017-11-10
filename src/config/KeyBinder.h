/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __KEYBINDER_H__
#define __KEYBINDER_H__

#include "../engine/common/Common.h"
#include "../engine/window/Event.h"

#include "Action.h"

class KeyBinder {
    public :
        static KeyBinder* getInstance();

        Action* bind(Event* event);

        void load(string filename); // load key configuration
        void save(string filename);

    private :
        KeyBinder();
        ~KeyBinder();
        static KeyBinder instance;

        Action action;
        Keys binding[NB_ACTIONS];
};

#endif  // KeyBinder.h
