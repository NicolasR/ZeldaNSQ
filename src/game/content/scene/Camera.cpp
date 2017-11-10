#include "Camera.h"

Camera::Camera() : vitesse(0), targetX(0), targetY(0), manuel(false) {
}

Camera::~Camera() {
}

void Camera::setX(int x) {
    if (x < bounds.getX()) {
        x = bounds.getX();
    }
    if (x + box.getW() > bounds.getX() + bounds.getW()) {
        x = bounds.getX() + bounds.getW() - box.getW();
    }
    box.setX(x);
}

void Camera::setY(int y) {
    if (y < bounds.getY()) {
        y = bounds.getY();
    }
    if (y + box.getH() > bounds.getY() + bounds.getH()) {
        y = bounds.getY() + bounds.getH() - box.getH();
    }
    box.setY(y);
}

void Camera::setW(int w) {
    if (w > bounds.getW()) {
        w = bounds.getW();
    }
    box.setW(w);
}

void Camera::setH(int h) {
    if (h > bounds.getH()) {
        h = bounds.getH();
    }
    box.setH(h);
}

void Camera::setBox(int x, int y, int w, int h) {
    setW(w);
    setH(h);
    setX(x);
    setY(y);
}

void Camera::setPosition(int x, int y) {
    x -= box.getW() / 2;
    y -= box.getH() / 2;
    setX(x);
    setY(y);
    vitesse = 0;
    targetX = 0;
    targetY = 0;
}

void Camera::moveTo(int x, int y, int v) {
    x -= box.getW() / 2;
    y -= box.getH() / 2;

    if (x < bounds.getX()) {
        x = bounds.getX();
    }
    if (x + box.getW() > bounds.getX() + bounds.getW()) {
        x = bounds.getX() + bounds.getW() - box.getW();
    }
    if (y < bounds.getY()) {
        y = bounds.getY();
    }
    if (y + box.getH() > bounds.getY() + bounds.getH()) {
        y = bounds.getY() + bounds.getH() - box.getH();
    }
    vitesse = v;
    targetX = x;
    targetY = y;
}

void Camera::setBounds(int x, int y, int w, int h) {
    bounds.setX(x);
    bounds.setY(y);
    bounds.setW(w);
    bounds.setH(h);
}

BoundingBox* Camera::getBoundingBox() {
    return &box;
}

void Camera::loop() {
    if (vitesse > 0) {

        int oldX = box.getX();
        int diffX = targetX - box.getX();
        if (diffX > 0) {
            if (diffX < vitesse) {
                setX(targetX);
            } else {
                setX(oldX + vitesse);
            }
        } else if (diffX < 0) {
            if (diffX > -vitesse) {
                setX(targetX);
            } else {
                setX(oldX - vitesse);
            }
        }

        int oldY = box.getY();
        int diffY = targetY - box.getY();
        if (diffY > 0) {
            if (diffY < vitesse) {
                setY(targetY);
            } else {
                setY(oldY + vitesse);
            }
        } else if (diffY < 0) {
            if (diffY > -vitesse) {
                setY(targetY);
            } else {
                setY(oldY - vitesse);
            }
        }


        if (diffX != 0 && oldX == box.getX()) {
            targetX = box.getX();
        }
        if (diffY != 0 && oldY == box.getY()) {
            targetY = box.getY();
        }

        if (box.getX() == targetX && box.getY() == targetY) {
            vitesse = 0;
        }
    }
}

bool Camera::isManuel() {
    return manuel;
}

void Camera::setManuel(bool b) {
    manuel = b;
}
