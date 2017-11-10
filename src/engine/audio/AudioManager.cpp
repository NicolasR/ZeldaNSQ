#include "AudioManager.h"

AudioManager AudioManager::instance=AudioManager();

AudioManager::AudioManager() : current(0), music(0), volumeMusic(128), volumeSound(128),
    config(0), previous_volume(0), previous_volson(0), playing(false) {

    SOUND = true;
    previousMusic = -1;
}

AudioManager::~AudioManager() {

}

void AudioManager::init() {
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) == -1) SOUND = false;

    if (SOUND) {
        Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
        previous_volume = Mix_VolumeMusic(volumeMusic);
    }
}

AudioManager* AudioManager::getInstance() {
    return &instance;
}

void AudioManager::close() {
    if (SOUND) {
        if (current != 0) {
            Mix_PauseMusic();
        }
        Mix_VolumeMusic(previous_volume);
        if (current != 0) {
            Mix_HaltMusic();
            delete current;
        }

        Mix_CloseAudio();
    }
}

void AudioManager::setConfiguration(AudioConfiguration* conf) {
    if (SOUND) {
        config = conf;
        config->loadSounds();
        setVolumeSound(volumeSound);
    }
}

int AudioManager::getVolumeMusic() {
    return volumeMusic;
}

int AudioManager::getVolumeSound() {
    return volumeSound;
}

void AudioManager::setVolumeMusic(int v) {
    volumeMusic = v;
    if (SOUND) Mix_VolumeMusic(volumeMusic);
}

void AudioManager::setVolumeSound(int v) {
    volumeSound = v;
    if (SOUND) for (int i = 0; i < config->getNbSounds(); i++) Mix_VolumeChunk(config->getSound(i)->getSound(), volumeSound);
}

void AudioManager::playMusic(int i) {
    if (SOUND) {
        if (music != i || !playing) {
            music = i;
            if (current != 0) {
                Mix_HaltMusic();
                delete current;
            }
            current = new WMusic(config->getMusicName(i));
            Mix_PlayMusic(current->getMusic(),-1);
            playing = true;
        }
    }
}

void AudioManager::stopMusic() {
    if (SOUND) {
        previousMusic = music;
        Mix_HaltMusic();
        playing = false;
    }
}

void AudioManager::replayMusic() {
    if (previousMusic != -1) {
        playMusic(previousMusic);
    }
}

void AudioManager::playSound(int i, int chl) {
    if (SOUND) {
        Mix_PlayChannel(chl, config->getSound(i)->getSound(), 0);
    }
}

int AudioManager::isPlaying(int chl) {
    if (SOUND) {
        return Mix_Playing(chl);
    }
    return 0;
}

void AudioManager::stopSound(int chl) {
    if (SOUND) {
        Mix_HaltChannel(chl);
    }
}
