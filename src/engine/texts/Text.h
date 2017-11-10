/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TEXT_H__
#define __TEXT_H__

#include "../common/Common.h"

#include "../renderer/Surface.h"

class Text {
    public :
        Text(string txt);
        ~Text();

        void display(int x, int y, int l = -1);     // display on a line
        void displayOnSurface(int x, int y, Surface* surface, int l = -1);     // display on a line
        void displayBox(int x, int y, int l = -1);  // display on a box

        void setBox(int w, int h);
        bool hasNext();
        void next();

        int getWBox();
        int getHBox();
        int getLength();
        int getLengthInBox();
        string getText();
        char charAtInBox(int i);

    private :
        void computeLength();
        void cutBox(); // cut text to inBox and outBox
        int wordSize(string txt, unsigned int i); // return size of a word from i

        void displayInternal(int x, int y, int l, Surface* surface);     // display on a line

        int wBox;
        int hBox;
        int length;
        int lengthInBox;
        string text;
        string inBox;
        string outBox;

};

#endif  // Text.h
