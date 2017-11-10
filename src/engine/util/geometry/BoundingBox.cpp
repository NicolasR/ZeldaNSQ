#include "BoundingBox.h"

BoundingBox::BoundingBox() : x(0), y(0), w(0), h(0) {
}

BoundingBox::BoundingBox(int a, int b, int c, int d) : x(a), y(b), w(c), h(d) {
}

BoundingBox::~BoundingBox() {
}

void BoundingBox::setX(int i) {
    x = i;
}

void BoundingBox::setY(int i) {
    y = i;
}

void BoundingBox::setW(int i) {
    w = i;
}

void BoundingBox::setH(int i) {
    h = i;
}

int BoundingBox::getX() {
    return x;
}

int BoundingBox::getY() {
    return y;
}

int BoundingBox::getW() {
    return w;
}

int BoundingBox::getH() {
    return h;
}

bool BoundingBox::intersect(BoundingBox* other) {
    return x < other->x + other->w && x + w > other->x && y < other->y + other->h && y + h > other->y;
}

bool BoundingBox::hasPoint(int i, int j) {
    return (i >= x && i < x + w && j >= y && j < y + h);
}
