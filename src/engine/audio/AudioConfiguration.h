/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __AUDIOCONFIGURATION_H__
#define __AUDIOCONFIGURATION_H__

#include "../common/Common.h"
#include "../resources/WMusic.h"
#include "../resources/WSound.h"

class AudioConfiguration {
    public :
        AudioConfiguration();
        virtual ~AudioConfiguration() = 0;

        virtual string getMusicName(int id) = 0;
        virtual WSound* getSound(int id) = 0;

        virtual void loadSounds() = 0;

        virtual int getNbSounds() = 0;
};

#endif  // AudioConfiguration.h
