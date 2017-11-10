/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __HUD_H__
#define __HUD_H__

#include "../../../engine/resources/WImage.h"

#include "../link/Status.h"
#include "../link/Inventory.h"
#include "../link/Navi.h"


class Hud {
    public :
        Hud(Status* status, Inventory* inventory, Navi* navi, bool isFrench);
        ~Hud();

        void loop();
        void draw();

        void setBossLife(int life, int maxLife);
        void reset();

        void displayEtage();

    private :

        void drawLife();
        void drawMagic();
        void drawItems();
        void drawObject();
        void drawSpells();
        void drawBonus();
        void drawBoss();
        void drawTime();
        void drawEtage();

        void drawNombre(int val, int max, int x, int y, int nb);
        void drawJaugeMultiStyle(int x, int y, int current, int max);

        Status* status;
        Inventory* inventory;
        Navi* navi;
        bool french;
        int animNavi;
        int animBoss;
        int bossLife;
        int bossMaxLife;
        bool removeBoss;
        int etage;
        int displayEtg;

        WImage* image;
        WImage* chiffres;
        WImage* level;

};

#endif  // Hud.h
