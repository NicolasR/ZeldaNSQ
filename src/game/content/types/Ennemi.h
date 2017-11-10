/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENNEMI_H__
#define __ENNEMI_H__

#include "../../../engine/common/Common.h"

#include "../CommonGame.h"

#include "Collisionable.h"
#include "Loopable.h"

#include "../link/Link.h"

#include "../helper/ItemRandomizer.h"

class Ennemi : public Collisionable, public Loopable {
    public :
        Ennemi();
        virtual ~Ennemi() = 0;

        void loop();
        virtual void draw(int offsetX, int offsetY) = 0;

        virtual void underAttack(Direction dir, int force, TypeAttack type, TypeEffect effect, BoundingBox* box);

        virtual int getX() = 0;
        virtual int getY() = 0;

        virtual BoundingBox* getBoundingBox() = 0;

        bool isIdle();

        virtual void reset() = 0;
        virtual bool isResetable();

    protected :

        virtual BoundingBox* getBoundingBoxForDamage();

        virtual void ennLoop() = 0;

        virtual bool hasEffect(TypeAttack type, TypeEffect effect, Direction dir);

        virtual void giveItem(int x, int y);

        Link* getLink();

        bool testDegatOnLink(BoundingBox* box, Direction dir, int force, TypeAttack type, TypeEffect effect);

        void move(int dx, int dy, bool ignoreIdle = false);

        void moveX(int dx);

        void moveY(int dy);

        void jumpBack(Direction dir);

        int x;
        int y;

        int life;
        int maxLife;
        int invul;
        int recul;
        int vitesseRecul;

        int maxDist;
        Direction direction;

        int startX;
        int startY;
        Direction startDir;

        bool idle;

        ItemRandomizer items;

        int gel;
        int maxGel;

        bool isBoss;
        bool stunnable;

        bool expert;
        int forceEnn;

    private :

        void computeProjection();

        Link* link;

        int toucheX;
        int toucheY;
        int reculRestant;
};

#endif  // Ennemi.h
