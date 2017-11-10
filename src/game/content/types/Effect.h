/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "../../../engine/common/Common.h"

#include "../../../engine/util/tree/Node.h"

#include "../CommonGame.h"

#include "Loopable.h"

class Effect : public Loopable {
    public :
        Effect();
        virtual ~Effect() = 0;

        virtual void loop() = 0;
        virtual void draw(int offsetX, int offsetY) = 0;

        virtual int getX() = 0;
        virtual int getY() = 0;
};

#endif  // Effect.h
