#include "WMusic.h"

WMusic::WMusic(string filename) {
#ifdef __vita__
    name = "app0:" + filename;
#else
    name = filename;
#endif
    music = Mix_LoadMUS(name.c_str());
}

WMusic::~WMusic() {
    Mix_FreeMusic(music);
}

Mix_Music* WMusic::getMusic() {
    return music;
}
