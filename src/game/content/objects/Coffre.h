/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __COFFRE_H__
#define __COFFRE_H__

#include "../types/Object.h"

class Coffre : public Object {
    public :
        Coffre(int x, int y, int type, bool opened, TypeItem object, int id = 0);
        ~Coffre();

        void draw(int offsetX, int offsetY);

        bool action(Direction dir);

        bool isOpened();
        void open();
        void close(TypeItem object, int id = 0);

        int getDown();

    private :

        int type;
        bool opened;
        TypeItem object;
        int objId;
};

#endif  // Coffre.h
