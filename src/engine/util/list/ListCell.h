/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __LISTCELL_H__
#define __LISTCELL_H__

#include "Listable.h"

class ListCell {
    public :
        ListCell(Listable* obj);
        ~ListCell();
        int compareTo(ListCell* other); // > 0 if this > other

        void add(ListCell* obj);
        bool remove(Listable* obj);
        void merge(ListCell* obj);

        Listable* getContent();

        ListCell* getNext();
        void setNext(ListCell* obj);

        void cleanNext();

    private :

        Listable* content;
        ListCell* next;
};

#endif  // ListCell.h



