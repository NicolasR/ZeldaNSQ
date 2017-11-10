/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __RUBISBLEU_H__
#define __RUBISBLEU_H__

#include "../types/Item.h"

class RubisBleu : public Item {
    public :
        RubisBleu(int x, int y);
        ~RubisBleu();

        void loop();
        void draw(int offsetX, int offsetY);

        void action();

};

#endif  // RubisBleu.h
