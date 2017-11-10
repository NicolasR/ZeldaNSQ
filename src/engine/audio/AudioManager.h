/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include "../common/Common.h"

#include "AudioConfiguration.h"

class AudioManager {
    public :
        static AudioManager* getInstance();

        void init();

        void setConfiguration(AudioConfiguration* conf);

        void playMusic(int i);
        void stopMusic();
        void replayMusic();
        void playSound(int i, int chl = -1);

        int isPlaying(int chl);
        void stopSound(int chl);

        int getVolumeMusic();
        int getVolumeSound();

        void setVolumeMusic(int v);
        void setVolumeSound(int v);

        void close();

    private :
        AudioManager();
        ~AudioManager();
        static AudioManager instance;

        WMusic* current;
        int music;
        int volumeMusic;
        int volumeSound;
        AudioConfiguration* config;
        int previous_volume;
        int previous_volson;
        bool playing;
        bool SOUND;
        int previousMusic;
};

#endif  // AudioManager.h
