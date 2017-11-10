/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GAMECONTROLLER_H__
#define __GAMECONTROLLER_H__

#include "../../engine/common/Common.h"
#include "../../engine/window/Event.h"
#include "../../engine/renderer/SimpleTransition.h"


#include "scene/SceneController.h"
#include "menu/GameMenuController.h"
#include "troc/GameTrocController.h"
#include "textes/TextController.h"
#include "transitionMaps/TransitionMapsController.h"
#include "transitionRooms/TransitionRoomsController.h"
#include "teleport/TeleportController.h"
#include "ocarina/OcarinaController.h"
#include "map/MapController.h"
#include "help/HelpController.h"

#include "../Save.h"

enum GameStep {GAME_MAIN, GAME_MENU, GAME_TROC, GAME_TEXT, GAME_SWITCH_MAP, GAME_SWITCH_ROOM, GAME_TELEPORT, GAME_OCARINA, GAME_MAP, GAME_HELP};

class GameController {
    public :
        GameController();
        ~GameController();

        void launch();

        void handleEvents(Event* event);
        void loop();
        void draw();

        void setSave(Save* sv);

        void saveData(bool endGame = false);

        void setStep(GameStep newStep);

        SceneController* getSceneController();
        GameMenuController* getGameMenuController();
        GameTrocController* getGameTrocController();
        TextController* getTextController();
        TeleportController* getTeleportController();
        OcarinaController* getOcarinaController();
        MapController* getMapController();

        void displayText(int id);
        void displayMap(int mapId, bool withTp = false);
        void hideMap();

        int getTime();
        Save* getSave();

        bool isTransitionRoom();
        bool isTeleport();

    private :
        SceneController scene;
        GameMenuController menu;
        GameTrocController troc;
        TextController text;
        TransitionMapsController transitionMaps;
        TransitionRoomsController transitionRooms;
        TeleportController teleport;
        OcarinaController ocarina;
        MapController map;
        HelpController help;

        GameStep step;
        GameStep nextStep;

        SimpleTransition transition;

        Save* save;

        Chrono chrono;

        bool needLoad;

        int timeOffset;
};

#endif  // GameController.h
