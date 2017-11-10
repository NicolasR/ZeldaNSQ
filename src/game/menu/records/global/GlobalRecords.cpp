#include "GlobalRecords.h"

#include "../../../MainController.h"
#include "../../../../engine/window/WindowManager.h"
#include "../../../../engine/resources/ResourceManager.h"
#include "../../../../engine/audio/AudioManager.h"
#include "../../../../engine/texts/TextManager.h"
#include "../../../../config/ConfigurationManager.h"

GlobalRecords::GlobalRecords() : line(0), tir(false), surface(0), surfaceArc(0), skin(0) {
    image = ResourceManager::getInstance()->loadImage("data/images/menus/cadres.png");
    link = ResourceManager::getInstance()->loadImage("data/images/menus/curseur.png", true);
}

GlobalRecords::~GlobalRecords() {
    ResourceManager::getInstance()->free(image);
    ResourceManager::getInstance()->free(link);
    delete surface;
    delete surfaceArc;
}

void GlobalRecords::init() {

    globalSave = GlobalSave::getInstance();
    if (line == 0) line = (globalSave->isExpertUnlocked()) ? 3 : 2;
    skin = ConfigurationManager::getInstance()->getSkin();

    delete surface;
    surface = new Surface(320, 240);

    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 20; i++) {
            surface->put(image, i * 16, j * 16, 16, 16, 16, 16);
        }
    }

    // title
    int letterSize = TextManager::getInstance()->getWSpace();
    Text* text = TextManager::getInstance()->getText(689);
    int size = text->getLength() * letterSize;
    int blocs = (size + 15) / 16;

    int x = 16;
    surface->put(image, x, 0, 0, 0, 16, 16);
    surface->put(image, x, 16, 0, 16, 16, 16);
    surface->put(image, x, 32, 0, 32, 16, 16);

    for (int i = 0; i <= blocs; i++) {
        x += 16;
        surface->put(image, x, 0, 16, 0, 16, 16);
        surface->put(image, x, 16, 16, 64, 16, 16);
        surface->put(image, x, 32, 16, 32, 16, 16);
    }

    x += 16;
    surface->put(image, x, 0, 32, 0, 16, 16);
    surface->put(image, x, 16, 32, 16, 16, 16);
    surface->put(image, x, 32, 32, 32, 16, 16);

    text->displayOnSurface(32 + 8 + ((blocs * 16) - size) / 2, 16, surface);

    delete text;

    if (globalSave->isExpertUnlocked()) {
        cadre(16, 48, 288, 32, surface);
        cadre(16, 96, 288, 32, surface);
        cadre(16, 144, 288, 32, surface);
        cadre(16, 192, 136, 32, surface);

        text = TextManager::getInstance()->getText(690);
        text->displayOnSurface(44, 56, surface);
        delete text;

        ostringstream os;
        int score = globalSave->getScoreArc();
        if (score < 10) os << "0";
        os << score << " / 50";
        text = new Text(os.str());
        text->displayOnSurface(44+208, 56, surface);
        delete text;

        text = TextManager::getInstance()->getText(692);
        text->displayOnSurface(44, 104, surface);
        delete text;

        os.str("");
        int nbRangs = 0;
        if (globalSave->isNormalFull()) nbRangs++;
        if (globalSave->isNormalUlti()) nbRangs++;
        if (globalSave->isNormalRush()) nbRangs++;
        os << nbRangs << " / 3";
        text = new Text(os.str());
        text->displayOnSurface(44+208+6, 104, surface);
        delete text;

        text = TextManager::getInstance()->getText(693);
        text->displayOnSurface(44, 152, surface);
        delete text;

        os.str("");
        nbRangs = 0;
        if (globalSave->isExpertFull()) nbRangs++;
        if (globalSave->isExpertUlti()) nbRangs++;
        if (globalSave->isExpertRush()) nbRangs++;
        os << nbRangs << " / 3";
        text = new Text(os.str());
        text->displayOnSurface(44+208+6, 152, surface);
        delete text;
    } else {
        cadre(16, 48+16, 288, 32, surface);
        cadre(16, 144-16, 288, 32, surface);
        cadre(16, 192, 136, 32, surface);

        text = TextManager::getInstance()->getText(690);
        text->displayOnSurface(44, 56+16, surface);
        delete text;

        ostringstream os;
        int score = globalSave->getScoreArc();
        if (score < 10) os << "0";
        os << score << " / 50";
        text = new Text(os.str());
        text->displayOnSurface(44+208, 56+16, surface);
        delete text;

        text = TextManager::getInstance()->getText(691);
        text->displayOnSurface(44, 104+32, surface);
        delete text;

        os.str("");
        int nbRangs = 0;
        if (globalSave->isNormalFull()) nbRangs++;
        if (globalSave->isNormalUlti()) nbRangs++;
        if (globalSave->isNormalRush()) nbRangs++;
        os << nbRangs << " / 3";
        text = new Text(os.str());
        text->displayOnSurface(44+208+6, 104+32, surface);
        delete text;
    }
    text = TextManager::getInstance()->getText(694);
    size = text->getLength() * letterSize;
    text->displayOnSurface(16 + (136 - size) / 2, 200, surface);
    delete text;

    delete surfaceArc;
    surfaceArc = new Surface(192, 56);
    cadre(0, 0, 192, 56, surfaceArc);
    text = TextManager::getInstance()->getText(695);
    size = text->getLength() * letterSize;
    text->displayOnSurface((192 - size) / 2, 9+2, surfaceArc);
    delete text;
    text = TextManager::getInstance()->getText(696);
    size = text->getLength() * letterSize;
    text->displayOnSurface((192 - size) / 2, 9+2+16, surfaceArc);
    delete text;
}

void GlobalRecords::cadre(int x, int y, int w, int h, Surface* surf) {

    // center
    for (int j = y + 16; j < y + h - 16; j += 16) {
        for (int i = x + 16; i < x + w - 16; i += 16) {
            surf->put(image, i, j, 16, 64, 16, 16);
        }
    }

    //top and bottom
    for (int i = x + 16; i < x + w - 16; i += 16) {
        surf->put(image, i, y, 16, 48, 16, 16);
        surf->put(image, i, y + h - 16, 16, 80, 16, 16);
    }

    //left and right
    for (int j = y + 16; j < y + h - 16; j += 16) {
        surf->put(image, x, j, 0, 64, 16, 16);
        surf->put(image, x + w - 16, j, 32, 64, 16, 16);
    }

    //top left corner
    surf->put(image, x, y, 0, 48, 16, 16);

    //top right corner
    surf->put(image, x + w - 16, y, 32, 48, 16, 16);

    //bottom left corner
    surf->put(image, x, y + h - 16, 0, 80, 16, 16);

    //bottom right corner
    surf->put(image, x + w - 16, y + h - 16, 32, 80, 16, 16);
}

void GlobalRecords::handleEvents(Event* event) {
    if (event->isPushed(kReturn)) {

        if (line == 3 || (line == 2 && !globalSave->isExpertUnlocked())) {

            MainController::getInstance()->getMenuController()->setStep(MENU_MAIN);
            AudioManager::getInstance()->playSound(TS_MENU2);

        } else {

            if (line == 0) {
                if (tir) {
                    tir = false;
                    AudioManager::getInstance()->playSound(TS_MENU2);
                } else {
                    tir = true;
                    AudioManager::getInstance()->playSound(TS_MENU1);
                }
            } else if (line == 1) {
                MainController::getInstance()->getMenuController()->getRecordsController()->setStep(RECORDS_NORMAL);
                AudioManager::getInstance()->playSound(TS_MENU1);
            } else if (line == 2) {
                MainController::getInstance()->getMenuController()->getRecordsController()->setStep(RECORDS_EXPERT);
                AudioManager::getInstance()->playSound(TS_MENU1);
            }
        }
        return;
    }

    if (event->isPushed(kUp)) {
        line--;
        if (line < 0) line = 3;
        if (line == 3 && !globalSave->isExpertUnlocked()) line = 2;
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
    if (event->isPushed(kDown)) {
        line++;
        if (line > 3 || (line > 2 && !globalSave->isExpertUnlocked())) line = 0;
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
}

void GlobalRecords::draw() {
    WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);

    if (globalSave->isExpertUnlocked()) {
        WindowManager::getInstance()->draw(link, skin * 16, 0, 16, 21, 26, 53 + 48 * line);
    } else {
        WindowManager::getInstance()->draw(link, skin * 16, 0, 16, 21, 26, 53 + 16 + 64 * line);
    }

    if (tir) {
        WindowManager::getInstance()->draw(surfaceArc->toImage(), 0, 0, 192, 56, 64, 92);
    }
}
