#include "Timer.h"

Timer::Timer() {
}

Timer::~Timer() {
}

void Timer::reset() {
    lastAnimTime = SDL_GetTicks();
}

int Timer::getElapsedTime() {
    return SDL_GetTicks() - lastAnimTime;
}
