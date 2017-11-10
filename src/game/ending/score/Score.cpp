#include "Score.h"

#include "../../MainController.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/texts/TextManager.h"
#include "../../../engine/audio/AudioManager.h"

Score::Score() : surface(0) {
}

Score::~Score() {
    delete surface;
}

void Score::init() {
    delete surface;
    surface = new Surface(320, 240);

    WImage* image = ResourceManager::getInstance()->loadImage("data/images/ending/arbre.png");
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 7; i++) {
            surface->put(image, i * 48, j * 96, 0, 0, 48, 96);
        }
    }
    ResourceManager::getInstance()->free(image);

    int letterSize = TextManager::getInstance()->getWSpace();

    // SCORES :
    Text* text = TextManager::getInstance()->getText(657);
    int size = text->getLength() * letterSize;
    text->displayOnSurface(160 - (size / 2), 10, surface);
    delete text;

    Save* save = MainController::getInstance()->getGameController()->getSave();

    // TEMPS DE JEU :
    text = TextManager::getInstance()->getText(658);
    size = text->getLength() * letterSize;
    text->displayOnSurface(10, 30, surface);
    delete text;

    int time = save->getTime();

    ostringstream os;
    int hours = time / 3600;
    int minutes = (time % 3600) / 60;
    int seconds = time % 60;
    if (hours < 10) os << "0";
    os << hours << ":";
    if (minutes < 10) os << "0";
    os << minutes << ":";
    if (seconds < 10) os << "0";
    os << seconds;
    text = new Text(os.str());
    text->displayOnSurface(10 + size + letterSize, 30, surface);
    delete text;

    // MORT N FOIS
    text = TextManager::getInstance()->getText(save->getDeaths() < 2 ? 766 : 659);
    text->displayOnSurface(160, 30, surface);
    delete text;

    // NAVI
    text = TextManager::getInstance()->getText(664);
    text->displayOnSurface(10, 50, surface);
    delete text;

    // 1/4 COEURS
    text = TextManager::getInstance()->getText(660);
    text->displayOnSurface(160, 50, surface);
    delete text;

    // GEMMES FORCE
    text = TextManager::getInstance()->getText(661);
    text->displayOnSurface(10, 70, surface);
    delete text;

    // COQUILLAGES
    text = TextManager::getInstance()->getText(662);
    text->displayOnSurface(160, 70, surface);
    delete text;

    // TROC
    text = TextManager::getInstance()->getText(663);
    text->displayOnSurface(10, 90, surface);
    delete text;

    // OBJETS
    text = TextManager::getInstance()->getText(665);
    text->displayOnSurface(160, 90, surface);
    delete text;

    // TOTAL
    text = TextManager::getInstance()->getText(666);
    text->displayOnSurface(10, 130, surface);
    delete text;

    // RANG
    text = TextManager::getInstance()->getText(667);
    size = text->getLength() * letterSize;
    text->displayOnSurface(10, 170, surface);
    delete text;

    // %
    int tmp = MainController::getInstance()->getGameController()->getSave()->getCompletion();
    if (tmp == 100) { // touriste
        text = TextManager::getInstance()->getText(668);
    } else if (tmp >= 95) { // vacancier
        text = TextManager::getInstance()->getText(669);
    } else if (tmp >= 90) { // voyageur
        text = TextManager::getInstance()->getText(670);
    } else { // explorateur
        text = TextManager::getInstance()->getText(671);
    }
    // RANG 1
    text->displayOnSurface(10 + size + letterSize, 170, surface);
    delete text;

    // RANG 2
    int morts = MainController::getInstance()->getGameController()->getSave()->getDeaths();
    if (morts >= 70) {
        text = TextManager::getInstance()->getText(672);
        text->displayOnSurface(10 + size + letterSize, 190, surface);
        delete text;
    } else if (morts >= 30) {
        text = TextManager::getInstance()->getText(673);
        text->displayOnSurface(10 + size + letterSize, 190, surface);
        delete text;
    }

}

void Score::handleEvents(Event* event) {
    if (event->isPushed(kReturn)) {
        AudioManager::getInstance()->playSound(TS_MENU1);
        MainController::getInstance()->getEndingController()->setStep(END_CREDITS);
    }
}

void Score::draw() {
    WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);
}
