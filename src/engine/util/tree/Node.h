/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __NODE_H__
#define __NODE_H__

#include "../list/List.h"
#include "../geometry/BoundingBox.h"

/**
    this class describes a node
*/
class Node : public Listable {
    public :
        Node();
        virtual ~Node() = 0;

        void setBox(int x, int y, int w, int h);

        void add(Node* obj);
        bool remove(Node* obj);
        Node* removeSon(int id);
        void clearSons();

        int getSize();
        int getLength();
        int getSonIdWithMaxLength();

        void checkPosition();

        void get(BoundingBox* bb, List* list);

        void permuteNodes(Node* old);

        bool isAlive();
        void killNode();

        virtual bool isResetable();
        virtual void reset();

        void resetNodes(BoundingBox* bb);

        void removeDeadNodes();

        virtual int getX() = 0;
        virtual int getY() = 0;

    protected :
        void computeMaxSize();

        int width;
        int height;

        bool alive;

    private :
        void computeLength();
        void computeSize();
        void computeBox();
        int getSonFromPosition(int x, int y);

        Node* previous;
        Node* sons[4];

        int length;
        int size;

        int sonId;

        int maxWidth;
        int maxHeight;

        BoundingBox bbox;
};

#endif  // Node.h
