#include "CircularTransition.h"

#include "../window/WindowManager.h"

#include <SDL/SDL_gfxPrimitives.h>

CircularTransition::CircularTransition() {
    rect = SDL_CreateRGBSurface(SDL_HWSURFACE, GAME_SIZE_W, GAME_SIZE_H, 32, 0, 0, 0, 0);
    reset();
}

CircularTransition::~CircularTransition() {
    SDL_FreeSurface(rect);
}

void CircularTransition::loop() {
    if (step == 1) {
        radius -= 10;
        if (radius == 0) {
            step = 2;
        }
    } else if (step == 3) {
        radius += 10;
        if (radius == 320) {
            step = 4;
        }
    }
    SDL_FillRect(rect, NULL, SDL_MapRGB(rect->format, 0, 0, 0));
    if (radius > 0) {
        filledEllipseRGBA(rect, x, y, radius, radius*2/3, 0, 0, 255, 255);
        SDL_SetColorKey(rect,SDL_SRCCOLORKEY, SDL_MapRGB(rect->format,0,0,255));
    }
}

void CircularTransition::draw() {
    WindowManager::getInstance()->draw(rect);
}

bool CircularTransition::isRunning() {
    return run;
}

void CircularTransition::start(int i, int j) {
    run = true;
    step = 1;
    x = i;
    y = j;
}

bool CircularTransition::isWaiting() {
    return (step == 2);
}

void CircularTransition::restart(int i, int j) {
    step = 3;
    radius = 0;
    x = i;
    y = j;
}

bool CircularTransition::isAchieved() {
    return (step == 4);
}

void CircularTransition::reset() {
    run = false;
    step = 0;
    radius = 320;
    filledEllipseRGBA(rect, 160, 120, radius, radius*2/3, 0, 0, 255, 255);
    SDL_SetColorKey(rect,SDL_SRCCOLORKEY, SDL_MapRGB(rect->format,0,0,255));
}
