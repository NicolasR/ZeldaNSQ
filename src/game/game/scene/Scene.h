/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __SCENE_H__
#define __SCENE_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "../../../engine/util/time/Metronome.h"

#include "../../../engine/renderer/CircularTransition.h"

#include "../../Save.h"
#include "../../content/scene/Camera.h"
#include "../../content/scene/Map.h"
#include "../../content/scene/Hud.h"
#include "../../content/scene/AnimationInGame.h"
#include "../../content/scene/Primes.h"
#include "../../content/link/Link.h"
#include "../../content/link/Navi.h"

enum SceneStep {SCENE_MAIN, SCENE_TRANSITION_LIVE, SCENE_ONLY_LINK, SCENE_TRANSITION_BETWEEN_TWO_MAPS};

class Scene {
    public :
        Scene();
        ~Scene();

        void init(Save* sv);

        void saveData();

        void handleActions(Action* action);
        void loop();
        void draw();

        void fixCameraBounds();

        bool checkCollisions(BoundingBox* box, Collisionable* object, bool checkEnnemis, bool ignoreNotIdle = false,
                             bool withAvoid = false, bool onlyBox = false, bool checkObjects = true,
                             int safeX = 0, int safeY = 0, int safeW = -1, int safeH = -1);
        bool checkCollisionsWithLink(BoundingBox* box);

        bool testDegatOnLink(BoundingBox* box, Direction dir, int force, TypeAttack type, TypeEffect effect);
        bool testDegat(BoundingBox* box, Direction dir, int force, TypeAttack type, TypeEffect effect, bool onEnnemis = true);
        void testTransition();

        Map* getMap();
        Link* getLink();
        Navi* getNavi();
        Metronome* getMetronome();
        Camera* getCamera();
        Hud* getHud();
        AnimationInGame* getAnimationInGame();
        Primes* getPrimes();

        Avancement getAvancement();
        void setAvancement(Avancement av);

        void setStep(SceneStep s);
        void setMap(Map* map);

        bool permuteBlocs(int step); // 0 = orange up - blue down

        int getCoffre(int i, int j);
        void setCoffre(int i, int j, int val);

        int getFees(int i);
        void setFees(int i, int val);

        int getRooms(int d, int i, int j);
        void setRooms(int d, int i, int j, int val);

    private :

        SceneStep step;

        Save* save;
        Map* map;
        Camera camera;
        Metronome* metronome;
        Link* link;
        Navi* navi;
        Hud* hud;
        AnimationInGame animation;
        Primes primes;

        Avancement avancement;

        bool boxOnLink;

        CircularTransition transition;

        int coffres[NB_COFFRES][COFFRES_MAX_SIZE];
        int fees[NB_FAIRIES];

        int rooms[NB_DONJON][MAX_ROOMS_X][MAX_ROOMS_Y];


};

#endif  // Scene.h
