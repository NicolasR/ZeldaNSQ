/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __SHIELD_H__
#define __SHIELD_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/resources/WImage.h"

#include "../CommonGame.h"

class Shield {
    public :
        Shield(int i, bool onilink = false);
        ~Shield();

        void draw(int offsetX, int offsetY, Direction dir, Animation anim, int step, int charge);

        int getLevel();

    private :

        void draw(int dstX, int dstY, Direction dir);

        int level;

        bool onilink;

        WImage* image;
};

#endif  // Shield.h
