#include "WindowManager.h"

#ifdef __vita__
#include <vitaGL.h>
#include <imgui_vita.h>
bool bilinear = true;
bool vflux_window = false;
bool credits_window = false;
bool vflux_enabled = false;
float vcolors[3];
uint16_t *vindices;
float *colors;
float *vertices;
uint64_t tick;
SDL_Shader shader = SDL_SHADER_NONE;

void ImGui_callback() {
    ImGui_ImplVitaGL_NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Graphics")) {
            if (ImGui::MenuItem("Bilinear Filter", nullptr, bilinear)) {
                bilinear = !bilinear;
                SDL_SetVideoModeBilinear(bilinear);
            }
            if (ImGui::MenuItem("vFlux Config", nullptr, vflux_window)) {
                vflux_window = !vflux_window;
            }
            if (ImGui::BeginMenu("Shaders")) {
                if (ImGui::MenuItem("None", nullptr, shader == SDL_SHADER_NONE)) {
                    shader = SDL_SHADER_NONE;
                    SDL_SetVideoShader(SDL_SHADER_NONE);
                }
                if (ImGui::MenuItem("Sharp Bilinear", nullptr, shader == SDL_SHADER_SHARP_BILINEAR)) {
                    shader = SDL_SHADER_SHARP_BILINEAR;
                    SDL_SetVideoShader(SDL_SHADER_SHARP_BILINEAR);
                }
                if (ImGui::MenuItem("Sharp Bilinear (Scancode)", nullptr, shader == SDL_SHADER_SHARP_BILINEAR_SIMPLE)) {
                    shader = SDL_SHADER_SHARP_BILINEAR_SIMPLE;
                    SDL_SetVideoShader(SDL_SHADER_SHARP_BILINEAR_SIMPLE);
                }
                if (ImGui::MenuItem("LCD 3x", nullptr, shader == SDL_SHADER_LCD3X)) {
                    shader = SDL_SHADER_LCD3X;
                    SDL_SetVideoShader(SDL_SHADER_LCD3X);
                }
                if (ImGui::MenuItem("xBR x2", nullptr, shader == SDL_SHADER_XBR_2X_FAST)) {
                    shader = SDL_SHADER_XBR_2X_FAST;
                    SDL_SetVideoShader(SDL_SHADER_XBR_2X_FAST);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Info")) {
            if (ImGui::MenuItem("Credits", nullptr, credits_window)) {
                credits_window = !credits_window;
            }
            ImGui::EndMenu();
        }

        if (vflux_window) {
            ImGui::Begin("vFlux Configuration", &vflux_window);
            ImGui::ColorPicker3("Filter Color", vcolors);
            ImGui::Checkbox("Enable vFlux", &vflux_enabled);
            ImGui::End();
        }

        if (credits_window) {
            ImGui::Begin("Credits", &credits_window);
            ImGui::TextColored(ImVec4(255, 255, 0, 255), "Zelda: Navi's Quest v1.1");
            ImGui::Text("Game Creator: Vincent Jouillat");
            ImGui::Text("Port Author: usineur");
            ImGui::Separator();
            ImGui::TextColored(ImVec4(255, 255, 0, 255), "Special thanks to:");
            ImGui::Text("Rinnegatamante: SDL 1.2 and imgui Vita ports");
            ImGui::End();
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(870);

        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }

    if (vflux_enabled) {
        memcpy(&colors[0], vcolors, sizeof(float) * 3);
        memcpy(&colors[4], vcolors, sizeof(float) * 3);
        memcpy(&colors[8], vcolors, sizeof(float) * 3);
        memcpy(&colors[12], vcolors, sizeof(float) * 3);

        float a;
        SceDateTime time;
        sceRtcGetCurrentClockLocalTime(&time);
        if (time.hour < 6)        // Night/Early Morning
            a = 0.25f;
        else if (time.hour < 10) // Morning/Early Day
            a = 0.1f;
        else if (time.hour < 15) // Mid day
            a = 0.05f;
        else if (time.hour < 19) // Late day
            a = 0.15f;
        else // Evening/Night
            a = 0.2f;
        colors[3] = colors[7] = colors[11] = colors[15] = a;
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        vglIndexPointerMapped(vindices);
        vglVertexPointerMapped(vertices);
        vglColorPointerMapped(GL_FLOAT, colors);
        vglDrawObjects(GL_TRIANGLE_FAN, 4, true);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    ImGui::Render();
    ImGui_ImplVitaGL_RenderDrawData(ImGui::GetDrawData());

    SceTouchData touch;
    sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);
    uint64_t delta_touch = sceKernelGetProcessTimeWide() - tick;
    if (touch.reportNum > 0) {
        ImGui::GetIO().MouseDrawCursor = true;
        tick = sceKernelGetProcessTimeWide();
    } else if (delta_touch > 3000000) {
        ImGui::GetIO().MouseDrawCursor = false;
    }
}
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
#ifdef __vita__
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
        joystick = SDL_JoystickOpen(0);

        sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
        sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
#endif
        atexit(SDL_Quit);

#ifndef __vita__
        SDL_WM_SetCaption(title.c_str(), NULL);
        SDL_Surface* icon = SDL_LoadBMP(iconName.c_str());
        SDL_SetColorKey(icon, SDL_SRCCOLORKEY, SDL_MapRGB(icon->format,0,0,0));
        SDL_WM_SetIcon(icon, NULL);

        SDL_ShowCursor(isFullScreen ? SDL_DISABLE : SDL_ENABLE);
#else
        SDL_ShowCursor(SDL_DISABLE);
#endif

#ifdef __vita__
        window = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
        int sh = 544;
        int sw = (float)window->w * ((float)sh / (float)window->h);
        int x = (960 - sw) / 2;
        SDL_SetVideoModeScaling(x, 0, sw, sh);

        vindices = (uint16_t*)malloc(sizeof(uint16_t)*4);
        colors = (float*)malloc(sizeof(float)*4*4);
        vertices = (float*)malloc(sizeof(float)*3*4);
        vertices[0] =   0.0f;
        vertices[1] =   0.0f;
        vertices[2] =   0.0f;
        vertices[3] = 960.0f;
        vertices[4] =   0.0f;
        vertices[5] =   0.0f;
        vertices[6] = 960.0f;
        vertices[7] = 544.0f;
        vertices[8] =   0.0f;
        vertices[9] =   0.0f;
        vertices[10]= 544.0f;
        vertices[11]=   0.0f;
        vindices[0] = 0;
        vindices[1] = 1;
        vindices[2] = 2;
        vindices[3] = 3;

        ImGui::CreateContext();
        ImGui_ImplVitaGL_Init();
        ImGui_ImplVitaGL_TouchUsage(true);
        ImGui_ImplVitaGL_MouseStickUsage(false);
        ImGui_ImplVitaGL_UseIndirectFrontTouch(true);
        ImGui::StyleColorsDark();
        ImGui::GetIO().MouseDrawCursor = false;

        SDL_SetVideoCallback(ImGui_callback);
#else
        window = SDL_SetVideoMode(640, 480, 32, isFullScreen ? SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN : SDL_HWSURFACE|SDL_DOUBLEBUF);
#endif
        windowTmp = SDL_CreateRGBSurface(SDL_HWSURFACE, 640, 480, 32, 0, 0, 0, 0);

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

#ifdef __vita__
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

#ifdef __vita__
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

#ifdef __vita__
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
