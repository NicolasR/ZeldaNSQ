#include "WindowManager.h"

#ifdef __PSP2__
#define BTN_TRIANGLE 0
#define BTN_CIRCLE 1
#define BTN_CROSS 2
#define BTN_SQUARE 3
#define BTN_LTRIGGER 4
#define BTN_RTRIGGER 5
#define BTN_DOWN 6
#define BTN_LEFT 7
#define BTN_UP 8
#define BTN_RIGHT 9
#define BTN_SELECT 10
#define BTN_START 11
#define LSTICK 0
#define RSTICK 1
#define STICK_UP 0
#define STICK_DOWN 1
#define STICK_LEFT 2
#define STICK_RIGHT 3
#endif

WindowManager WindowManager::instance=WindowManager();

WindowManager::WindowManager() : isFullScreen(FULLSCREEN), event(0), window(0), windowTmp(0), open(false) {
}

WindowManager::~WindowManager() {
    delete event;
}

WindowManager* WindowManager::getInstance() {
    return &instance;
}

int WindowManager::createWindow(string title, string iconName) {
    if (window == 0) {
        if(SDL_Init(SDL_INIT_VIDEO) == -1) {
            printf("Could not load SDL : %s\n", SDL_GetError());
            return -1;
        }
#ifdef __PSP2__
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
        joystick = SDL_JoystickOpen(0);
#endif
        atexit(SDL_Quit);

#ifndef __PSP2__
        SDL_WM_SetCaption(title.c_str(), NULL);
        SDL_Surface* icon = SDL_LoadBMP(iconName.c_str());
        SDL_SetColorKey(icon, SDL_SRCCOLORKEY, SDL_MapRGB(icon->format,0,0,0));
        SDL_WM_SetIcon(icon, NULL);

        SDL_ShowCursor(isFullScreen ? SDL_DISABLE : SDL_ENABLE);
#else
        SDL_ShowCursor(SDL_DISABLE);
#endif

#ifdef __PSP2__
        int sh = 544;
        int sw = (float)320*((float)sh/(float)240);
        int x = (960 - sw)/2;

        window = SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
        SDL_SetVideoModeScaling(x, 0, sw, sh);

        windowTmp = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
#else
        window = SDL_SetVideoMode(640, 480, 32, isFullScreen ? SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN : SDL_HWSURFACE|SDL_DOUBLEBUF);
        windowTmp = SDL_CreateRGBSurface(SDL_HWSURFACE, 640, 480, 32, 0, 0, 0, 0);
#endif

        //if (isFullScreen) SDL_Delay(2000); // wait the window is displayed

        event = new Event();
        lastAnimTime = SDL_GetTicks();
        open = true;
    }
    return 0;
}

bool WindowManager::isOpened() {
    return open;
}

#ifdef __PSP2__
int WindowManager::stickPosition(int stick, int direction) {
    int axis;
    int axisValue;
    if (stick == LSTICK && (direction == STICK_LEFT || direction == STICK_RIGHT)) axis = 0; // Left stick, horizontal axis
    if (stick == LSTICK && (direction == STICK_UP || direction == STICK_DOWN)) axis = 1; // Left stick, vertical axis
    if (stick == RSTICK && (direction == STICK_LEFT || direction == STICK_RIGHT)) axis = 2; // Right stick, horizontal axis
    if (stick == RSTICK && (direction == STICK_UP || direction == STICK_DOWN)) axis = 3; // Right stick, vertical axis

    axisValue = SDL_JoystickGetAxis(joystick, axis); // range is -32768 to 32767
    return ((axisValue < -31000 && direction == STICK_LEFT)
      || (axisValue > 31000 && direction == STICK_RIGHT)
      || (axisValue < -31000 && direction == STICK_UP)
      || (axisValue > 31000 && direction == STICK_DOWN));
}
#endif

Event* WindowManager::getEvent() {


    Uint8* keys = SDL_GetKeyState(NULL);

#ifdef __PSP2__
    // Bindings can be found in src/config/KeyBinder.cpp
    keys[SDLK_UP] = stickPosition(LSTICK, STICK_UP) || stickPosition(RSTICK, STICK_UP);
    keys[SDLK_DOWN] = stickPosition(LSTICK, STICK_DOWN) || stickPosition(RSTICK, STICK_DOWN);
    keys[SDLK_LEFT] = stickPosition(LSTICK, STICK_LEFT) || stickPosition(RSTICK, STICK_LEFT);
    keys[SDLK_RIGHT] = stickPosition(LSTICK, STICK_RIGHT) || stickPosition(RSTICK, STICK_RIGHT);
    keys[SDLK_z] = SDL_JoystickGetButton(joystick, BTN_SQUARE); // Hit + Load
    // TODO keys[SDLK_w] = SDL_JoystickGetButton(joystick, BTN_SQUARE); // Hit 2 + Load 2
    keys[SDLK_x] = SDL_JoystickGetButton(joystick, BTN_TRIANGLE); // Action
    keys[SDLK_c] = SDL_JoystickGetButton(joystick, BTN_LTRIGGER); // Carry
    keys[SDLK_SPACE] = SDL_JoystickGetButton(joystick, BTN_CIRCLE); // Talk / Read
    keys[SDLK_LCTRL] = stickPosition(RSTICK, STICK_UP) || stickPosition(RSTICK, STICK_DOWN) ||
        stickPosition(RSTICK, STICK_LEFT) || stickPosition(RSTICK, STICK_RIGHT);
    keys[SDLK_i] = SDL_JoystickGetButton(joystick, BTN_LEFT); // Troc items
    keys[SDLK_p] = SDL_JoystickGetButton(joystick, BTN_UP); // Map
    keys[SDLK_o] = SDL_JoystickGetButton(joystick, BTN_DOWN); // Oni
    keys[SDLK_v] = SDL_JoystickGetButton(joystick, BTN_RIGHT); // Ulti
    keys[SDLK_LSHIFT] = SDL_JoystickGetButton(joystick, BTN_RTRIGGER); // Run
    keys[SDLK_RETURN] = SDL_JoystickGetButton(joystick, BTN_CROSS); // Confirm / Pass text
    keys[SDLK_ESCAPE] = SDL_JoystickGetButton(joystick, BTN_START); // Escape game
    keys[SDLK_F1] = SDL_JoystickGetButton(joystick, BTN_SELECT); // Help
#endif

    event->update(keys);

    if (event->isPushed(RESIZE)) {
        isFullScreen = ! isFullScreen;

        isFullScreen ? SDL_ShowCursor(SDL_DISABLE) : SDL_ShowCursor(SDL_ENABLE);
        window = SDL_SetVideoMode(640, 480, 32, isFullScreen ? SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN : SDL_HWSURFACE|SDL_DOUBLEBUF);

    }

    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT : event->setEvent(QUIT, true); break;
        }
    }

    return event;
}

void WindowManager::close() {
    open = false;
}

void WindowManager::display() {

#ifdef __PSP2__
    SDL_BlitSurface(windowTmp, NULL, window, NULL);
#else
    SDL_Surface* tmp = zoomSurface(windowTmp, 2, 2, 0);
    SDL_BlitSurface(tmp, NULL, window, NULL);
    SDL_FreeSurface(tmp);
#endif

    SDL_Flip(window);

    if (SDL_GetTicks() < lastAnimTime + (1000 / 60)) SDL_Delay(lastAnimTime + (1000 / 60) - SDL_GetTicks());
    lastAnimTime = SDL_GetTicks();
}

void WindowManager::draw(SDL_Surface* object) {
    SDL_BlitSurface(object, NULL, windowTmp, NULL);
}

void WindowManager::draw(WImage* image, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int alpha) {
    SDL_Rect src;
    src.x = srcX;
    src.y = srcY;
    src.w = srcW;
    src.h = srcH;

    SDL_Rect dst;
    dst.x = dstX;
    dst.y = dstY;

    SDL_BlitSurface(image->getImage(), &src, windowTmp, &dst);
}

void WindowManager::exit() {
    SDL_FreeSurface(windowTmp);

    SDL_ShowCursor(SDL_ENABLE);
    SDL_Quit();
}
