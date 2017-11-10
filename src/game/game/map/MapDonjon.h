/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAPDONJON_H__
#define __MAPDONJON_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/resources/WImage.h"
#include "../../../engine/util/time/Chrono.h"
#include "../../../engine/renderer/Surface.h"

#include "../../../config/Action.h"

#include "../../content/CommonGame.h"

class MapDonjon {
    public :
        MapDonjon();
        ~MapDonjon();

        void handleActions(Action* action);
        void loop();
        void draw();

        void launch();

    private :

        int getEtageMin();
        int getEtageMax();
        int getEtageBoss();

        bool bossIsAlive();

        void loadImages();
        void prepareMap();

        int getWidth();
        int getHeight();
        int getOffsetX();
        int getOffsetY();

        int anim;
        int animMax;
        int vanim;

        bool started;

        int xLink;
        int yLink;

        int mapId;
        int previousMapId;

        int etage;

        bool wasFrench;

        int offsetSkin;

        WImage* imageMap;
        WImage* imageLevel;
        WImage* imageLevel2;
        WImage* imageLink;
        WImage* imageBoussole;
        Surface* surface;
        Surface* surface2;
        WImage* imageEtages[5];
        WImage* imageEtagesN[5];

        Chrono chrono;
};

#endif  // MapDonjon.h
