/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TALKABLEAREA_H__
#define __TALKABLEAREA_H__

#include "../types/Object.h"

class TalkableArea : public Object {
    public :
        TalkableArea(int x, int y, int texte);
        ~TalkableArea();

        void loop();
        void draw(int offsetX, int offsetY);

        bool action(Direction dir);

    private :

        int getDynamicText();

        int texte;
};

#endif  // TalkableArea.h
