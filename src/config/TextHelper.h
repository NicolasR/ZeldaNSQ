/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TEXTHELPER_H__
#define __TEXTHELPER_H__

#include "../engine/common/Common.h"
#include "../engine/texts/TextConfiguration.h"
#include "../engine/resources/WImage.h"

class TextHelper : public TextConfiguration {
    public :
        TextHelper();
        ~TextHelper();

        void setLanguage(int i);

        string getVariableValue(int textId, int varId);
        string getCommonValue(string tag);

    private :
        WImage* image;
};

#endif  // TextHelper.h
