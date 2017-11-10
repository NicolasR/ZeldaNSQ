/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

/**
    this class describes a bounding box
*/
class BoundingBox {
    public :
        BoundingBox();
        BoundingBox(int a, int b, int c, int d);
        ~BoundingBox();
        void setX(int i);
        void setY(int i);
        void setW(int i);
        void setH(int i);
        int getX();
        int getY();
        int getW();
        int getH();
        bool intersect(BoundingBox* other);
        bool hasPoint(int i, int j);

    private :
        int x;
        int y;
        int w;
        int h;
};

#endif  // BoundingBox.h
