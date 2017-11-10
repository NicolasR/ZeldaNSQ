#include "Ocarina.h"

#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

Ocarina::Ocarina() : id(0) {
}

Ocarina::~Ocarina() {
}

void Ocarina::handleActions(Action* action) {
    if (action->isAction(ENTER)) {

        Inventory* inventory = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink()->getInventory();
        switch (id) {
            case 0 : if (!inventory->hasObject(CHANT_1)) return; break;
            case 1 : if (!inventory->hasObject(CHANT_2)) return; break;
            case 2 : if (!inventory->hasObject(CHANT_3)) return; break;
            default : break;
        }

        AudioManager::getInstance()->stopSound(1);
        stop();
        return;
    }
}

void Ocarina::loop() {
    if (!AudioManager::getInstance()->isPlaying(1)) {
        stop();
    }
}

void Ocarina::start() {
    AudioManager::getInstance()->stopMusic();
    switch (id) {
        case 0 : AudioManager::getInstance()->playSound(TS_SONG_1, 1); break;
        case 1 : AudioManager::getInstance()->playSound(TS_SONG_2, 1); break;
        case 2 : AudioManager::getInstance()->playSound(TS_SONG_3, 1); break;
        default : AudioManager::getInstance()->playSound(TS_SONG_1, 1); break;
    }
}

void Ocarina::stop() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    Link* link = scene->getLink();
    link->setAnimation(IDLE);

    MainController::getInstance()->getGameController()->setStep(GAME_MAIN);

    Inventory* inventory = link->getInventory();
    switch (id) {
        case 0 : if (!inventory->hasObject(CHANT_1)) link->trouveObjet(TI_CHANT_1); break;
        case 1 : if (!inventory->hasObject(CHANT_2)) link->trouveObjet(TI_CHANT_2); break;
        case 2 : if (!inventory->hasObject(CHANT_3)) link->trouveObjet(TI_CHANT_3); break;
        default : break;
    }

    scene->getMap()->songPlayed(id);
    AudioManager::getInstance()->replayMusic();
}

void Ocarina::setSong(int i) {
    id = i;
}
