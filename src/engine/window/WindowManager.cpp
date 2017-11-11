#include "WindowManager.h"

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
int WindowManager::button(int buttonId) {
    return SDL_JoystickGetButton(joystick, buttonId);
}
int WindowManager::stick(int stickId, int direction) {
    int axis;
    int axisValue;
    if (stickId == LSTICK && (direction == STICK_LEFT || direction == STICK_RIGHT)) axis = 0; // Left stick, horizontal axis
    if (stickId == LSTICK && (direction == STICK_UP || direction == STICK_DOWN)) axis = 1; // Left stick, vertical axis
    if (stickId == RSTICK && (direction == STICK_LEFT || direction == STICK_RIGHT)) axis = 2; // Right stick, horizontal axis
    if (stickId == RSTICK && (direction == STICK_UP || direction == STICK_DOWN)) axis = 3; // Right stick, vertical axis

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
    int ctrl = button(BTN_LTRIGGER);
    int lup = stick(LSTICK, STICK_UP);
    int ldown = stick(LSTICK, STICK_DOWN);
    int lleft = stick(LSTICK, STICK_LEFT);
    int lright = stick(LSTICK, STICK_RIGHT);
    int rup = stick(RSTICK, STICK_UP);
    int rdown = stick(RSTICK, STICK_DOWN);
    int rleft = stick(RSTICK, STICK_LEFT);
    int rright = stick(RSTICK, STICK_RIGHT);
    int navi = ctrl | rup | rdown | rleft | rright;

    keys[SDLK_UP] = (!navi & lup) | rup;
    keys[SDLK_DOWN] = (!navi & ldown) | rdown;
    keys[SDLK_LEFT] = (!navi & lleft) | rleft;
    keys[SDLK_RIGHT] = (!navi & lright) | rright;
    keys[SDLK_z] = button(BTN_SQUARE); // Hit
    keys[SDLK_x] = button(BTN_TRIANGLE); // Action
    keys[SDLK_c] = !navi & button(BTN_CROSS); // Carry
    keys[SDLK_SPACE] = button(BTN_CIRCLE); // Talk / Read
    keys[SDLK_LCTRL] = navi; // Hi Navi
    keys[SDLK_i] = button(BTN_LEFT); // Troc items
    keys[SDLK_p] = button(BTN_UP); // Map
    keys[SDLK_o] = button(BTN_DOWN); // Oni Link Transformation
    keys[SDLK_v] = ctrl & button(BTN_CROSS);
    keys[SDLK_LSHIFT] = button(BTN_RTRIGGER); // Run
    keys[SDLK_g] = !navi & button(BTN_RIGHT); // Open item menu
    keys[SDLK_RETURN] = !navi & button(BTN_CROSS); // Confirm / Pass text
    keys[SDLK_ESCAPE] = button(BTN_START); // Escape game
    keys[SDLK_F1] = button(BTN_SELECT); // Help
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
