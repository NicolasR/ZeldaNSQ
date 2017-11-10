/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "Node.h"
#include "../list/List.h"
#include "../geometry/BoundingBox.h"

/**
    this class describes a quadtree
*/
class Quadtree {
    public :
        Quadtree();
        ~Quadtree();

        void setBox(int x, int y, int w, int h);

        void add(Node* obj);
        bool remove(Node* obj);

        int size();
        int length();

        void get(BoundingBox* box, List* list);
        void getAll(List* list);

        void resetNodes(BoundingBox* bb);

        void removeDeadNodes();

    private :
        Node* root;
        BoundingBox box;
};

#endif  // Quadtree.h
