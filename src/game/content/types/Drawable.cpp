#include "Drawable.h"

Drawable::Drawable() {
}

Drawable::~Drawable() {
}

int Drawable::compareTo(Listable* other) {
    Drawable* o = dynamic_cast<Drawable*>(other);
    if (o == 0) return 0;
    return o->getDown() - getDown();
}

int Drawable::getDown() {
    return getY() + height;
}
