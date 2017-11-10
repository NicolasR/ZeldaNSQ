/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __NAVI_H__
#define __NAVI_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../../../engine/util/time/Chrono.h"

#include "../CommonGame.h"

#include "../../Save.h"

#include "../../../config/Action.h"

#include "../types/Drawable.h"

class Navi : public Drawable {
    public :
        Navi(Save* save, int x, int y, Direction direction);
        ~Navi();

        void saveData(Save* save);

        void loop();

        void draw(int offsetX, int offsetY);

        void handleAction(Action* action);

        int getDown();

        int getX();
        int getY();

        void setX(int x);
        void setY(int y);

        void reset(int i, int j, Direction dir);

        bool isBoxOnLink();
        void setBoxOnLink(bool b);

        void setFollowLink(bool b);
        void setDirection(Direction dir);

        int getMagic();
        int getMaxMagic();
        int getLvlHeal();
        int getLvlAtt();
        int getLvlDef();
        void setLvlHeal(int lvl);
        void setLvlAtt(int lvl);
        void setLvlDef(int lvl);

        int getBonusForce();
        int getTmpForce();
        int getMaxTmpForce();

        int getBonusDef();
        int getTmpDef();
        int getMaxTmpDef();

        void setMagic(int i);
        void setMaxMagic(int i);

    private :

        int x;
        int y;

        int magic;
        int maxMagic;

        int lvlHeal;
        int lvlAtt;
        int lvlDef;

        int bonusForce;
        int tmpForce;
        int maxTmpForce;

        int bonusDef;
        int tmpDef;
        int maxTmpDef;

        int regen;

        int anim;
        int animMax;
        int vanim;

        bool boxOnLink;
        bool sayLook;
        bool followLink;

        Direction direction;

        WImage* image;

        Chrono chrono;
};

#endif  // Navi.h
