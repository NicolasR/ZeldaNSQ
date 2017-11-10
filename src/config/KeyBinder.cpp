#include "KeyBinder.h"

KeyBinder KeyBinder::instance=KeyBinder();

KeyBinder::KeyBinder() {
    // default values :
    binding[UP] = kUp;
    binding[DOWN] = kDown;
    binding[LEFT] = kLeft;
    binding[RIGHT] = kRight;

    binding[RUN] = kShift;

    binding[HIT] = kZ;
    binding[LOAD] = kZ;
    binding[HIT2] = kW;
    binding[LOAD2] = kW;
    binding[ACTION] = kX;
    binding[CARRY] = kC;
    binding[ULTI] = kV;

    binding[TALK] = kSpace;

    binding[NAVI] = kCtrl;

    binding[TROC_ITEMS] = kI;
    binding[ONI] = kO;
    binding[DISPLAY_MAP] = kP;

    binding[MAP_1] = k1;
    binding[MAP_2] = k2;
    binding[MAP_3] = k3;
    binding[MAP_4] = k4;
    binding[MAP_5] = k5;
    binding[MAP_6] = k6;
    binding[MAP_7] = k7;
    binding[MAP_8] = k8;
    binding[MAP_9] = k9;

    binding[ENTER] = kReturn;
    binding[QUIT_GAME] = kEscape;

    binding[AIDE] = kF1;
}

KeyBinder::~KeyBinder() {
}

KeyBinder* KeyBinder::getInstance() {
    return &instance;
}

Action* KeyBinder::bind(Event* event) {

    action.setAction(UP, event->isDown(binding[UP]));
    action.setAction(DOWN, event->isDown(binding[DOWN]));
    action.setAction(LEFT, event->isDown(binding[LEFT]));
    action.setAction(RIGHT, event->isDown(binding[RIGHT]));

    action.setAction(PUSH_UP, event->isPushed(binding[UP]));
    action.setAction(PUSH_DOWN, event->isPushed(binding[DOWN]));
    action.setAction(PUSH_LEFT, event->isPushed(binding[LEFT]));
    action.setAction(PUSH_RIGHT, event->isPushed(binding[RIGHT]));

    action.setAction(RUN, event->isDown(binding[RUN]));

    action.setAction(HIT, (event->isPushed(binding[HIT]) && !event->isDown(binding[LOAD2]))
                     || (event->isPushed(binding[HIT2]) && !event->isDown(binding[LOAD])));
    action.setAction(LOAD, event->isDown(binding[LOAD]) || event->isDown(binding[LOAD2]));
    action.setAction(ACTION, event->isPushed(binding[ACTION]));
    action.setAction(CARRY, event->isPushed(binding[CARRY]));
    action.setAction(ULTI, event->isPushed(binding[ULTI]));

    action.setAction(TALK, event->isPushed(binding[TALK]));

    action.setAction(NAVI, event->isDown(binding[NAVI]));

    action.setAction(TROC_ITEMS, event->isPushed(binding[TROC_ITEMS]));
    action.setAction(ONI, event->isPushed(binding[ONI]));
    action.setAction(DISPLAY_MAP, event->isPushed(binding[DISPLAY_MAP]));

    action.setAction(MAP_1, event->isPushed(binding[MAP_1]));
    action.setAction(MAP_2, event->isPushed(binding[MAP_2]));
    action.setAction(MAP_3, event->isPushed(binding[MAP_3]));
    action.setAction(MAP_4, event->isPushed(binding[MAP_4]));
    action.setAction(MAP_5, event->isPushed(binding[MAP_5]));
    action.setAction(MAP_6, event->isPushed(binding[MAP_6]));
    action.setAction(MAP_7, event->isPushed(binding[MAP_7]));
    action.setAction(MAP_8, event->isPushed(binding[MAP_8]));
    action.setAction(MAP_9, event->isPushed(binding[MAP_9]));

    action.setAction(AIDE, event->isPushed(binding[AIDE]));

    action.setAction(ENTER, event->isPushed(binding[ENTER]));
    action.setAction(QUIT_GAME, event->isPushed(binding[QUIT_GAME]));

    return &action;
}

void KeyBinder::load(string filename) {
    // TODO
}

void KeyBinder::save(string filename) {
    // TODO
}
