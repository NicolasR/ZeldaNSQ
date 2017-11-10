#include "MainController.h"

MainController MainController::instance=MainController();

MainController::MainController() : step(LOGO) {
    nextStep = step;
}

MainController::~MainController() {
}

MainController* MainController::getInstance() {
    return &instance;
}

void MainController::init() {
    logo = new LogoController();
    title = new TitleController();
    menu = new MenuController();
    opening = new OpeningController();
    game = new GameController();
    ending = new EndingController();
}

void MainController::close() {
    delete logo;
    delete title;
    delete menu;
    delete opening;
    delete game;
    delete ending;
}

void MainController::control(Event* event) {

    if (!transition.isRunning()) {
        handleEvents(event);
        loop();
    }
    draw();

    if (transition.isRunning()) {
        transition.loop();
        transition.draw();

        if (transition.isWaiting()) {
            setInternalStep(nextStep);
            transition.restart();
        } else if (transition.isAchieved()) {
            transition.reset();
        }
    }

}

void MainController::handleEvents(Event* event) {
    switch(step) {
        case LOGO : logo->handleEvents(event); break;
        case TITRE : title->handleEvents(event); break;
        case MENU : menu->handleEvents(event); break;
        case OPENING : opening->handleEvents(event); break;
        case GAME : game->handleEvents(event); break;
        case ENDING : ending->handleEvents(event); break;
        default : break;
    }
}

void MainController::loop() {
    switch(step) {
        case LOGO : logo->loop(); break;
        case TITRE : title->loop(); break;
        case MENU : menu->loop(); break;
        case OPENING : opening->loop(); break;
        case GAME : game->loop(); break;
        case ENDING : ending->loop(); break;
        default : break;
    }
}

void MainController::draw() {
    switch(step) {
        case LOGO : logo->draw(); break;
        case TITRE : title->draw(); break;
        case MENU : menu->draw(); break;
        case OPENING : opening->draw(); break;
        case GAME : game->draw(); break;
        case ENDING : ending->draw(); break;
        default : break;
    }
}

void MainController::setInternalStep(MainStep newStep) {
    step = newStep;
    switch(step) {
        case LOGO : logo->launch(); break;
        case TITRE : title->launch(); break;
        case MENU : menu->launch(); break;
        case OPENING : opening->launch(); break;
        case GAME : game->launch(); break;
        case ENDING : ending->launch(); break;
        default : break;
    }
}

void MainController::setStep(MainStep newStep) {
    nextStep = newStep;
    transition.start();
}

MenuController* MainController::getMenuController() {
    return menu;
}

GameController* MainController::getGameController() {
    return game;
}

OpeningController* MainController::getOpeningController() {
    return opening;
}

EndingController* MainController::getEndingController() {
    return ending;
}
