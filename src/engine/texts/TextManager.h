/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TEXTMANAGER_H__
#define __TEXTMANAGER_H__

#include "../common/Common.h"

#include "../renderer/Surface.h"

#include "TextConfiguration.h"
#include "Text.h"

#include <vector>

class TextManager {
    public :
        static TextManager* getInstance();

        void setConfiguration(TextConfiguration* conf);

        int getWSpace();
        int getWSize();
        int getHSize();

        Text* getText(int id);
        WImage* getImage();

        void drawLetter(char c, int i, int j, int style = 0);
        void drawLetterOnSurface(char c, int i, int j, Surface* surface, int style = 0);

        void reloadTexts();

    private :
        TextManager();
        ~TextManager();
        static TextManager instance;

        void loadTextsFromFile(string filename);

        void drawLetterInternal(char c, int i, int j, int style, Surface* surface = 0);

        TextConfiguration* config;
        vector<string> texts;
};

#endif  // TextManager.h
