#include "SlideManager.h"

SlideManager::SlideManager() {
    raz();
}

SlideManager::~SlideManager() {
}

void SlideManager::setMoves(int dx, int dy, int vitesse) {

    if (dy != 0) {
        if (dy > 0) {
            fillHisto(1, vitesse);
            fillHisto(0, 0);
        } else {
            fillHisto(0, vitesse);
            fillHisto(1, 0);
        }
    } else {
        fillHisto(0, 0);
        fillHisto(1, 0);
    }

    if (dx != 0) {
        if (dx > 0) {
            fillHisto(3, vitesse);
            fillHisto(2, 0);
        } else {
            fillHisto(2, vitesse);
            fillHisto(3, 0);
        }

    } else {
        fillHisto(2, 0);
        fillHisto(3, 0);
    }
}

void SlideManager::fillHisto(int dir, int vitesse) {

    int oldSpeed = 0;
    if (elan[dir] > 0) {
        oldSpeed = histo[dir][elan[dir] - 1];
    }
    if (vitesse < oldSpeed) {
        computeDistance(dir);
    }

    if (vitesse) {
        if (elan[dir] < 16) {
            elan[dir]++;
        } else {
            for (int i = 0; i < 15; i++) {
                histo[dir][i] = histo[dir][i + 1];
            }
        }
        histo[dir][elan[dir] - 1] = vitesse;
    } else {
        elan[dir] = 0;
        histo[dir][0] = vitesse;
    }

}

void SlideManager::computeDistance(int dir) {
    int total = 0;
    int last = 0;
    for (int i = 0; i < elan[dir]; i++) {
        last = histo[dir][i];
        total += last;
    }
    distanceToSlide[dir] = total;
    speedToSlide[dir] = last;
}

int SlideManager::getSlideX() {

    // W
    int totalW = 0;
    if (distanceToSlide[2] > 0 && speedToSlide[2] > 0) {
        int currentSpeed = 0;
        if (elan[2] > 0) {
            currentSpeed = histo[2][elan[2] - 1];
        }

        if (distanceToSlide[2] > speedToSlide[2]) {
            totalW = speedToSlide[2];

        } else {
            totalW = distanceToSlide[2];
        }
        distanceToSlide[2] -= totalW;
        totalW -= currentSpeed;
        if (totalW < 0) {
            totalW = 0;
        }
    }

    // E
    int totalE = 0;
    if (distanceToSlide[3] > 0 && speedToSlide[3] > 0) {
        int currentSpeed = 0;
        if (elan[3] > 0) {
            currentSpeed = histo[3][elan[3] - 1];
        }

        if (distanceToSlide[3] > speedToSlide[3]) {
            totalE = speedToSlide[3];

        } else {
            totalE = distanceToSlide[3];
        }
        distanceToSlide[3] -= totalE;
        totalE -= currentSpeed;
        if (totalE < 0) {
            totalE = 0;
        }
    }

    return totalE - totalW;
}

int SlideManager::getSlideY() {

    // N
    int totalN = 0;
    if (distanceToSlide[0] > 0 && speedToSlide[0] > 0) {
        int currentSpeed = 0;
        if (elan[0] > 0) {
            currentSpeed = histo[0][elan[0] - 1];
        }

        if (distanceToSlide[0] > speedToSlide[0]) {
            totalN = speedToSlide[0];

        } else {
            totalN = distanceToSlide[0];
        }
        distanceToSlide[0] -= totalN;
        totalN -= currentSpeed;
        if (totalN < 0) {
            totalN = 0;
        }
    }

    // S
    int totalS = 0;
    if (distanceToSlide[1] > 0 && speedToSlide[1] > 0) {
        int currentSpeed = 0;
        if (elan[1] > 0) {
            currentSpeed = histo[1][elan[1] - 1];
        }

        if (distanceToSlide[1] > speedToSlide[1]) {
            totalS = speedToSlide[1];

        } else {
            totalS = distanceToSlide[1];
        }
        distanceToSlide[1] -= totalS;
        totalS -= currentSpeed;
        if (totalS < 0) {
            totalS = 0;
        }
    }

    return totalS - totalN;
}

void SlideManager::stopX() {
    for (int i = 2; i < 4; i++) {
        for (int j = 0; j < 16; j++) {
            histo[i][j] = 0;
        }
        speedToSlide[i] = 0;
        distanceToSlide[i] = 0;
        elan[i] = 0;
    }
}

void SlideManager::stopY() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 16; j++) {
            histo[i][j] = 0;
        }
        speedToSlide[i] = 0;
        distanceToSlide[i] = 0;
        elan[i] = 0;
    }
}

void SlideManager::raz() {
    stopY();
    stopX();
}
