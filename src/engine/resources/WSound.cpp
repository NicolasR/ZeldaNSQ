#include "WSound.h"

WSound::WSound(string filename) {
    name = filename;
    sound = Mix_LoadWAV(name.c_str());
}

WSound::~WSound() {
    Mix_FreeChunk(sound);
}

Mix_Chunk* WSound::getSound() {
    return sound;
}
