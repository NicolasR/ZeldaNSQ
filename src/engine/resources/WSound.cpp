#include "WSound.h"

WSound::WSound(string filename) {
#ifdef __PSP2__
    name = "app0:" + filename;
#else
    name = filename;
#endif
    sound = Mix_LoadWAV(name.c_str());
}

WSound::~WSound() {
    Mix_FreeChunk(sound);
}

Mix_Chunk* WSound::getSound() {
    return sound;
}
