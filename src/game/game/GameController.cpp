#include "GameController.h"

#include "../../config/KeyBinder.h"

GameController::GameController() : needLoad(true), timeOffset(0) {
}

GameController::~GameController() {
}

void GameController::launch() {
    chrono.reset();
    setStep(GAME_MAIN);
}

void GameController::handleEvents(Event* event) {
    if (!transition.isRunning()) {

        Action* action = KeyBinder::getInstance()->bind(event);
        switch(step) {
            case GAME_MAIN : scene.handleActions(action); break;
            case GAME_MENU : menu.handleActions(action); break;
            case GAME_TROC : troc.handleActions(action); break;
            case GAME_TEXT : text.handleActions(action); break;
            case GAME_OCARINA : ocarina.handleActions(action); break;
            case GAME_MAP : map.handleActions(action); break;
            case GAME_HELP : help.handleActions(action); break;
            default : break;
        }

    }
    // Escape should not end the game
    if (event->isPushed(kEscape)) {
        event->setEvent(kEscape, false);
    }
}

void GameController::loop() {
    if (transition.isRunning()) {
        return;
    }
    switch(step) {
        case GAME_MAIN : scene.loop(); break;
        case GAME_MENU : menu.loop(); break;
        case GAME_TROC : troc.loop(); break;
        case GAME_TEXT : text.loop(); break;
        case GAME_SWITCH_MAP : transitionMaps.loop(); break;
        case GAME_SWITCH_ROOM : transitionRooms.loop(); break;
        case GAME_TELEPORT : teleport.loop(); break;
        case GAME_OCARINA : ocarina.loop(); break;
        case GAME_MAP : map.loop(); break;
        case GAME_HELP : help.loop(); break;
        default : break;
    }
}

void GameController::draw() {
    switch(step) {
        case GAME_MAIN : case GAME_OCARINA : case GAME_SWITCH_ROOM : scene.draw(); break;
        case GAME_MENU : scene.draw(); menu.draw(); break;
        case GAME_TROC : scene.draw(); troc.draw(); break;
        case GAME_TEXT : scene.draw(); text.draw(); break;
        case GAME_SWITCH_MAP : transitionMaps.draw(); break;
        case GAME_TELEPORT : scene.draw(); teleport.draw(); break;
        case GAME_MAP : map.draw(); break;
        case GAME_HELP : help.draw(); break;
        default : break;
    }

    if (transition.isRunning()) {
        transition.loop();
        transition.draw();

        if (transition.isWaiting()) {
            setStep(nextStep);
            transition.restart();
        } else if (transition.isAchieved()) {
            transition.reset();
        }
    }
}

void GameController::setStep(GameStep newStep) {
    step = newStep;
    switch(step) {
        case GAME_MAIN : if (needLoad) {scene.launch(save); menu.init(); needLoad = false;} break;
        case GAME_MENU : menu.launch(); break;
        case GAME_TROC : troc.launch(); break;
        case GAME_TEXT : text.launch(); break;
        case GAME_SWITCH_MAP : transitionMaps.launch(); break;
        case GAME_SWITCH_ROOM : transitionRooms.launch(); break;
        case GAME_TELEPORT : teleport.launch(); break;
        case GAME_OCARINA : ocarina.launch(); break;
        case GAME_MAP : map.launch(); break;
        case GAME_HELP : help.launch(); break;
        default : break;
    }
}

void GameController::setSave(Save* sv) {
    save = sv;
    timeOffset = save->getTime();
    needLoad = true;
}

void GameController::displayText(int id) {
    text.setTextId(id);
    setStep(GAME_TEXT);
}

void GameController::displayMap(int mapId, bool withTp) {
    if (mapId >= 15 && mapId < 58) return;
    map.setStep(mapId, withTp);
    nextStep = GAME_MAP;
    transition.start();
}

void GameController::hideMap() {
    if (step != GAME_MAP) return;
    nextStep = GAME_MAIN;
    transition.start();
}

bool GameController::isTransitionRoom() {
    return step == GAME_SWITCH_ROOM;
}

bool GameController::isTeleport() {
    return step == GAME_TELEPORT;
}

SceneController* GameController::getSceneController() {
    return &scene;
}

GameMenuController* GameController::getGameMenuController() {
    return &menu;
}

GameTrocController* GameController::getGameTrocController() {
    return &troc;
}

TextController* GameController::getTextController() {
    return &text;
}

TeleportController* GameController::getTeleportController() {
    return &teleport;
}

OcarinaController* GameController::getOcarinaController() {
    return &ocarina;
}

MapController* GameController::getMapController() {
    return &map;
}

void GameController::saveData(bool endGame) {
    scene.saveData();
    save->setTime(timeOffset + (chrono.getElapsedTime() / 1000));
    save->save(endGame);
}

int GameController::getTime() {
    int t = timeOffset + (chrono.getElapsedTime() / 1000);
    if (t > 359999) t = 359999;
    return t;
}

Save* GameController::getSave() {
    return save;
}

