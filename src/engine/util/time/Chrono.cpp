#include "Chrono.h"

Chrono::Chrono() {
}

Chrono::~Chrono() {
}

void Chrono::reset() {
    lastAnimTime = SDL_GetTicks();
}

int Chrono::getElapsedTime() {
    return SDL_GetTicks() - lastAnimTime;
}
