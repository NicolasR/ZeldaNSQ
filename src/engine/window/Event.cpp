#include "Event.h"

Event::Event() {
    for (int i = 0; i < NB_KEYS; i++) {
         current[i] = false;
         past[i] = false;
     }
}

Event::~Event() {
}

void Event::update(Uint8* keys) {
    for (int i = 0; i < NB_KEYS; i++) {
        past[i] = current[i];
        current[i] = false;
    }

    current[kUp] = keys[SDLK_UP];
    current[kDown] = keys[SDLK_DOWN];
    current[kLeft] = keys[SDLK_LEFT];
    current[kRight] = keys[SDLK_RIGHT];

    current[kA] = keys[SDLK_a];
    current[kB] = keys[SDLK_b];
    current[kC] = keys[SDLK_c];
    current[kD] = keys[SDLK_d];
    current[kE] = keys[SDLK_e];
    current[kF] = keys[SDLK_f];
    current[kG] = keys[SDLK_g];
    current[kH] = keys[SDLK_h];
    current[kI] = keys[SDLK_i];
    current[kJ] = keys[SDLK_j];
    current[kK] = keys[SDLK_k];
    current[kL] = keys[SDLK_l];
    current[kM] = keys[SDLK_m];
    current[kN] = keys[SDLK_n];
    current[kO] = keys[SDLK_o];
    current[kP] = keys[SDLK_p];
    current[kQ] = keys[SDLK_q];
    current[kR] = keys[SDLK_r];
    current[kS] = keys[SDLK_s];
    current[kT] = keys[SDLK_t];
    current[kU] = keys[SDLK_u];
    current[kV] = keys[SDLK_v];
    current[kW] = keys[SDLK_w];
    current[kX] = keys[SDLK_x];
    current[kY] = keys[SDLK_y];
    current[kZ] = keys[SDLK_z];

    current[k0] = keys[SDLK_0] || keys[SDLK_KP0];
    current[k1] = keys[SDLK_1] || keys[SDLK_KP1];
    current[k2] = keys[SDLK_2] || keys[SDLK_KP2];
    current[k3] = keys[SDLK_3] || keys[SDLK_KP3];
    current[k4] = keys[SDLK_4] || keys[SDLK_KP4];
    current[k5] = keys[SDLK_5] || keys[SDLK_KP5];
    current[k6] = keys[SDLK_6] || keys[SDLK_KP6];
    current[k7] = keys[SDLK_7] || keys[SDLK_KP7];
    current[k8] = keys[SDLK_8] || keys[SDLK_KP8];
    current[k9] = keys[SDLK_9] || keys[SDLK_KP9];

    current[kComma] = keys[SDLK_COMMA];
    current[kEscape] = keys[SDLK_ESCAPE];
    current[kTab] = keys[SDLK_TAB];
    current[kShift] = keys[SDLK_CAPSLOCK] || keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT];
    current[kCtrl] = keys[SDLK_LCTRL] || keys[SDLK_RCTRL];
    current[kAlt] = keys[SDLK_LALT] || keys[SDLK_RALT];
    current[kSpace] = keys[SDLK_SPACE];
    current[kReturn] = keys[SDLK_RETURN] || keys[SDLK_KP_ENTER];

    current[QUIT] = false;
    current[RESIZE] = current[kCtrl] && current[kReturn];
    current[QUIT_FORCED] = current[kAlt] && keys[SDLK_F4];

    current[kF1] = keys[SDLK_F1];

    if (current[RESIZE]) {
        current[kReturn] = false;
    }

}

bool Event::isDown(Keys key) {
    if (current == 0) {
        return false;
    }
    return current[key];
}

bool Event::isPushed(Keys key) {
    if (current == 0 || past == 0) {
        return false;
    }
    return current[key] && !past[key];
}

void Event::setEvent(Keys key, bool b) {
    if (current != 0) {
        current[key] = b;
    }
}
