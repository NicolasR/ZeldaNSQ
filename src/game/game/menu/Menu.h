/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MENU_H__
#define __MENU_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"
#include "../../../engine/resources/WImage.h"
#include "../../../engine/util/time/Metronome.h"

#include "../../content/CommonGame.h"

#include "../../../config/Action.h"

#include "../../content/link/Link.h"

enum GameMenuStep {MENU_START, MENU_ON, MENU_END, MENU_OFF};

class Menu {
    public :
        Menu();
        ~Menu();

        void init();

        void handleActions(Action* action);
        void loop();
        void draw();

        void start();
        void stop();

        Equipment getSelectedStuff();

    private :

        void drawObjects(int df);

        void drawObjectName(int df);

        void drawHearts(int df);

        void drawGemms(int df);

        void drawMedaillons(int df);

        void drawStuff(int df);

        void drawCadre(int x, int y, int w, int h);

        void drawNombre(int val, int x, int y, int nb, bool green);
        void drawNombreTricolore(int val, int x, int y, int nb, int color);

        void updateSelectedStuff();

        int i;
        int j;

        GameMenuStep step;
        int anim;

        WImage* imageMenu;
        WImage* imageCoeur;
        WImage* imageChiffres;
        WImage* imageMedaillons;
        WImage* imageStuff;
        WImage* imageStatus;

        Metronome* metronome;

        Link* link;

        int nbQuartCoeur;

        int nbGemme;

        int nbTotalGemme;

        int allGemme;

        bool displayMedaillons;

        int force;
        int defense;
        int tunique;
        int epee;
        int bouclier;
        int maxRupees;
        int coquillages;
        int hasCoquillage;
        int navi;
        int maxFleches;
        int fleches;
        int recupFleches;
        int maxBombes;
        int bombes;
        int recupBombes;
        int naviHeal;
        int naviAtt;
        int naviDef;
};

#endif  // Menu.h
