/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TEXTCONFIGURATION_H__
#define __TEXTCONFIGURATION_H__

#include "../common/Common.h"
#include "../resources/WImage.h"

class TextConfiguration {
    public :
        TextConfiguration();
        virtual ~TextConfiguration() = 0;

        virtual string getVariableValue(int textId, int varId) = 0;
        virtual string getCommonValue(string tag) = 0;

        int getWSize();
        int getHSize();
        int getWSpace();
        string getTextfile();
        WImage* getImage();

    protected :
        void setWSize(int i);
        void setHSize(int i);
        void setWSpace(int i);
        void setTextfile(string s);
        void setImage(WImage* im);

    private :
        int wSpace;
        int wSize;
        int hSize;
        WImage* image;
        string textfile;
};

#endif  // TextConfiguration.h
