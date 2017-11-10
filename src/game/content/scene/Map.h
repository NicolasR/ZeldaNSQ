/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAP_H__
#define __MAP_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Metronome.h"
#include "../../../engine/util/tree/Quadtree.h"

#include "../CommonGame.h"

#include "../types/Effect.h"
#include "../types/Ennemi.h"
#include "../types/Item.h"
#include "../types/Projectile.h"
#include "../types/Object.h"
#include "../types/Portable.h"

#include "MapInitializer.h"

#include "../ennemis/Ennemi153.h"

#define NB_IMAGES           4

class Map {
    public :
        Map(int level, Metronome* metronome);
        ~Map();

        void launch();
        void reset();
        void load();

        void launchRoom();
        BoundingBox* getBounds();
        void resetRoom();

        void loop();
        void draw(List* toDraw, BoundingBox* box, int dstX = 0, int dstY = 0);

        int getW();
        int getH();

        int getId();

        short getMur(int x, int y); // returns mur value from coordinatess
        short getSol(int x, int y); // returns sol value from coordinatess

        void setSol(int x, int y, short s);
        void setSol(int x, int y, short s, Collision c);
        void setSol(int x, int y, int w, int h, short s, Collision c);

        void addEffect(Effect* effect);
        void addEnnemi(Ennemi* ennemi);
        void addItem(Item* item);
        void addProjectile(Projectile* projectile);
        void addObject(Object* object);

        void removeObject(Object* object);

        bool checkCollisions(BoundingBox* box, Collisionable* object, bool checkEnnemis, bool ignoreNotIdle, bool withAvoid, bool onlyBox,
                             bool checkObjects = true, int safeX = 0, int safeY = 0, int safeW = -1, int safeH = -1);

        bool testDegat(BoundingBox* box, Direction dir, int force, TypeAttack type, TypeEffect effect, bool onEnnemis = true);

        bool testTransition(int x, int y);
        bool testInnerTransition(int x, int y);
        bool testTeleport(int x, int y);

        void testAnim(int x, int y, Direction dir);
        void testAnimRoom();

        void ouvrePorteMaison(int x, int y);

        bool testGemmeForce(int x, int y);
        int nbGemmeForce(); // 0 if ok, 1 if missing on close map, 2 if missing on the current map
        int nbGemmeForce(int mapId);

        void activateInter(int x, int y, bool sound = true);
        bool desactivateInter(int x, int y, bool sound = true);
        void activateOeil(bool on);
        void activateAllInter();
        void allumeTorche(int x, int y);

        void pickUpItems(BoundingBox* box);
        Item* getItem(BoundingBox* box);
        void killItems(BoundingBox* box);
        void killEnnemis(BoundingBox* box);
        Portable* pickUpObject(BoundingBox* box);
        void pushObject(BoundingBox* box, Direction dir);

        bool tryToTalk(BoundingBox* box, Direction dir);

        void permuteBlocs(int step); // 0 = orange up - blue down

        int nbPouleInBox();
        int nbEnnemis();

        void ouvrePorte(int x, int y, int type);
        void fermePorte(int x, int y, int type);

        void removeStele();

        void songPlayed(int id);

        // game map 34
        void closeChests(int nb, int id, bool gemme);
        void openChests(int x, int y);
        bool areChestsClosed();

        // game map 43
        void startCibles();
        void endCibles();
        bool isGameStarted();
        int getGameScore();
        void updateScore(int total);

        int getEtage();

        void addCoffre(int x, int y, TypeItem type, int id);

        void syncJarre(int x, int y);

        void startAnimOniLink();
        Ennemi153* getFinalBoss();

    private :

        void init();

        void loadFromFile();

        void drawSol(int srcX, int srcY, int srcW, int srcH, int dstX, int dstY);
        void drawAir(int srcX, int srcY, int srcW, int srcH, int dstX, int dstY);

        int getMotif(int value);

        Metronome* metronome;
        int map;
        int w; // size
        int h;

        int music;

        int stones; // for the puzzle in the desert
        int inters; // for red switchs
        bool eau1Up; // for map 61
        bool eau2Up; // for map 61

        short sol[MAX_CASE_X][MAX_CASE_Y];
        short air[MAX_CASE_X][MAX_CASE_Y];
        short mur[MAX_CASE_X * 2][MAX_CASE_Y * 2];

        WImage* images[NB_IMAGES];

        Quadtree* effects;
        Quadtree* ennemis;
        Quadtree* items;
        Quadtree* projectiles;
        Quadtree* objects;

        MapInitializer* initializer;

        BoundingBox bounds; // room limits

        // game map 43
        int gameScore;

        int oldMetronomeValue;

};

#endif  // Map.h
