/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAPCONTROLLER_H__
#define __MAPCONTROLLER_H__

#include "../../../engine/common/Common.h"

#include "../../../config/Action.h"
#include "MapOverworld.h"
#include "MapOcarina.h"
#include "MapDonjon.h"

enum MapStep {MAP_OVERWORLD, MAP_OCARINA, MAP_DONJON};

class MapController {
    public :
        MapController();
        ~MapController();

        void launch();

        void handleActions(Action* action);
        void loop();
        void draw();

        void setStep(int mapId, bool withTp = false);

    private :

        MapOverworld overworld;
        MapOcarina ocarina;
        MapDonjon donjon;

        MapStep step;
};

#endif  // MapController.h
