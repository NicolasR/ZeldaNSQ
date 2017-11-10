/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __LISTABLE_H__
#define __LISTABLE_H__

class Listable {
    public :
        Listable();
        virtual ~Listable() = 0;
        virtual int compareTo(Listable* other) = 0; // > 0 if this > other

        //void add(Listable* obj);
        //bool remove(Listable* obj);
        //void merge(Listable* obj);

        //Listable* getNext();
        //void setNext(Listable* obj);

        //void cleanNext();

    //private :

        //Listable* next;
};

#endif  // Listable.h



