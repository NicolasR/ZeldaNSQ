/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __LINK_H__
#define __LINK_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/geometry/BoundingBox.h"

#include "../../../engine/util/time/Chrono.h"

#include "../../../engine/util/time/Metronome.h"

#include "../../../engine/util/physic/SlideManager.h"

#include "../../../config/Action.h"

#include "../CommonGame.h"

#include "../../Save.h"

#include "../types/Portable.h"

#include "../projectiles/ProjGrappin.h"
#include "../effects/Flamme.h"

#include "../objects/CaisseSomaria.h"

#include "Status.h"
#include "Inventory.h"
#include "Sword.h"
#include "Shield.h"

class Link : Drawable, Collisionable {
    public :
        Link(Save* save, Metronome* metronome);
        ~Link();

        int compareTo(Listable* other);

        void saveData(Save* save);

        void handleAction(Action* action);
        void loop();
        void draw(int offsetX, int offsetY);

        void setAnimation(Animation newAnim);
        Animation getAnimation();

        BoundingBox* getBoundingBox();

        int getX();
        int getY();
        int getW();
        int getH();
        Direction getDirection();

        void setX(int i);
        void setY(int j);
        void setDirection(Direction dir);

        int getTunique();
        int getEpee();
        int getBouclier();

        Status* getStatus();
        Inventory* getInventory();

        void setTunique(int i);
        void setEpee(int i);
        void setBouclier(int i);

        bool underAttack(Direction dir, int force, TypeAttack type, TypeEffect effect);

        bool isAbleToChangeStuff();

        void trouveObjet(TypeItem type, int id = 0);

        void unlockBoomerang();

        void notifySomariaEnd();

        void revit(); // "reset" link

        int getDefense();

        int getForce();

        void startNewMap(); // when link enter on a new area
        void startNewRoom(bool newMap = false); // when link enter in a new room

        bool hasMoved();

        void setLapin(bool lapin);

        bool isByrna();

        void setOniLink(bool onilink, bool revit = false, bool withAnim = true);

        void setFatalAnim();
        bool isFatalAnim();

    private :

        void loadFromSave(Save* save);

        void findDirection(bool up, bool down, bool left, bool right, int nb_dir);
        void moveX(int dx, int vitesse, int nbDir);
        void moveY(int dy, int vitesse, int nbDir);

        void playStep(int id);

        void jumpInWater();
        void computeProjection();

        void jumpOutWater();

        void fallInGap();

        bool useStuff();
        bool porteObjet();
        Portable* getPortable();
        void pousse();

        void lacheObj();
        void lanceObj();

        void hitByrna();

        void drawIdle(int dstX, int dstY);
        void drawWalk(int dstX, int dstY);
        void drawPush(int dstX, int dstY);
        void drawTouche(int dstX, int dstY);
        void drawChute(int dstX, int dstY);
        void drawFlotte(int dstX, int dstY);
        void drawCoule(int dstX, int dstY);
        void drawEpee(int dstX, int dstY);
        void drawSpin(int dstX, int dstY);
        void drawCharge(int dstX, int dstY);
        void drawDead(int dstX, int dstY);
        void drawTrouveSimple(int dstX, int dstY);
        void drawTrouveDouble(int dstX, int dstY);
        void drawArc(int dstX, int dstY);
        void drawPorte(int dstX, int dstY, int offsetX, int offsetY);
        void drawThrow(int dstX, int dstY);
        void drawSouleve(int dstX, int dstY);
        void drawGrappin(int dstX, int dstY, int offsetX, int offsetY);
        void drawBoomerang(int dstX, int dstY);
        void drawBaguette(int dstX, int dstY);
        void drawLanterne(int dstX, int dstY, int offsetX, int offsetY);
        void drawCanne(int dstX, int dstY);
        void drawFlacon(int dstX, int dstY);
        void drawShadow(int dstX, int dstY);
        void drawByrna(int dstX, int dstY, int offsetX, int offsetY);
        void drawOcarina(int dstX, int dstY);
        void drawElectric(int dstX, int dstY);

        Metronome* metronome;

        int x;
        int y;
        int w;
        int h;
        int speed;
        bool moved;

        Direction direction;
        int tunique;
        Sword* epee;
        Shield* bouclier;
        bool charging;
        int charge;

        Animation animation;
        int anim;
        int animMax;
        int vanim;

        Chrono chrono;

        WImage* imageLink;
        WImage* imageEffets;
        WImage* imageObjets;

        BoundingBox box;

        int oldx[8]; // previous x
        int oldy[8]; // previous y

        int accuPush;

        bool wasOnIce;
        SlideManager slide;

        int toucheX; // when Link is jumping
        int toucheY;

        Status* status;
        Inventory* inventory;

        // start new map
        int startX;
        int startY;
        Direction startDir;

        // start new map
        int startX2;
        int startY2;
        Direction startDir2;

        TypeItem trouve;

        Portable* porte;
        int elan;

        ProjGrappin* grappin;
        Flamme* flamme;
        CaisseSomaria* caisse;

        bool boomerang;
        bool byrna; // true if effect enabled
        int animByrna;
        Direction dirByrna;
        bool lapin;
        bool fatal;
};

#endif  // Link.h
