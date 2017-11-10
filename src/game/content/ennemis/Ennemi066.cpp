#include "Ennemi066.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"
#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/FumeeBlanche.h"

Ennemi066::Ennemi066(int i, int j, Map* map) : anim(0), animMax(3), vanim(180), animTentacules(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/ennemis/ennemi66.png", true);
    chrono.reset();

    x = i;
    y = j;

    // for quadtree operations:
    width = 96;
    height = 124;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    life = expert ? 240 : 160;
    maxLife = expert ? 240 : 160;
    recul = 0;
    vitesseRecul = 0;

    isBoss = true;
    stunnable = false;

    tentaculeHG = new Ennemi068(x - 16 * 2 + 12, y + 16 * 6 - 4);
    tentaculeBG = new Ennemi068(x - 16 * 2 + 4, y + 16 * 7 + 12);
    tentaculeHD = new Ennemi069(x + 16 * 5 + 10 - 57, y + 16 * 6 - 4);
    tentaculeBD = new Ennemi069(x + 16 * 5 + 18 - 57, y + 16 * 7 + 12);

    map->addEnnemi(tentaculeHG);
    map->addEnnemi(tentaculeBG);
    map->addEnnemi(tentaculeHD);
    map->addEnnemi(tentaculeBD);

    for (int i = 0; i < 4; i++) steps[i] = 0;

    forceEnn = expert ? 20 : 8;
}

Ennemi066::~Ennemi066() {
    ResourceManager::getInstance()->free(image);
}

void Ennemi066::reset() {
    Ennemi::reset();
    chrono.reset();
    anim = 0;
    animTentacules = 0;
    for (int i = 0; i < 4; i++) steps[i] = 0;
}

bool Ennemi066::isResetable() {
    return alive;
}

void Ennemi066::ennLoop() {

    testDegatOnLink(&box, direction, forceEnn, TA_PHYSIC, TE_NORMAL);

    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
        }
        animTentacules++;
        if (animTentacules == 10) {
            // wake up tentacules
            wakeUp();
        } else if (animTentacules == 15) {
            setSteps(2);
        } else if (animTentacules == 20) {
            setSteps(3);
        } else if (animTentacules == 21) {
            setSteps(0);
            animTentacules = 0;
        }
        chrono.reset();
    }
}

void Ennemi066::draw(int offsetX, int offsetY) {
    if (!alive) {
        return;
    }

    int dstX = x - offsetX;
    int dstY = y - offsetY;

    WindowManager::getInstance()->draw(image, anim * width, 0, width, height, dstX, dstY);
}

int Ennemi066::getX() {
    return x;
}

int Ennemi066::getY() {
    return y;
}

BoundingBox* Ennemi066::getBoundingBox() {
    box.setX(x);
    box.setY(y);
    return &box;
}

void Ennemi066::giveItem(int x, int y) {
    AudioManager::getInstance()->playSound(TS_KILLENNEMY);
    Map* map = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getMap();
    map->addEffect(new FumeeBlanche(x, y));
    tentaculeHG->endOfLife();
    tentaculeBG->endOfLife();
    tentaculeHD->endOfLife();
    tentaculeBD->endOfLife();
    map->addItem(ItemHelper::getInstance()->createItem(TI_COEUR, x + 1, y + 7 * 16 + 4, 0));
}

int Ennemi066::getDown() {
    return y + 118;
}

void Ennemi066::wakeUp() {
    if (life > maxLife / 2) {
        wakeUp(1);
    } else if (life > maxLife / 4) {
        wakeUp(2);
    } else if (life > 10) {
        wakeUp(3);
    } else {
        wakeUp(4);
    }

    for (int i = 0; i < 4; i++) {
        if (steps[i] == 1) {
            switch (i) {
                case 0 : tentaculeHG->setStep(1); break;
                case 1 : tentaculeBG->setStep(1); break;
                case 2 : tentaculeHD->setStep(1); break;
                case 3 : tentaculeBD->setStep(1); break;
            }
        }
    }
}

void Ennemi066::wakeUp(int nb) {
    if (nb == 4) {
        for (int i = 0; i < 4; i++) {
            steps[i] = 1;
        }
        return;
    }

    if (nb == 3) {
        int randomValue = (int)((float)rand() / RAND_MAX * (4));
        for (int i = 0; i < 4; i++) {
            if (i != randomValue) {
                steps[i] = 1;
            }
        }
        return;
    }

    int total = 4;

    while (nb > 0) {
        int randomValue = (int)((float)rand() / RAND_MAX * (total));

        for (int i = 0; i < 4; i++) {
            if (steps[i]) {
                randomValue++;
            } else if (i == randomValue) {
                steps[i] = 1;
            }
        }
        total--;
        nb--;
    }
}

void Ennemi066::setSteps(int value) {
    for (int i = 0; i < 4; i++) {
        if (steps[i] > 0) {
            steps[i] = value;
            switch (i) {
                case 0 : tentaculeHG->setStep(value); break;
                case 1 : tentaculeBG->setStep(value); break;
                case 2 : tentaculeHD->setStep(value); break;
                case 3 : tentaculeBD->setStep(value); break;
            }
        }
    }
}
