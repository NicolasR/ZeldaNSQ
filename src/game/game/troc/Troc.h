/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TROC_H__
#define __TROC_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"
#include "../../../engine/resources/WImage.h"
#include "../../../engine/texts/Text.h"

#include "../../content/CommonGame.h"

#include "../../../config/Action.h"

#include "../../content/link/Inventory.h"

enum GameTrocStep {MENU_TROC_START, MENU_TROC_ON, MENU_TROC_END, MENU_TROC_OFF};

class Troc {
    public :
        Troc();
        ~Troc();

        void handleActions(Action* action);
        void loop();
        void draw();

        void start();
        void stop();

    private :

        void drawTitre(int df);

        void drawObjects(int df);

        void drawCadre(int x, int y, int w, int h);

        void drawNombre(int val, int x, int y);

        GameTrocStep step;
        int anim;

        WImage* imageMenu;
        WImage* imageChiffres;

        Inventory* inventory;

        Text* text;

        int blocs;

        int offset;
};

#endif  // Troc.h
