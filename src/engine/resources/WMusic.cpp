#include "WMusic.h"

WMusic::WMusic(string filename) {
    name = filename;
    music = Mix_LoadMUS(name.c_str());
}

WMusic::~WMusic() {
    Mix_FreeMusic(music);
}

Mix_Music* WMusic::getMusic() {
    return music;
}
