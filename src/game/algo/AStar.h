/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ASTAR_H__
#define __ASTAR_H__

#include "../../engine/common/Common.h"

#include "../../engine/util/geometry/BoundingBox.h"

#include "../content/types/Collisionable.h"

#include <map>

struct node {
    int cost_g;
    int cost_h;
    int cost_f;
    std::pair<int,int> parent;
};

typedef map<pair<int,int>, node> l_node;

class AStar {
    public :
        static AStar* getInstance();

        pair<int, int> resolvePath(Collisionable* object, int dstX, int dstY, Direction direction, int lvl = 50);

    private :
        AStar();
        ~AStar();
        static AStar instance;

        int simpleDistance(int x1, int y1, int x2, int y2);
        pair <int, int> retrieveBestNode(l_node* list);
        void addNodes(l_node* opened, l_node* closed, pair <int, int> current, node nd,
                      int dx, int dy, int dstX, int dstY, Collisionable* object,BoundingBox* box, Direction direction, bool begin);
};

#endif  // AStar.h
