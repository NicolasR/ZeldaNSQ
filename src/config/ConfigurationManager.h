/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CONFIGURATIONMANAGER_H__
#define __CONFIGURATIONMANAGER_H__

#include "../engine/common/Common.h"

#include "AudioHelper.h"
#include "TextHelper.h"

class ConfigurationManager {
    public :
        static ConfigurationManager* getInstance();

        void init(string filename, string keys);

        void save();

        bool isFrench();
        string getSkinName();

        int getVolume();
        int getVolson();
        int getLang();
        int getSkin();

        void setVolume(int v);
        void setVolson(int v);
        void setLang(int l);
        void setSkin(int s);

        void close();

    private :
        ConfigurationManager();
        ~ConfigurationManager();
        static ConfigurationManager instance;

        bool haveToSave;
        string file;
        int volume;
        int volson;
        int lang;
        int skin;

        AudioHelper* audio;
        TextHelper* text;
};

#endif  // ConfigurationManager.h
