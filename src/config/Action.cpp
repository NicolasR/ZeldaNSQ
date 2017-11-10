#include "Action.h"

Action::Action() {
    for (int i = 0; i < NB_ACTIONS; i++) {
        actions[i] = false;
    }
}

Action::~Action() {
}

void Action::setAction(ActionType key, bool b) {
    actions[key] = b;
}

bool Action::isAction(ActionType key) {
    return actions[key];
}
