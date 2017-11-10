/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __LISTE_H__
#define __LISTE_H__

#include "ListCell.h"

/**
    this class describes a sorted list
*/
class List {
    public :
        List();
        ~List();

        void add(Listable* obj);
        bool remove(Listable* obj);
        Listable* remove(int i);

        int size();
        void merge(List* other);

        Listable* get(int i);

        void iterateOnFirst();
        bool hasNext();
        Listable* getNext();

    private :
        ListCell* head;
        ListCell* current;
        int length;
};

#endif  // List.h
