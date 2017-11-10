#include "MainMenu.h"

#include "../../MainController.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/audio/AudioManager.h"
#include "../../../engine/texts/TextManager.h"
#include "../../../config/ConfigurationManager.h"

MainMenu::MainMenu() : line(0), column(0), load(false), confirm(false), expert(false), subLine(0),
    surface(0), surfaceSave(0), surfaceConfirm(0), surfaceExpert(0), skin(0) {

    image = ResourceManager::getInstance()->loadImage("data/images/menus/cadres.png");
    link = ResourceManager::getInstance()->loadImage("data/images/menus/curseur.png", true);
    status = ResourceManager::getInstance()->loadImage("data/images/status/status.png", true);
    fairy = ResourceManager::getInstance()->loadImage("data/images/menus/fee.png", true);
    medaillons = ResourceManager::getInstance()->loadImage("data/images/status/medaillons.png", true);
    rangs = ResourceManager::getInstance()->loadImage("data/images/menus/rangs.png");

    for (int i = 0; i < 3; i++) {
        saves[i] = new Save(i + 1);
    }
    //saves[0]->save();
}

MainMenu::~MainMenu() {
    ResourceManager::getInstance()->free(image);
    ResourceManager::getInstance()->free(link);
    ResourceManager::getInstance()->free(status);
    ResourceManager::getInstance()->free(fairy);
    ResourceManager::getInstance()->free(medaillons);
    ResourceManager::getInstance()->free(rangs);
    delete surface;
    delete surfaceSave;
    delete surfaceConfirm;
    delete surfaceExpert;

    for (int i = 0; i < 3; i++) {
        delete saves[i];
    }
}

void MainMenu::init() {
    delete surface;
    surface = new Surface(320, 240);

    skin = ConfigurationManager::getInstance()->getSkin();

    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 20; i++) {
            surface->put(image, i * 16, j * 16, 16, 16, 16, 16);
        }
    }

    // title
    int letterSize = TextManager::getInstance()->getWSpace();
    Text* text = TextManager::getInstance()->getText(1);
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

    cadre(16, 48, 288, 32, surface);
    cadre(16, 96, 288, 32, surface);
    cadre(16, 144, 288, 32, surface);
    cadre(16, 192, 136, 32, surface);
    cadre(168, 192, 136, 32, surface);

    text = new Text("1.");
    text->displayOnSurface(44, 56, surface);
    delete text;

    text = new Text("2.");
    text->displayOnSurface(44, 104, surface);
    delete text;

    text = new Text("3.");
    text->displayOnSurface(44, 152, surface);
    delete text;

    // options
    text = TextManager::getInstance()->getText(2);
    size = text->getLength() * letterSize;
    text->displayOnSurface(16 + (136 - size) / 2, 200, surface);
    delete text;

    // records
    text = TextManager::getInstance()->getText(3);
    size = text->getLength() * letterSize;
    text->displayOnSurface(168 + (136 - size) / 2, 200, surface);
    delete text;

    // saves
    for (int i = 0; i < 3; i++) {
        Save* save = saves[i];
        if (save->isLoaded()) {
            // life
            for(int j = 0; j < save->getMaxLife()/2; j++) {

                int x = 0;
                int l = save->getLife() - (j * 2);
                if (l > 1) x = 141;
                if (l == 1) x = 149;
                if (l < 1) x = 157;

                surface->put(status, 64 + ((j % 10) * 8) + 8,
                             57 + 8 * ((int)(j / 10)) + i * 48, x, 9, 7, 7);
            }

            if (save->getAvancement() >= AV_MAIRE_VU) {
                for (int j = 0; j < 7; j++) {
                    if (save->getMedaillons(j)) {
                        surface->put(medaillons, 160 + j*16, 56 + i * 48, j*16, 0, 16, 16);
                    } else {
                        surface->put(medaillons, 160 + j*16, 56 + i * 48, 112, 0, 16, 16);
                    }
                }
            }

            if (save->getCoffre(9, 28)) {
                surface->put(rangs, 280, 56 + i * 48, 0, 0, 16, 16);
            }

            if (save->isExpert()) {
                surface->put(rangs, 56, 56 + i * 48, 32, 0, 16, 16);
            }
        }
    }


    //init save widget
    delete surfaceSave;
    surfaceSave = new Surface(112, 72);
    cadre(0, 0, 112, 72, surfaceSave);
    // load
    text = TextManager::getInstance()->getText(4);
    size = text->getLength() * letterSize;
    text->displayOnSurface((112 - size) / 2, 9, surfaceSave);
    delete text;

    // erase
    text = TextManager::getInstance()->getText(5);
    size = text->getLength() * letterSize;
    text->displayOnSurface((112 - size) / 2, 29, surfaceSave);
    delete text;

    // cancel
    text = TextManager::getInstance()->getText(6);
    size = text->getLength() * letterSize;
    text->displayOnSurface((112 - size) / 2, 49, surfaceSave);
    delete text;



    // init confirm widget
    delete surfaceConfirm;
    surfaceConfirm = new Surface(112, 72);
    cadre(0, 0, 112, 72, surfaceConfirm);
    // delete
    text = TextManager::getInstance()->getText(7);
    size = text->getLength() * letterSize;
    text->displayOnSurface((112 - size) / 2, 9, surfaceConfirm);
    delete text;

    // yes
    text = TextManager::getInstance()->getText(8);
    size = text->getLength() * letterSize;
    text->displayOnSurface((112 - size) / 2, 29, surfaceConfirm);
    delete text;

    // no
    text = TextManager::getInstance()->getText(9);
    size = text->getLength() * letterSize;
    text->displayOnSurface((112 - size) / 2, 49, surfaceConfirm);
    delete text;



    // init expert widget
    delete surfaceExpert;
    surfaceExpert = new Surface(112, 72);
    cadre(0, 0, 112, 72, surfaceExpert);
    // normal
    text = TextManager::getInstance()->getText(698);
    size = text->getLength() * letterSize;
    text->displayOnSurface((112 - size) / 2, 9, surfaceExpert);
    delete text;

    // expert
    text = TextManager::getInstance()->getText(699);
    size = text->getLength() * letterSize;
    text->displayOnSurface((112 - size) / 2, 29, surfaceExpert);
    delete text;
    surfaceExpert->put(rangs, 88, 29, 32, 0, 16, 16);

    // cancel
    text = TextManager::getInstance()->getText(6);
    size = text->getLength() * letterSize;
    text->displayOnSurface((112 - size) / 2, 49, surfaceExpert);
    delete text;



    AudioManager::getInstance()->playMusic(1);

    load = false;
    confirm = false;
    expert = false;
}

void MainMenu::cadre(int x, int y, int w, int h, Surface* surf) {

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

void MainMenu::handleEvents(Event* event) {
    if (event->isPushed(kReturn)) {

        if (line == 3) {

            if (column == 0) {
                MainController::getInstance()->getMenuController()->setStep(MENU_OPTIONS);
            } else {
                MainController::getInstance()->getMenuController()->setStep(MENU_RECORDS);
            }
            AudioManager::getInstance()->playSound(TS_MENU1);

        } else {

            if (confirm) {

                if (subLine == 0) {
                    saves[line]->erase();
                    init();
                    load = false;
                }
                confirm = false;
                AudioManager::getInstance()->playSound(TS_MENU2);

            } else if (load) {

                if (subLine == 0) {
                    AudioManager::getInstance()->playSound(TS_MENU1);
                    AudioManager::getInstance()->stopMusic();
                    MainController::getInstance()->getGameController()->setSave(saves[line]);
                    MainController::getInstance()->setStep(GAME);
                } else if (subLine == 1) {
                    confirm = true;
                    AudioManager::getInstance()->playSound(TS_MENU1);
                } else if (subLine == 2) {
                    load = false;
                    AudioManager::getInstance()->playSound(TS_MENU2);
                }

            } else if (expert) {

                if (subLine == 2) {
                    expert = false;
                    AudioManager::getInstance()->playSound(TS_MENU2);
                } else {
                    saves[line]->setExpert(subLine == 1);
                    AudioManager::getInstance()->stopMusic();
                    MainController::getInstance()->getGameController()->setSave(saves[line]);
                    MainController::getInstance()->setStep(OPENING);
                }

            } else {
                if (saves[line]->isLoaded()) {
                    load = true;
                    subLine = 0;
                } else {
                    if (GlobalSave::getInstance()->isExpertUnlocked()) {
                        expert = true;
                        subLine = 0;
                    } else {
                        AudioManager::getInstance()->stopMusic();
                        MainController::getInstance()->getGameController()->setSave(saves[line]);
                        MainController::getInstance()->setStep(OPENING);
                    }
                }
                AudioManager::getInstance()->playSound(TS_MENU1);
            }
        }
        return;
    }

    if (event->isPushed(kUp)) {
        if (confirm) {
            subLine--;
            if (subLine < 0) subLine = 1;
        } else if (load || expert) {
            subLine--;
            if (subLine < 0) subLine = 2;
        } else {
            line--;
            if (line < 0) line = 3;
            column = 0;
        }
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
    if (event->isPushed(kDown)) {
        if (confirm) {
            subLine++;
            if (subLine > 1) subLine = 0;
        } else if (load || expert) {
            subLine++;
            if (subLine > 2) subLine = 0;
        } else {
            line++;
            if (line > 3) line = 0;
            column = 0;
        }
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
    if (!load && !confirm && !expert && event->isPushed(kLeft) && line == 3) {
        column--;
        if (column < 0) column = 1;
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
    if (!load && !confirm && !expert && event->isPushed(kRight) && line == 3) {
        column++;
        if (column > 1) column = 0;
        AudioManager::getInstance()->playSound(TS_MENU3);
    }
}

void MainMenu::draw() {
    WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);

    WindowManager::getInstance()->draw(link, skin * 16, 0, 16, 21, 26 + 152 * column, 53 + 48 * line);

    if (load) {
        WindowManager::getInstance()->draw(surfaceSave->toImage(), 0, 0, 112, 72, 104, 84);
        WindowManager::getInstance()->draw(fairy, 0, 0, 16, 16, 112, 93 + 20 * subLine);
    }
    if (confirm) {
        WindowManager::getInstance()->draw(surfaceConfirm->toImage(), 0, 0, 112, 72, 104, 84);
        WindowManager::getInstance()->draw(fairy, 0, 0, 16, 16, 112, 113 + 20 * subLine);
    }
    if (expert) {
        WindowManager::getInstance()->draw(surfaceExpert->toImage(), 0, 0, 112, 72, 104, 84);
        WindowManager::getInstance()->draw(fairy, 0, 0, 16, 16, 112, 93 + 20 * subLine);
    }
}
