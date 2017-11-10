#include "WResource.h"

WResource::WResource() : count(0), name("") {
}

WResource::~WResource() {
}

string WResource::getName() {
    return name;
}

void WResource::load() {
    count++;
}

int WResource::unload() {
    return --count;
}
