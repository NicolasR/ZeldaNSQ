#include "SimpleTransition.h"

#include "../window/WindowManager.h"


SimpleTransition::SimpleTransition() {
    rect = SDL_CreateRGBSurface(SDL_HWSURFACE, GAME_SIZE_W, GAME_SIZE_H, 32, 0, 0, 0, 0);
    reset();
}

SimpleTransition::~SimpleTransition() {
    SDL_FreeSurface(rect);
}

void SimpleTransition::loop() {
    if (step == 1) {
        alpha += 15;
        if (alpha == 255) {
            step = 2;
        }
        //rect.SetColor(sf::Color(0, 0, 0, alpha));
        SDL_SetAlpha(rect, SDL_SRCALPHA, alpha);
    } else if (step == 3) {
        alpha -= 15;
        if (alpha == 0) {
            step = 4;
        }
        SDL_SetAlpha(rect, SDL_SRCALPHA, alpha);
    }
}

void SimpleTransition::draw() {
    WindowManager::getInstance()->draw(rect);
}

bool SimpleTransition::isRunning() {
    return run;
}

void SimpleTransition::start() {
    run = true;
    step = 1;
}

bool SimpleTransition::isWaiting() {
    return (step == 2);
}

void SimpleTransition::restart() {
    step = 3;
    alpha = 255;
}

bool SimpleTransition::isAchieved() {
    return (step == 4);
}

void SimpleTransition::reset() {
    run = false;
    step = 0;
    alpha = 0;
}

