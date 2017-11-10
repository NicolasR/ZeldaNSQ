#include "Primes.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../MainController.h"

Primes::Primes() : current(0), running(false), success(false), image(0) {
}

Primes::~Primes() {
    ResourceManager::getInstance()->free(image);
}

void Primes::init(int crt) {
    maxTime = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 180 : 300;
    current = crt;
    ResourceManager::getInstance()->free(image);
    ostringstream os;
    os << current;
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi" + os.str() + ".png", true);
}

void Primes::reset() {
    current = 0;
    running = false;
    success = false;
}

void Primes::draw(int x, int y) {
    int srcX = 0;
    int srcY = 0;
    int srcW = 0;
    int srcH = 0;

    switch (current) {
        case 12 : srcW = 18; srcH = 17; break;
        case 14 : srcW = 41; srcH = 16; break;
        case 15 : srcX = 25; srcY = 5; srcW = 22; srcH = 26; break;
        case 18 : srcX = 2; srcW = 12; srcH = 14; break;
        case 23 : srcW = 26; srcH = 17; break;
        case 42 : srcW = 26; srcH = 19; break;
        case 53 : srcX = 1; srcW = 12; srcH = 15; break;
    }

    WindowManager::getInstance()->draw(image, srcX, srcY, srcW, srcH, x - srcW / 2, y);
}

void Primes::start() {
    running = true;
    success = false;
    chrono.reset();
}

void Primes::stop(bool successful) {
    running = false;
    success = successful;
}

bool Primes::isRunning() {
    return running;
}

bool Primes::isSuccessful() {
    return success;
}

int Primes::getTime() {
    int time = ((maxTime*1000) - chrono.getElapsedTime())/1000;
    if (time <= -1) {
        stop(false);
    }
    return time;
}

int Primes::getCurrent() {
    return current;
}
