#include "Map.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

#include "../helper/MapHelper.h"
#include "../helper/ProjectileHelper.h"

#include "../effects/FumeeBlanche.h"

#include "../objects/Interrupteur.h"
#include "../objects/Switch.h"
#include "../effects/Debris.h"
#include "../effects/Feuilles.h"
#include "../objects/Pierre.h"
#include "../ennemis/Ennemi001.h"
#include "../ennemis/Ennemi102.h"
#include "../effects/Plouf.h"
#include "../objects/Pnj.h"
#include "../objects/Coffre.h"
#include "../objects/Jarre.h"
#include "../objects/Stele.h"
#include "../objects/Poule.h"

Map::Map(int level, Metronome* metronome) : metronome(metronome), map(level),
w(0), h(0), music(0), stones(0), inters(0), eau1Up(true), eau2Up(true), effects(0),
ennemis(0), items(0), projectiles(0), objects(0), oldMetronomeValue(0) {

    ostringstream os;
    for (int i = 0; i < NB_IMAGES; i++) {
        os << (i + 1);
        string filename = "data/images/tileset/image" + os.str() + ".png";
        images[i] = ResourceManager::getInstance()->loadImage(filename, true);
        os.str("");
    }

    initializer = new MapInitializer(this);
}

Map::~Map() {
    for (int i = 0; i < NB_IMAGES; i++) {
        ResourceManager::getInstance()->free(images[i]);
    }
    delete effects;
    delete ennemis;
    delete items;
    delete projectiles;
    delete objects;
    delete initializer;
}

void Map::load() {

    for (int j = 0; j < MAX_CASE_Y; j++) {
        for (int i = 0; i < MAX_CASE_X; i++) {
            sol[i][j] = -1;
            air[i][j] = -1;
            mur[i * 2][j * 2] = HERBE;
            mur[i * 2 + 1][j * 2] = HERBE;
            mur[i * 2][j * 2 + 1] = HERBE;
            mur[i * 2 + 1][j * 2 + 1] = HERBE;
        }
    }

    loadFromFile();


    bounds.setX(0);
    bounds.setY(0);
    bounds.setW(w);
    bounds.setH(h);

    delete effects;
    effects = new Quadtree();
    effects->setBox(0, 0, w, h);

    delete ennemis;
    ennemis = new Quadtree();
    ennemis->setBox(0, 0, w, h);

    delete items;
    items = new Quadtree();
    items->setBox(0, 0, w, h);

    delete projectiles;
    projectiles = new Quadtree();
    projectiles->setBox(0, 0, w, h);

    delete objects;
    objects = new Quadtree();
    objects->setBox(0, 0, w, h);

    init();
}

void Map::loadFromFile() {
    ostringstream oss;
    oss << map;
    string result = "data/maps/map" + oss.str() + ".dat";
    ifstream file(result.c_str());

    file >> w;
    file >> h;
    file >> music;

    for (int i=0; i<w/16; i++){
        for (int j=0; j<h/16; j++){
            file >> sol[i][j];
            file >> mur[i*2][j*2];
            file >> mur[i*2+1][j*2];
            file >> mur[i*2][j*2+1];
            file >> mur[i*2+1][j*2+1];
        }
    }

    int cpt = 0;
    int a;
    int b;

    file >> cpt;
    for (int i=0; i<cpt; i++) {
        file >> a;
        file >> b;
        file >> air[a][b];
    }

    file.close();
}

int Map::getW() {
    return w;
}

int Map::getH() {
    return h;
}

int Map::getId() {
    return map;
}

void Map::launch() {

    int musicToPlay = music;

    int special = MainController::getInstance()->getGameController()->getSceneController()
                                    ->getScene()->getAnimationInGame()->getSpecialMusicId();

    if (special > 0) {
        musicToPlay = special;
    } else {
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        switch (map) {
            case 16 : if (scene->getFees(1)) musicToPlay = 1; break;
            case 17 : if (scene->getFees(2)) musicToPlay = 1; break;
            case 18 : if (scene->getFees(3)) musicToPlay = 1; break;
            case 19 : if (scene->getFees(4)) musicToPlay = 1; break;
            case 24 : if (scene->getFees(5)) musicToPlay = 1; break;
            case 25 : if (scene->getFees(6)) musicToPlay = 1; break;
            case 28 : if (scene->getFees(7)) musicToPlay = 1; break;
            case 65 : if (bounds.getX() == 320 * 2 && bounds.getY() == 0 && scene->getCoffre(8, 25) && scene->getLink()->getEpee() < 5) musicToPlay = 1; break;
            case 66 : if (bounds.getX() >= 320 * 8) musicToPlay = 37; break;
            default : break;
        }
    }

    AudioManager::getInstance()->playMusic(musicToPlay);

    if (map == 63 || map == 13) {
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        Link* link = scene->getLink();
        if (!link->getInventory()->hasObject(PERLE_LUNE)) {
            link->setLapin(map == 63);
        }
    }

    if (map == 30) {
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        Link* link = scene->getLink();
        link->getStatus()->resetOniLife();
    }

    launchRoom();
}

void Map::launchRoom() {
    if (map >= 15) { // indoor only
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
        Link* link = scene->getLink();


        bounds.setX(320 * (link->getX() / 320));
        bounds.setY(240 * (link->getY() / 240));
        bounds.setW(320);
        bounds.setH(240);

        // handle big rooms
        switch (map) {
            case 25 :
                if (bounds.getX() == 320) {
                    bounds.setY(240);
                    bounds.setH(480);
                }
                break;
            case 44 :
                bounds.setY(0);
                bounds.setH(480);
                break;
            case 58 :
                if (bounds.getX() == 320 * 14 && bounds.getY() == 240 * 2) {
                    AudioManager::getInstance()->playMusic(28);
                }
                if (bounds.getX() >= 320 && bounds.getX() <= 320*3
                    && bounds.getY() >= 240 && bounds.getY() <= 240*2) {
                    bounds.setX(320);
                    bounds.setY(240);
                    bounds.setW(960);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*6 && bounds.getX() <= 320*8
                    && bounds.getY() >= 240 && bounds.getY() <= 240*2) {
                    bounds.setX(320*6);
                    bounds.setY(240);
                    bounds.setW(960);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*11 && bounds.getX() <= 320*13
                    && bounds.getY() >= 240 && bounds.getY() <= 240*2) {
                    bounds.setX(320*11);
                    bounds.setY(240);
                    bounds.setW(960);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*3 && bounds.getX() <= 320*4 && bounds.getY() == 240*3) {
                    bounds.setX(320*3);
                    bounds.setW(640);
                    break;
                }
                if ((bounds.getX() == 320*4 || bounds.getX() == 320*9 || bounds.getX() == 320*10) && bounds.getY() >= 240 && bounds.getY() <= 240*2) {
                    bounds.setY(240);
                    bounds.setH(480);
                    break;
                }
                if ((bounds.getX() == 320*7 || bounds.getX() == 320*10 || bounds.getX() == 320*12) && bounds.getY() >= 240*3 && bounds.getY() <= 240*4) {
                    bounds.setY(240*3);
                    bounds.setH(480);
                    break;
                }
                break;
            case 59 :
                if (bounds.getX() == 320 * 3 && bounds.getY() == 240 * 3) {
                    AudioManager::getInstance()->playMusic(29);
                }
                if (bounds.getX() >= 320 && bounds.getX() <= 320*2
                    && bounds.getY() >= 240*6 && bounds.getY() <= 240*7) {
                    bounds.setX(320);
                    bounds.setY(240*6);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320 && bounds.getX() <= 320*2
                    && bounds.getY() >= 240*11 && bounds.getY() <= 240*12) {
                    bounds.setX(320);
                    bounds.setY(240*11);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*5 && bounds.getX() <= 320*6
                    && bounds.getY() >= 240*7 && bounds.getY() <= 240*8) {
                    bounds.setX(320*5);
                    bounds.setY(240*7);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*5 && bounds.getX() <= 320*6
                    && bounds.getY() >= 240*12 && bounds.getY() <= 240*13) {
                    bounds.setX(320*5);
                    bounds.setY(240*12);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*4 && bounds.getX() <= 320*5 && bounds.getY() == 0) {
                    bounds.setX(320*4);
                    bounds.setW(640);
                    break;
                }
                if (bounds.getX() >= 320*5 && bounds.getX() <= 320*6 && (bounds.getY() == 240*9 || bounds.getY() == 240*11 || bounds.getY() == 240*14)) {
                    bounds.setX(320*5);
                    bounds.setW(640);
                    break;
                }
                if (bounds.getX() == 0 && bounds.getY() >= 240*3 && bounds.getY() <= 240*4) {
                    bounds.setY(240*3);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() == 320 && bounds.getY() >= 240 && bounds.getY() <= 240*2) {
                    bounds.setY(240);
                    bounds.setH(480);
                    break;
                }
                if ((bounds.getX() == 320*2 || bounds.getX() == 320*4) && bounds.getY() >= 240*13 && bounds.getY() <= 240*14) {
                    bounds.setY(240*13);
                    bounds.setH(480);
                    break;
                }
                break;
            case 60 :
                if (bounds.getX() == 320 * 2 && bounds.getY() == 240 * 5) {
                    AudioManager::getInstance()->playMusic(30);
                }
                if (bounds.getX() >= 0 && bounds.getX() <= 320
                    && bounds.getY() >= 240*4 && bounds.getY() <= 240*5) {
                    bounds.setX(0);
                    bounds.setY(240*4);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*3 && bounds.getX() <= 320*4
                    && bounds.getY() >= 240*4 && bounds.getY() <= 240*5) {
                    bounds.setX(320*3);
                    bounds.setY(240*4);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*5 && bounds.getX() <= 320*6
                    && bounds.getY() >= 240 && bounds.getY() <= 240*2) {
                    bounds.setX(320*5);
                    bounds.setY(240);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*5 && bounds.getX() <= 320*6
                    && bounds.getY() >= 240*6 && bounds.getY() <= 240*7) {
                    bounds.setX(320*5);
                    bounds.setY(240*6);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*7 && bounds.getX() <= 320*8
                    && bounds.getY() >= 240*3 && bounds.getY() <= 240*4) {
                    bounds.setX(320*7);
                    bounds.setY(240*3);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*7 && bounds.getX() <= 320*8
                    && bounds.getY() >= 240*5 && bounds.getY() <= 240*6) {
                    bounds.setX(320*7);
                    bounds.setY(240*5);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*9 && bounds.getX() <= 320*10
                    && bounds.getY() >= 240*4 && bounds.getY() <= 240*5) {
                    bounds.setX(320*9);
                    bounds.setY(240*4);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*10 && bounds.getX() <= 320*11
                    && bounds.getY() >= 240 && bounds.getY() <= 240*2) {
                    bounds.setX(320*10);
                    bounds.setY(240);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if ((bounds.getX() == 320*2 || bounds.getX() == 320*4) && bounds.getY() >= 240*6 && bounds.getY() <= 240*7) {
                    bounds.setY(240*6);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() == 320*11 && bounds.getY() >= 240*5 && bounds.getY() <= 240*6) {
                    bounds.setY(240*5);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*8 && bounds.getX() <= 320*9 && bounds.getY() == 240*7) {
                    bounds.setX(320*8);
                    bounds.setW(640);
                    break;
                }
                break;
            case 61 :
                if (bounds.getX() % (320 * 7) == 320 * 3 && bounds.getY() == 240 * 2) {
                    AudioManager::getInstance()->playMusic(31);
                }
                if (bounds.getX() % (320 * 7) >= 320 && bounds.getX() % (320 * 7) <= 320*2
                    && bounds.getY() >= 0 && bounds.getY() <= 240) {
                    bounds.setX(320 + 320*7*(bounds.getX()/(320*7)));
                    bounds.setY(0);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() % (320 * 7) >= 320 && bounds.getX() % (320 * 7) <= 320*2
                    && bounds.getY() >= 240*3 && bounds.getY() <= 240*4) {
                    bounds.setX(320 + 320*7*(bounds.getX()/(320*7)));
                    bounds.setY(240*3);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() % (320 * 7) >= 320*5 && bounds.getX() % (320 * 7) <= 320*6
                    && bounds.getY() >= 240*3 && bounds.getY() <= 240*4) {
                    bounds.setX(320*5 + 320*7*(bounds.getX()/(320*7)));
                    bounds.setY(240*3);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() % (320 * 7) >= 0 && bounds.getX() % (320 * 7) <= 320*1
                    && bounds.getY() >= 240*5 && bounds.getY() <= 240*6) {
                    bounds.setX(320*7*(bounds.getX()/(320*7)));
                    bounds.setY(240*5);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() % (320 * 7) >= 320*5 && bounds.getX() % (320 * 7) <= 320*6
                    && bounds.getY() >= 240*5 && bounds.getY() <= 240*6) {
                    bounds.setX(320*5 + 320*7*(bounds.getX()/(320*7)));
                    bounds.setY(240*5);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() % (320 * 7) >= 320*2 && bounds.getX() % (320 * 7) <= 320*3
                    && bounds.getY() >= 240*6 && bounds.getY() <= 240*7) {
                    bounds.setX(320*2 + 320*7*(bounds.getX()/(320*7)));
                    bounds.setY(240*6);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() % (320 * 7) >= 0 && bounds.getX() % (320 * 7) <= 320*1
                    && bounds.getY() >= 240*8 && bounds.getY() <= 240*9) {
                    bounds.setX(320*7*(bounds.getX()/(320*7)));
                    bounds.setY(240*8);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() % (320 * 7) >= 320*5 && bounds.getX() % (320 * 7) <= 320*6
                    && bounds.getY() >= 240*8 && bounds.getY() <= 240*9) {
                    bounds.setX(320*5 + 320*7*(bounds.getX()/(320*7)));
                    bounds.setY(240*8);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() % (320 * 7) == 320*4 && bounds.getY() >= 0 && bounds.getY() <= 240) {
                    bounds.setY(0);
                    bounds.setH(480);
                    break;
                }
                if ((bounds.getX() % (320 * 7) == 0 || bounds.getX() % (320 * 7) == 320*4) && bounds.getY() >= 240*3 && bounds.getY() <= 240*4) {
                    bounds.setY(240*3);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() % (320 * 7) == 320*3 && bounds.getY() >= 240*10 && bounds.getY() <= 240*11) {
                    bounds.setY(240*10);
                    bounds.setH(480);
                    break;
                }
                break;
            case 62 :
                if (bounds.getX() == 320 * 2 && bounds.getY() == 240 * 3) {
                    AudioManager::getInstance()->playMusic(32);
                }
                if (bounds.getX() >= 320*2 && bounds.getX() <= 320*3
                    && bounds.getY() >= 240*6 && bounds.getY() <= 240*8) {
                    bounds.setX(320*2);
                    bounds.setY(240*6);
                    bounds.setW(640);
                    bounds.setH(720);
                    break;
                }
                if (bounds.getX() >= 0 && bounds.getX() <= 320
                    && bounds.getY() >= 240 && bounds.getY() <= 240*2) {
                    bounds.setX(0);
                    bounds.setY(240);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*3 && bounds.getX() <= 320*4
                    && bounds.getY() >= 0 && bounds.getY() <= 240) {
                    bounds.setX(320*3);
                    bounds.setY(0);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*4 && bounds.getX() <= 320*5
                    && bounds.getY() >= 240*2 && bounds.getY() <= 240*3) {
                    bounds.setX(320*4);
                    bounds.setY(240*2);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*4 && bounds.getX() <= 320*5
                    && bounds.getY() >= 240*7 && bounds.getY() <= 240*8) {
                    bounds.setX(320*4);
                    bounds.setY(240*7);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320 && bounds.getX() <= 320*2
                    && bounds.getY() >= 240*10 && bounds.getY() <= 240*11) {
                    bounds.setX(320);
                    bounds.setY(240*10);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*4 && bounds.getX() <= 320*5
                    && bounds.getY() >= 240*10 && bounds.getY() <= 240*11) {
                    bounds.setX(320*4);
                    bounds.setY(240*10);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*2 && bounds.getX() <= 320*3
                    && bounds.getY() >= 240*12 && bounds.getY() <= 240*13) {
                    bounds.setX(320*2);
                    bounds.setY(240*12);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if ((bounds.getX() == 0 || bounds.getX() == 320*3) && bounds.getY() >= 240*3 && bounds.getY() <= 240*4) {
                    bounds.setY(240*3);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() == 320*4 && bounds.getY() >= 240*12 && bounds.getY() <= 240*13) {
                    bounds.setY(240*12);
                    bounds.setH(480);
                    break;
                }
                break;
            case 63 :
                if (bounds.getX() == 320 * 2 && bounds.getY() == 240 * 2) {
                    AudioManager::getInstance()->playMusic(33);
                }
                if (bounds.getX() >= 0 && bounds.getX() <= 320
                    && bounds.getY() >= 0 && bounds.getY() <= 240) {
                    bounds.setX(0);
                    bounds.setY(0);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*4 && bounds.getX() <= 320*5
                    && bounds.getY() >= 240 && bounds.getY() <= 240*2) {
                    bounds.setX(320*4);
                    bounds.setY(240);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*4 && bounds.getX() <= 320*5
                    && bounds.getY() >= 240*6 && bounds.getY() <= 240*7) {
                    bounds.setX(320*4);
                    bounds.setY(240*6);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*4 && bounds.getX() <= 320*5
                    && bounds.getY() >= 240*10 && bounds.getY() <= 240*11) {
                    bounds.setX(320*4);
                    bounds.setY(240*10);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*2 && bounds.getX() <= 320*3
                    && bounds.getY() >= 240*11 && bounds.getY() <= 240*12) {
                    bounds.setX(320*2);
                    bounds.setY(240*11);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*4 && bounds.getX() <= 320*5
                    && bounds.getY() >= 240*12 && bounds.getY() <= 240*13) {
                    bounds.setX(320*4);
                    bounds.setY(240*12);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320 && bounds.getX() <= 320*2
                    && bounds.getY() >= 240*13 && bounds.getY() <= 240*14) {
                    bounds.setX(320);
                    bounds.setY(240*13);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() == 320 && bounds.getY() >= 240*3 && bounds.getY() <= 240*4) {
                    bounds.setY(240*3);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320*3 && bounds.getX() <= 320*4 && bounds.getY() == 240*4) {
                    bounds.setX(320*3);
                    bounds.setW(640);
                    break;
                }
                if (bounds.getX() >= 320 && bounds.getX() <= 320*2 && bounds.getY() == 240*5) {
                    bounds.setX(320);
                    bounds.setW(640);
                    break;
                }
                break;
            case 64 :
                if (bounds.getX() == 320 * 3 && bounds.getY() == 240 * 2) {
                    AudioManager::getInstance()->playMusic(34);
                }
                if (bounds.getX() >= 320 * 2 && bounds.getX() <= 320 * 4
                    && bounds.getY() >= 240 * 2 && bounds.getY() <= 240 * 3) {
                    bounds.setX(320 * 2);
                    bounds.setY(240 * 2);
                    bounds.setW(960);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320 * 2 && bounds.getX() <= 320 * 4
                    && bounds.getY() >= 240 * 7 && bounds.getY() <= 240 * 8) {
                    bounds.setX(320 * 2);
                    bounds.setY(240 * 7);
                    bounds.setW(960);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320 * 12 && bounds.getX() <= 320 * 13
                    && bounds.getY() >= 0 && bounds.getY() <= 240) {
                    bounds.setX(320 * 12);
                    bounds.setY(0);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() == 320 * 8 && bounds.getY() >= 240 && bounds.getY() <= 240 * 2) {
                    bounds.setY(240);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() == 320 && bounds.getY() >= 240 * 5 && bounds.getY() <= 240 * 6) {
                    bounds.setY(240 * 5);
                    bounds.setH(480);
                    break;
                }
                if ((bounds.getX() == 320 * 8 || bounds.getX() == 320 * 12) && bounds.getY() >= 240 * 6 && bounds.getY() <= 240 * 7) {
                    bounds.setY(240 * 6);
                    bounds.setH(480);
                    break;
                }
                if ((bounds.getX() == 320 * 5 || bounds.getX() == 320 * 13) && bounds.getY() >= 240 * 7 && bounds.getY() <= 240 * 8) {
                    bounds.setY(240 * 7);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320 * 2 && bounds.getX() <= 320 * 3 && bounds.getY() == 240 * 4) {
                    bounds.setX(320 * 2);
                    bounds.setW(640);
                    break;
                }
                if (bounds.getX() >= 320 * 10 && bounds.getX() <= 320 * 11 && bounds.getY() == 240 * 5) {
                    bounds.setX(320 * 10);
                    bounds.setW(640);
                    break;
                }
                break;
            case 65 :
                if (bounds.getX() == 320 * 2 && bounds.getY() == 240 * 2) {
                    AudioManager::getInstance()->playMusic(35);
                }
                if (bounds.getX() >= 320 * 1 && bounds.getX() <= 320 * 3
                    && bounds.getY() >= 240 * 2 && bounds.getY() <= 240 * 3) {
                    bounds.setX(320 * 1);
                    bounds.setY(240 * 2);
                    bounds.setW(960);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() == 320 && bounds.getY() <= 240) {
                    bounds.setY(0);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320 * 3 && bounds.getX() <= 320 * 4 && bounds.getY() == 240 * 4) {
                    bounds.setX(320 * 3);
                    bounds.setW(640);
                    break;
                }
                break;
            case 66 :
                if (bounds.getX() == 320 * 5 && bounds.getY() == 240 * 3) {
                    AudioManager::getInstance()->playMusic(36);
                }
                if (bounds.getX() == 320 * 9 && bounds.getY() == 240 * 3) {
                    AudioManager::getInstance()->playMusic(37);
                }
                if (bounds.getX() >= 320 && bounds.getX() <= 320 * 2
                    && bounds.getY() >= 240 && bounds.getY() <= 240 * 2) {
                    bounds.setX(320);
                    bounds.setY(240);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320 * 5 && bounds.getX() <= 320 * 6
                    && bounds.getY() >= 240 && bounds.getY() <= 240 * 2) {
                    bounds.setX(320 * 5);
                    bounds.setY(240);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() >= 320 * 10
                    && bounds.getY() >= 240 && bounds.getY() <= 240 * 2) {
                    bounds.setX(320 * 10);
                    bounds.setY(240);
                    bounds.setW(640);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() == 320 * 3 && bounds.getY() <= 240) {
                    bounds.setY(0);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() == 320 * 7 && bounds.getY() >= 240 * 2) {
                    bounds.setY(240 * 2);
                    bounds.setH(480);
                    break;
                }
                if (bounds.getX() == 320 * 8 && bounds.getY() >= 240 && bounds.getY() <= 240 * 2) {
                    bounds.setY(240);
                    bounds.setH(480);
                    break;
                }
                break;
            default : break;
        }

        if (map > 57) {
            for (int j = bounds.getY() / 240; j < (bounds.getY() + bounds.getH()) / 240; j++) {
                for (int i = bounds.getX() / 320; i < (bounds.getX() + bounds.getW()) / 320; i++) {
                    if (map == 61) {
                        scene->setRooms(3, i % 7, j, 1);
                    } else {
                        scene->setRooms(map - 58, i, j, 1);
                    }
                }
            }
        }

        testAnimRoom();
    }
}

void Map::resetRoom() {
    if (map >= 15) {

        objects->resetNodes(&bounds);
        ennemis->resetNodes(&bounds);

        List toKill;
        effects->get(&bounds, &toKill);
        items->get(&bounds, &toKill);
        projectiles->get(&bounds, &toKill);

        toKill.iterateOnFirst();
        while (toKill.hasNext()) {
            Node* n = (Node*) toKill.getNext();
            if (!n->isResetable()) {
                n->killNode();
            }
        }
        inters = 0;
    }
}

BoundingBox* Map::getBounds() {
    return &bounds;
}

void Map::reset() {
    load();
    launch();
}

void Map::init() {
    initializer->init();

    switch (map) {
        case 11 : stones = 0; break;
        case 43 : gameScore = -1; break;
        case 61 : eau1Up = true; eau2Up = true; break;
    }
}

void Map::activateInter(int x, int y, bool sound) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    switch (map) {
        case 25 :
            scene->setCoffre(0, 7, 1);
            if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
            ouvrePorte(9, 28, 0);
            break;
        case 28 :
            scene->setCoffre(0, 8, 1);
            if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
            ouvrePorte(18, 21, 1);
            break;
        case 58 :
            if (x == 125*16 && y == 53*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                setSol(127*16,51*16,6,4,1597,PIERRE);
                setSol(128*16,52*16,4,2,1412, PIERRE);
            }
            if (x == 127*16 && y == 36*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                setSol(129*16,36*16,2,1,1597,PIERRE);
            }
            if (x == 222*16 && y == 19*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                setSol(226*16,21*16,6,4,1597,PIERRE);
                setSol(227*16,22*16,4,2,1414, PIERRE);
            }
            if (x == 266*16 && y == 64*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_SHOOT);
                ProjectileHelper::getInstance()->addProjectile(TP_SPEAR, 266 * 16 + 4, 74 * 16 - 4, N);
                ProjectileHelper::getInstance()->addProjectile(TP_SPEAR, 279 * 16 - 6, 64 * 16 + 4, W);
            }
            if (x == 273*16 && y == 64*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_SHOOT);
                ProjectileHelper::getInstance()->addProjectile(TP_SPEAR, 273 * 16 + 4, 74 * 16 - 4, N);
                ProjectileHelper::getInstance()->addProjectile(TP_SPEAR, 260 * 16 + 6, 64 * 16 + 4, E);

            }
            if (x == 266*16 && y == 70*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_SHOOT);
                ProjectileHelper::getInstance()->addProjectile(TP_SPEAR, 266 * 16 + 4, 60 * 16 + 4, S);
                ProjectileHelper::getInstance()->addProjectile(TP_SPEAR, 279 * 16 - 6, 70 * 16 + 4, W);
            }
            if (x == 273*16 && y == 70*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_SHOOT);
                ProjectileHelper::getInstance()->addProjectile(TP_SPEAR, 273 * 16 + 4, 60 * 16 + 4, S);
                ProjectileHelper::getInstance()->addProjectile(TP_SPEAR, 260 * 16 + 6, 70 * 16 + 4, E);
            }
            if (x == 37*16 && y == 64*16) {
                scene->setCoffre(1, 18, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 66, 1);
            }
            if (x == 32*16 && y == 17*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                scene->setCoffre(1, 19, 1);
                int nb = 0;
                if (scene->getCoffre(1, 20)) nb++;
                if (scene->getCoffre(1, 21)) nb++;
                if (scene->getCoffre(1, 22)) nb++;
                if (scene->getCoffre(1, 23)) nb++;
                scene->getAnimationInGame()->startAnim(20 + nb);
            }
            if (x == 36*16 && y == 22*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                scene->setCoffre(1, 20, 1);
                int nb = 0;
                if (scene->getCoffre(1, 19)) nb++;
                if (scene->getCoffre(1, 21)) nb++;
                if (scene->getCoffre(1, 22)) nb++;
                if (scene->getCoffre(1, 23)) nb++;
                scene->getAnimationInGame()->startAnim(20 + nb);
            }
            if (x == 47*16 && y == 34*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                scene->setCoffre(1, 21, 1);
                int nb = 0;
                if (scene->getCoffre(1, 19)) nb++;
                if (scene->getCoffre(1, 20)) nb++;
                if (scene->getCoffre(1, 22)) nb++;
                if (scene->getCoffre(1, 23)) nb++;
                scene->getAnimationInGame()->startAnim(20 + nb);
            }
            if (x == 60*16 && y == 22*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                scene->setCoffre(1, 22, 1);
                int nb = 0;
                if (scene->getCoffre(1, 19)) nb++;
                if (scene->getCoffre(1, 20)) nb++;
                if (scene->getCoffre(1, 21)) nb++;
                if (scene->getCoffre(1, 23)) nb++;
                scene->getAnimationInGame()->startAnim(20 + nb);
            }
            if (x == 74*16 && y == 37*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                scene->setCoffre(1, 23, 1);
                int nb = 0;
                if (scene->getCoffre(1, 19)) nb++;
                if (scene->getCoffre(1, 20)) nb++;
                if (scene->getCoffre(1, 21)) nb++;
                if (scene->getCoffre(1, 22)) nb++;
                scene->getAnimationInGame()->startAnim(20 + nb);
            }
            if (x == 62*16 && y == 64*16) {
                scene->setCoffre(1, 24, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(58, 66, 1);
            }
            if (x == 88*16 && y == 38*16) {
                scene->setCoffre(1, 25, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 43, 0);
            }
            if (x == 227*16 && y == 62*16) {
                scene->setCoffre(1, 26, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(229, 58, 0);
            }
            if (x == 272*16 && y == 47*16) {
                scene->setCoffre(1, 27, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(278, 51, 1);
            }
            if (x == 292*16 && y == 62*16) {
                scene->setCoffre(1, 28, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(289, 58, 0);
            }
            if (x == 151*16 && y == 47*16) {
                scene->setCoffre(1, 29, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(158, 51, 1);
            }
            if (x == 204*16 && y == 67*16) {
                scene->setCoffre(1, 30, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(218, 66, 1);
            }
            break;
        case 59 :
            if (x == 136*16 && y == 37*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                setSol(127*16,34*16,6,5,1597,PIERRE);
                setSol(128*16,35*16,4,3,1645,PIERRE);
            }

            if (x == 65*16 && y == 124*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(69, 133, 0);
            }

            if (x == 37*16 && y == 220*16) {
                scene->setCoffre(2, 22, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 216, 1);
            }
            if (x == 12*16 && y == 177*16) {
                scene->setCoffre(2, 23, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 163, 0);
            }
            if (x == 32*16 && y == 47*16) {
                scene->setCoffre(2, 24, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 51, 1);
            }
            if (x == 32*16 && y == 62*16) {
                scene->setCoffre(2, 25, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(29, 58, 0);
            }
            if (x == 27*16 && y == 207*16) {
                scene->setCoffre(2, 26, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(18, 201, 1);
            }
            if (x == 47*16 && y == 12*16) {
                scene->setCoffre(2, 27, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 6, 1);
            }
            if (x == 62*16 && y == 55*16) {
                scene->setCoffre(2, 28, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(78, 51, 1);
            }
            if (x == 72*16 && y == 87*16) {
                scene->setCoffre(2, 29, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(78, 81, 1);
            }
            if (x == 82*16 && y == 55*16) {
                scene->setCoffre(2, 30, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(98, 51, 1);
            }
            if (x == 97*16 && y == 175*16) {
                scene->setCoffre(2, 31, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 163, 0);
            }
            if (x == 107*16 && y == 62*16) {
                scene->setCoffre(2, 32, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(98, 66, 1);
            }
            break;
        case 60 :
            // red
            if ((x == 7*16 || x == 12*16) && y == 62*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                inters++;
                if (inters == 2) {
                    activateAllInter();
                    scene->setCoffre(3, 20, 1);
                    if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(9, 58, 0);
                }
            }
            if (((x == 187*16 || x == 192*16) && y == 62*16)
                || (x == 195*16 && y == 83*16)
                || (x == 207*16 && (y == 79*16 || y == 85*16))) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                inters++;
                if (inters == 5) {
                    activateAllInter();
                    scene->setCoffre(3, 26, 1);
                    if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(189, 58, 0);
                }
            }

            // blue
            if (x == 215*16 && y == 79*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                setSol(211*16,73*16,1,4,1597,PIERRE);
            }

            if (x == 122*16 && y == 85*16) {
                scene->setCoffre(3, 21, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(118, 81, 1);
            }
            if (x == 157*16 && y == 40*16) {
                scene->setCoffre(3, 22, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(138, 36, 1);
            }
            if (x == 177*16 && y == 25*16) {
                scene->setCoffre(3, 23, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(178, 21, 1);
            }
            if (x == 177*16 && y == 70*16) {
                scene->setCoffre(3, 24, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(178, 66, 1);
            }
            if (x == 191*16 && y == 32*16) {
                scene->setCoffre(3, 25, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(198, 36, 1);
            }
            if (x == 225*16 && y == 38*16) {
                scene->setCoffre(3, 27, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(229, 43, 0);
            }
            break;
        case 61 :
            // blue
            if (x == 146*16 && y == 36*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(149, 28, 0);
            }
            if (x == 253*16 && y == 21*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                setSol(249*16,21*16,2,1,1597,PIERRE);
            }

            // inter
            if (x % (320*7) == 17*16 && y == 4*16) {
                scene->setCoffre(4, 26, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(18, 6, 1);
                ouvrePorte(158, 6, 1);

                List list;
                BoundingBox b1(17*16, 4*16, 16, 16);
                BoundingBox b2(157*16, 4*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Interrupteur* inter = dynamic_cast<Interrupteur*>(list.getNext());
                    if (inter != 0) {
                        inter->setUsed();
                    }
                }
            }
            if (x % (320*7) == 7*16 && y == 117*16) {
                scene->setCoffre(4, 27, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 103, 0);
                ouvrePorte(149, 103, 0);

                List list;
                BoundingBox b1(7*16, 117*16, 16, 16);
                BoundingBox b2(147*16, 117*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Interrupteur* inter = dynamic_cast<Interrupteur*>(list.getNext());
                    if (inter != 0) {
                        inter->setUsed();
                    }
                }
            }
            if (x == 166*16 && y == 23*16) {
                scene->setCoffre(4, 28, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(29, 28, 0);
                ouvrePorte(169, 28, 0);
            }
            if (x % (320*7) == 37*16 && y == 115*16) {
                scene->setCoffre(4, 29, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 111, 1);
                ouvrePorte(178, 111, 1);
                ouvrePorte(138, 156, 1);

                List list;
                BoundingBox b1(37*16, 115*16, 16, 16);
                BoundingBox b2(177*16, 115*16, 16, 16);
                BoundingBox b3(137*16, 160*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);
                objects->get(&b3, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Interrupteur* inter = dynamic_cast<Interrupteur*>(list.getNext());
                    if (inter != 0) {
                        inter->setUsed();
                    }
                }
            }
            if (x == 193*16 && y == 68*16) {
                scene->setCoffre(4, 30, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(58, 66, 1);
                ouvrePorte(198, 66, 1);
            }
            if (x == 57*16 && y == 175*16) {
                scene->setCoffre(4, 31, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(58, 171, 1);
            }
            if (x % (320*7) == 72*16 && y == 137*16) {
                scene->setCoffre(4, 32, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(69, 133, 0);
                ouvrePorte(209, 133, 0);

                List list;
                BoundingBox b1(72*16, 137*16, 16, 16);
                BoundingBox b2(212*16, 137*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Interrupteur* inter = dynamic_cast<Interrupteur*>(list.getNext());
                    if (inter != 0) {
                        inter->setUsed();
                    }
                }
            }
            if (x % (320*7) == 97*16 && y == 115*16) {
                scene->setCoffre(4, 33, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 118, 0);
                ouvrePorte(229, 118, 0);

                List list;
                BoundingBox b1(97*16, 115*16, 16, 16);
                BoundingBox b2(237*16, 115*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Interrupteur* inter = dynamic_cast<Interrupteur*>(list.getNext());
                    if (inter != 0) {
                        inter->setUsed();
                    }
                }
            }
            if (x == 82*16 && y == 175*16) {
                scene->setCoffre(4, 34, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(78, 171, 1);
            }
            if (x == 92*16 && y == 182*16) {
                scene->setCoffre(4, 35, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 178, 0);
            }
            if (x % (320*7) == 117*16 && y == 4*16) {
                scene->setCoffre(4, 36, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(98, 6, 1);
                ouvrePorte(238, 6, 1);
                ouvrePorte(118, 171, 1);

                List list;
                BoundingBox b1(117*16, 4*16, 16, 16);
                BoundingBox b2(257*16, 4*16, 16, 16);
                BoundingBox b3(137*16, 169*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);
                objects->get(&b3, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Interrupteur* inter = dynamic_cast<Interrupteur*>(list.getNext());
                    if (inter != 0) {
                        inter->setUsed();
                    }
                }
            }
            if (x % (320*7) == 112*16 && y == 32*16) {
                scene->setCoffre(4, 38, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(109, 28, 0);
                ouvrePorte(249, 28, 0);

                List list;
                BoundingBox b1(112*16, 32*16, 16, 16);
                BoundingBox b2(252*16, 32*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Interrupteur* inter = dynamic_cast<Interrupteur*>(list.getNext());
                    if (inter != 0) {
                        inter->setUsed();
                    }
                }
            }

            // switch
            if (x == 34*16 && y == 108*16) {
                List list;
                BoundingBox b1(174*16, 108*16, 16, 16);
                BoundingBox b2(134*16, 153*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Switch* inter = dynamic_cast<Switch*>(list.getNext());
                    if (inter != 0) {
                        inter->setLeft(false);
                    }
                }
                scene->getAnimationInGame()->startAnim(25);
                eau1Up = false;
            }
            if (x == 174*16 && y == 108*16) {
                List list;
                BoundingBox b1(34*16, 108*16, 16, 16);
                BoundingBox b2(134*16, 153*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Switch* inter = dynamic_cast<Switch*>(list.getNext());
                    if (inter != 0) {
                        inter->setLeft(true);
                    }
                }
                scene->getAnimationInGame()->startAnim(26);
                eau1Up = true;
            }
            if (x == 109*16+8 && y == 9*16+8) {
                List list;
                BoundingBox b1(249*16+8, 9*16, 16, 16);
                BoundingBox b2(129*16+8, 174*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Switch* inter = dynamic_cast<Switch*>(list.getNext());
                    if (inter != 0) {
                        inter->setLeft(false);
                    }
                }
                scene->getAnimationInGame()->startAnim(27);
                eau2Up = false;
            }
            if (x == 249*16+8 && y == 9*16+8) {
                List list;
                BoundingBox b1(109*16+8, 9*16, 16, 16);
                BoundingBox b2(129*16+8, 174*16, 16, 16);
                objects->get(&b1, &list);
                objects->get(&b2, &list);

                list.iterateOnFirst();
                while (list.hasNext()) {
                    Switch* inter = dynamic_cast<Switch*>(list.getNext());
                    if (inter != 0) {
                        inter->setLeft(true);
                    }
                }
                scene->getAnimationInGame()->startAnim(28);
                eau2Up = true;
            }
            break;
        case 62 :
            if (x == 27*16 && y == 17*16) {
                scene->setCoffre(5, 24, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(29, 13, 0);
            }
            if (x == 32*16 && y == 62*16) {
                scene->setCoffre(5, 26, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(29, 58, 0);
            }
            if (x == 35*16 && y == 87*16) {
                scene->setCoffre(5, 27, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(29, 88, 0);
            }
            if (x == 102*16 && y == 10*16) {
                scene->setCoffre(5, 28, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(98, 6, 1);
            }
            if (x == 7*16 && y == 192*16) {
                scene->setCoffre(5, 29, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 178, 0);
            }
            if (x == 87*16 && y == 128*16) {
                scene->setCoffre(5, 33, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(78, 126, 1);
            }
            if (x == 62*16 && y == 220*16) {
                scene->setCoffre(5, 35, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(58, 216, 1);
            }
            if (x == 57*16 && y == 145*16) {
                scene->setCoffre(5, 36, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(58, 141, 1);
            }
            if (x == 84*16 && y == 87*16) {
                scene->setCoffre(5, 38, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 88, 0);
            }
            if (x == 115*16 && y == 87*16) {
                scene->setCoffre(5, 39, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(109, 88, 0);
            }
            if (x == 42*16 && y == 4*16) {
                scene->setCoffre(5, 40, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 6, 1);
            }
            if (x == 77*16 && y == 70*16) {
                scene->setCoffre(5, 41, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(78, 66, 1);
            }
            if (x == 22*16 && y == 186*16) {
                scene->setCoffre(5, 42, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 186, 1);
            }
            break;
        case 63 :
            // red
            if (((x == 18*16 || x == 21*16) && y == 16*16)
                || ((x == 28*16 || x == 35*16) && y == 4*16)) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                inters++;
                if (inters == 4) {
                    activateAllInter();
                    scene->setCoffre(6, 14, 1);
                    scene->getAnimationInGame()->startAnim(41);
                }
            }

            // blue
            if (x == 92*16 && y == 202*16) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                setSol(87*16,206*16,6,1,1597,PIERRE);
            }

            if (x == 7*16 && y == 32*16) {
                scene->setCoffre(6, 15, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(18, 36, 1);
            }
            if (x == 37*16 && y == 40*16) {
                scene->setCoffre(6, 16, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 36, 1);
            }
            if (x == 56*16 && y == 142*16) {
                scene->setCoffre(6, 17, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 141, 1);
            }
            if (x == 56*16 && y == 157*16) {
                scene->setCoffre(6, 18, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(49, 163, 0);
            }
            if (x == 42*16 && y == 109*16) {
                scene->setCoffre(6, 19, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 111, 1);
            }
            if (x == 52*16 && y == 117*16) {
                scene->setCoffre(6, 20, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(58, 111, 1);
            }
            if (x == 82*16 && y == 214*16) {
                scene->setCoffre(6, 21, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(78, 216, 1);
            }
            if (x == 86*16 && y == 38*16) {
                scene->setCoffre(6, 22, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 43, 0);
            }
            if (x == 112*16 && y == 12*16) {
                scene->setCoffre(6, 23, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(109, 13, 0);
            }
            break;
        case 64 :
            if (x == 16*16 && y == 37*16) {
                scene->setCoffre(7, 26, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 28, 0);
            }
            if (x == 16*16 && y == 82*16) {
                scene->setCoffre(7, 27, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 88, 0);
            }
            if (x == 27*16 && y == 32*16) {
                scene->setCoffre(7, 28, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(18, 36, 1);
            }
            if (x == 23*16 && y == 52*16) {
                scene->setCoffre(7, 29, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 51, 1);
            }
            if (x == 47*16 && y == 87*16) {
                scene->setCoffre(7, 30, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(49, 88, 0);
            }
            if (x == 47*16 && y == 102*16) {
                scene->setCoffre(7, 31, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(49, 103, 0);
            }
            if (x == 43*16 && y == 142*16) {
                scene->setCoffre(7, 32, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(49, 133, 0);
            }
            if (x == 77*16 && y == 85*16) {
                scene->setCoffre(7, 33, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(78, 81, 1);
            }
            if (x == 87*16 && y == 17*16) {
                scene->setCoffre(7, 34, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 28, 0);
            }
            if (x == 87*16 && y == 87*16) {
                scene->setCoffre(7, 35, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 88, 0);
            }
            if (x == 102*16 && y == 40*16) {
                scene->setCoffre(7, 36, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(98, 36, 1);
            }
            if (x == 102*16 && y == 79*16) {
                scene->setCoffre(7, 37, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(118, 81, 1);
            }
            if (x == 127*16 && y == 47*16) {
                scene->setCoffre(7, 38, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(118, 51, 1);
            }
            if (x == 127*16 && y == 117*16) {
                scene->setCoffre(7, 39, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(118, 111, 1);
            }
            if (x == 147*16 && y == 32*16) {
                scene->setCoffre(7, 40, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(149, 43, 0);
            }
            if (x == 147*16 && y == 62*16) {
                scene->setCoffre(7, 41, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(149, 58, 0);
            }
            if (x == 183*16 && y == 7*16) {
                scene->setCoffre(7, 42, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(189, 13, 0);
            }
            if (x == 197*16 && y == 25*16) {
                scene->setCoffre(7, 43, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(198, 21, 1);
            }
            if (x == 272*16 && y == 62*16) {
                scene->setCoffre(7, 44, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(269, 58, 0);
            }
            break;
        case 65 :
            if (x == 22*16 && y == 25*16) {
                scene->setCoffre(8, 18, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(29, 28, 0);
            }
            if (x == 26*16 && y == 48*16) {
                scene->setCoffre(8, 19, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(18, 51, 1);
            }
            if (x == 64*16 && y == 36*16) {
                scene->setCoffre(8, 20, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(69, 28, 0);
            }
            if (x == 87*16 && y == 47*16) {
                scene->setCoffre(8, 21, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 58, 0);
            }
            if (x == 162*16 && y == 55*16) {
                scene->setCoffre(8, 22, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(158, 51, 1);
            }
            break;
        case 66 :
            if (x == 9*16 && y == 18*16) {
                scene->setCoffre(9, 12, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(18, 21, 1);
            }
            if (x == 17*16 && y == 55*16) {
                scene->setCoffre(9, 13, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(18, 51, 1);
            }
            if (x == 36*16 && y == 7*16) {
                scene->setCoffre(9, 14, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(29, 13, 0);
            }
            if (x == 42*16 && y == 55*16) {
                scene->setCoffre(9, 15, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 51, 1);
            }
            if (x == 66*16 && y == 6*16) {
                scene->setCoffre(9, 16, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(58, 6, 1);
            }
            if (x == 89*16 && y == 33*16) {
                scene->setCoffre(9, 17, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(98, 36, 1);
            }
            if (x == 109*16 && y == 11*16) {
                scene->setCoffre(9, 18, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(98, 6, 1);
            }
            if (x == 117*16 && y == 55*16) {
                scene->setCoffre(9, 19, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(118, 51, 1);
            }
            if (x == 130*16 && y == 11*16) {
                scene->setCoffre(9, 20, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(138, 6, 1);
            }
            if (x == 132*16 && y == 21*16) {
                scene->setCoffre(9, 21, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(129, 13, 0);
            }
            if (x == 132*16 && y == 38*16) {
                scene->setCoffre(9, 22, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(129, 43, 0);
            }
            if (x == 146*16 && y == 38*16) {
                scene->setCoffre(9, 23, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(138, 36, 1);
            }
            if (x == 177*16 && y == 55*16) {
                scene->setCoffre(9, 24, 1);
                if (sound) AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(178, 51, 1);
            }
            break;
    }
}

bool Map::desactivateInter(int x, int y, bool sound) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    switch (map) {
        case 58 :
            if (x == 125*16 && y == 53*16) {
                setSol(127*16,51*16,6,4,1598,MURRET);
                setSol(128*16,52*16,4,2,1412, PIERRE);
            }
            if (x == 127*16 && y == 36*16) {
                setSol(129*16,36*16,2,1,1598,MURRET);
            }
            if (x == 222*16 && y == 19*16) {
                setSol(226*16,21*16,6,4,1598,MURRET);
                setSol(227*16,22*16,4,2,1414, PIERRE);
            }
            if (!checkCollisions(link->getBoundingBox(), (Collisionable*)link, false, false, false, false, false) &&
                    !MainController::getInstance()->getGameController()->isTransitionRoom() &&
                    !MainController::getInstance()->getGameController()->isTeleport()) {
                activateInter(x, y, false);
                return false;
            }
            if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
            break;
        case 59 :
            if (x == 65*16 && y == 124*16) {
                fermePorte(69, 133, 0);
                if (!checkCollisions(link->getBoundingBox(), (Collisionable*)link, false, false, false, false, false) &&
                    !MainController::getInstance()->getGameController()->isTransitionRoom() &&
                    !MainController::getInstance()->getGameController()->isTeleport()) {
                    activateInter(x, y, false);
                    return false;
                }
                if (!MainController::getInstance()->getGameController()->isTeleport())
                /*if (sound) */AudioManager::getInstance()->playSound(TS_DOOR);
            }

            if (x == 136*16 && y == 37*16) {
                setSol(127*16,34*16,6,5,1598,MURRET);
                setSol(128*16,35*16,4,3,1645, PIERRE);
                if (!checkCollisions(link->getBoundingBox(), (Collisionable*)link, false, false, false, false, false) &&
                    !MainController::getInstance()->getGameController()->isTransitionRoom() &&
                    !MainController::getInstance()->getGameController()->isTeleport()) {
                    activateInter(x, y, false);
                    return false;
                }
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
            }
            break;
        case 60 :
            // red
            if (((x == 7*16 || x == 12*16 || x == 187*16 || x == 192*16) && y == 62*16)
                || (x == 195*16 && y == 83*16)
                || (x == 207*16 && (y == 79*16 || y == 85*16))) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                inters--;
            }

            // blue
            if (x == 215*16 && y == 79*16) {
                setSol(211*16,73*16,1,4,1598,MURRET);
                if (!checkCollisions(link->getBoundingBox(), (Collisionable*)link, false, false, false, false, false) &&
                    !MainController::getInstance()->getGameController()->isTransitionRoom() &&
                    !MainController::getInstance()->getGameController()->isTeleport()) {
                    activateInter(x, y, false);
                    return false;
                }
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
            }
            break;
        case 61 :
            if (x == 146*16 && y == 36*16 && (bounds.getX() != 140*16 || scene->getCoffre(4, 37))) {
                fermePorte(149, 28, 0);
                if (!checkCollisions(link->getBoundingBox(), (Collisionable*)link, false, false, false, false, false) &&
                    !MainController::getInstance()->getGameController()->isTransitionRoom() &&
                    !MainController::getInstance()->getGameController()->isTeleport()) {
                    activateInter(x, y, false);
                    return false;
                }
                if (sound || scene->getLink()->getY() < 33*16) AudioManager::getInstance()->playSound(TS_DOOR);
            }
            if (x == 253*16 && y == 21*16) {
                setSol(249*16,21*16,2,1,1598,MURRET);
                if (!checkCollisions(link->getBoundingBox(), (Collisionable*)link, false, false, false, false, false) &&
                    !MainController::getInstance()->getGameController()->isTransitionRoom() &&
                    !MainController::getInstance()->getGameController()->isTeleport()) {
                    activateInter(x, y, false);
                    return false;
                }
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
            }
            break;
        case 63 :
            // red
            if (((x == 18*16 || x == 21*16) && y == 16*16)
                || ((x == 28*16 || x == 35*16) && y == 4*16)) {
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
                inters--;
            }

            // blue
            if (x == 92*16 && y == 202*16) {
                setSol(87*16,206*16,6,1,1598,MURRET);
                if (!checkCollisions(link->getBoundingBox(), (Collisionable*)link, false, false, false, false, false) &&
                    !MainController::getInstance()->getGameController()->isTransitionRoom() &&
                    !MainController::getInstance()->getGameController()->isTeleport()) {
                    activateInter(x, y, false);
                    return false;
                }
                if (sound) AudioManager::getInstance()->playSound(TS_PLOT);
            }
            break;
    }
    return true;
}

void Map::activateAllInter() {
    List list;
    objects->get(&bounds, &list);

    list.iterateOnFirst();
    while (list.hasNext()) {
        Interrupteur* inter = dynamic_cast<Interrupteur*>(list.getNext());
        if (inter != 0) {
            inter->activate();
        }
    }
}

void Map::activateOeil(bool on) {
    switch (map) {
        case 29 :
            if (on) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                fermePorte(9, 13, 0);
            } else {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 13, 0);
            }
            break;
        case 63 :
            if (bounds.getX() == 0) {
                if (on) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    fermePorte(9, 88, 0);
                } else {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(9, 88, 0);
                }
            } else {
                if (on) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    fermePorte(69, 43, 0);
                } else {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(69, 43, 0);
                }
            }
            break;
    }
}

void Map::allumeTorche(int x, int y) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    switch (map) {
        case 62 :
            if ((x == 47 || x == 52) && y == 213) {
                if (sol[47][213] == 972 && sol[52][213] == 972 && mur[49 * 2 + 1][208 * 2 + 1] == MUR) {
                    scene->setCoffre(5, 34, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(49, 208, 0);
                }
            }
            if ((x == 15 || x == 24) && (y == 23 || y == 28 || y == 35 || y == 40)) {
                if (sol[15][23] == 972 && sol[15][28] == 972
                    && sol[15][35] == 972 && sol[15][40] == 972
                    && sol[24][23] == 972 && sol[24][28] == 972
                    && sol[24][40] == 972
                    && mur[9 * 2 + 1][13 * 2 + 1] == MUR) {
                    scene->setCoffre(5, 23, 1);
                    scene->getAnimationInGame()->startAnim(35);
                }
            }
            if ((x == 76 || x == 83) && (y == 16 || y == 25)) {
                if (sol[76][16] == 972 && sol[76][25] == 972
                    && sol[83][16] == 972 && sol[83][25] == 972
                    && mur[69 * 2 + 1][28 * 2 + 1] == MUR) {
                    scene->setCoffre(5, 45, 1);
                    scene->getAnimationInGame()->startAnim(34);
                }
            }
            if ((x == 86 && y == 43) || (x == 94 && y == 37) || (x == 95 && y == 49) || (x == 99 && y == 44)
                || (x == 104 && y == 39) || (x == 105 && y == 51) || (x == 110 && y == 45) || (x == 115 && y == 37)) {
                if (sol[86][43] == 972 && sol[94][37] == 972
                    && sol[95][49] == 972 && sol[99][44] == 972
                    && sol[104][39] == 972 && sol[105][51] == 972
                    && sol[110][45] == 972 && sol[115][37] == 972
                    && mur[109 * 2 + 1][58 * 2 + 1] == MUR) {
                    scene->setCoffre(5, 44, 1);
                    scene->getAnimationInGame()->startAnim(33);
                }
            }
            if ((x == 4 || x == 15) && (y == 77 || y == 87)) {
                if (sol[4][77] == 972 && sol[4][87] == 972
                    && sol[15][77] == 972 && sol[15][87] == 972
                    && mur[18 * 2 + 1][82 * 2] == MUR) {
                    scene->setCoffre(5, 25, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(18, 81, 1);
                }
            }
            if ((x == 49 || x == 70) && (y == 94 || y == 106 || y == 118 || y == 130)) {
                if (sol[49][94] == 972 && sol[49][106] == 972
                    && sol[49][118] == 972 && sol[49][130] == 972
                    && sol[70][94] == 972 && sol[70][106] == 972
                    && sol[70][118] == 972 && sol[70][130] == 972
                    && mur[69 * 2 + 1][88 * 2 + 1] == MUR) {
                    scene->setCoffre(5, 43, 1);
                    scene->getAnimationInGame()->startAnim(32);
                }
            }
            if ((x == 88 && y == 131) || (x == 91 && y == 131) || (x == 108 && y == 127) || (x == 111 && y == 127)
                || (x == 103 && y == 116) || (x == 106 && y == 116) || (x == 103 && y == 119) || (x == 106 && y == 119)) {
                if (sol[88][131] == 972 && sol[91][131] == 972
                    && sol[108][127] == 972 && sol[111][127] == 972
                    && sol[103][116] == 972 && sol[106][116] == 972
                    && sol[103][119] == 972 && sol[106][119] == 972
                    && mur[78 * 2 + 1][112 * 2] == MUR) {
                    scene->setCoffre(5, 37, 1);
                    scene->getAnimationInGame()->startAnim(31);
                }
            }
            if ((x == 82 && y == 154) || (x == 82 && y == 160) || (x == 84 && y == 171) || (x == 84 && y == 175)
                || (x == 89 && y == 164) || (x == 89 && y == 172) || (x == 95 && y == 164) || (x == 95 && y == 172)
                || (x == 108 && y == 157) || (x == 108 && y == 167) || (x == 112 && y == 157) || (x == 112 && y == 167)
                || (x == 107 && y == 177) || (x == 112 && y == 177)) {
                if (sol[82][154] == 972 && sol[82][160] == 972
                    && sol[84][171] == 972 && sol[84][175] == 972
                    && sol[89][164] == 972 && sol[89][172] == 972
                    && sol[95][164] == 972 && sol[95][172] == 972
                    && sol[108][157] == 972 && sol[108][167] == 972
                    && sol[112][157] == 972 && sol[112][167] == 972
                    && sol[107][177] == 972 && sol[112][177] == 972
                    && mur[78 * 2 + 1][157 * 2] == MUR) {
                    scene->setCoffre(5, 32, 1);
                    scene->getAnimationInGame()->startAnim(30);
                }
            }
            if ((x == 55 || x == 64) && (y == 190 || y == 199)) {
                if (sol[55][190] == 972 && sol[55][199] == 972
                    && sol[64][190] == 972 && sol[64][199] == 972
                    && mur[69 * 2 + 1][178 * 2 + 1] == MUR) {
                    scene->setCoffre(5, 31, 1);
                    scene->getAnimationInGame()->startAnim(29);
                }
            }
            if (x == 37 && (y == 197 || y == 207)) {
                if (sol[37][197] == 972 && sol[37][207] == 972 && mur[18 * 2 + 1][202 * 2] == MUR) {
                    scene->setCoffre(5, 30, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(18, 201, 1);
                }
            }


            if ((x == 102 || x == 117) && (y == 92 || y == 102)) {
                if (sol[102][92] == 972 && sol[117][92] == 972 && sol[102][102] == 972 && sol[117][102] == 972 && scene->getCoffre(5, 48) == 0) {
                    //scene->setCoffre(5, 48, 1);
                    AudioManager::getInstance()->playSound(TS_BREAK);
                    addEffect(new Debris(107 * 16 + 8, 95 * 16 + 8, N, 2, false, false));
                    addEffect(new Debris(112 * 16 + 8, 95 * 16 + 8, N, 2, false, false));
                    addEffect(new Debris(107 * 16 + 8, 99 * 16 + 8, N, 2, false, false));
                    addEffect(new Debris(112 * 16 + 8, 99 * 16 + 8, N, 2, false, false));
                    ouvrePorte(106, 94, 3);
                }
            }
            if ((x == 102 || x == 117) && (y == 17 || y == 27)) {
                if (sol[102][17] == 972 && sol[117][17] == 972 && sol[102][27] == 972 && sol[117][27] == 972 && scene->getCoffre(5, 49) == 0) {
                    //scene->setCoffre(5, 49, 1);
                    AudioManager::getInstance()->playSound(TS_BREAK);
                    addEffect(new Debris(108 * 16 + 8, 21 * 16 + 8, N, 2, false, false));
                    addEffect(new Debris(111 * 16 + 8, 21 * 16 + 8, N, 2, false, false));
                    addEffect(new Debris(108 * 16 + 8, 23 * 16 + 8, N, 2, false, false));
                    addEffect(new Debris(111 * 16 + 8, 23 * 16 + 8, N, 2, false, false));
                    ouvrePorte(107, 20, 4);
                }
            }
            break;
        case 63 :
            if ((x == 62 || x == 77) && (y == 92 || y == 102)) {
                List l;
                ennemis->get(&bounds, &l);
                l.iterateOnFirst();
                while (l.hasNext()) {
                    Ennemi102* enn = dynamic_cast<Ennemi102*>(l.getNext());
                    if (enn != 0) {
                        enn->allume();
                    }
                }
            }
            break;
    }
}

void Map::loop() {
    effects->removeDeadNodes();
    ennemis->removeDeadNodes();
    items->removeDeadNodes();
    projectiles->removeDeadNodes();
    objects->removeDeadNodes();


    List toLoop;
    effects->get(&bounds, &toLoop);
    ennemis->get(&bounds, &toLoop);
    items->get(&bounds, &toLoop);
    projectiles->get(&bounds, &toLoop);
    objects->get(&bounds, &toLoop);

    toLoop.iterateOnFirst();
    while (toLoop.hasNext()) {
        Loopable* l = (Loopable*) toLoop.getNext();
        l->loop();
    }

    if (map == 63) {
        if (oldMetronomeValue != metronome->getValue()) {
            Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
            if (link->getStatus()->getVirtualLife() > 0) {
                int value = 0;
                if (link->getY() < 75 * 16) {
                    value = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 15 : 20;
                } else if (link->getY() < 150 * 16) {
                    value = MainController::getInstance()->getGameController()->getSave()->isExpert() ? 30 : 40;
                }
                if (value > 0 && metronome->getValue() % value == 10 && !link->getStatus()->isInvincible() && !link->isByrna()) {
                    link->getStatus()->updateLife(-1);
                }
            }
        }
        oldMetronomeValue = metronome->getValue();
    } else if (map == 66) {
        if (oldMetronomeValue != metronome->getValue()) {
            Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
            if (!link->getStatus()->isOniLink() && link->getStatus()->getVirtualLife() > 0) {
                bool expert = MainController::getInstance()->getGameController()->getSave()->isExpert();
                if (link->getX() < 80 * 16) {
                    if (metronome->getValue() % (expert ? 40 : 60) == 15 && !link->getStatus()->isInvincible() && !link->isByrna()) {
                        link->getStatus()->updateOniLife(1);
                    }
                } else if (link->getX() < 160 * 16) {
                    if (metronome->getValue() % (expert ? 30 : 40) == 15 && !link->getStatus()->isInvincible() && !link->isByrna()) {
                        link->getStatus()->updateOniLife(1);
                    }
                } else if (!link->getInventory()->hasObject(NAVI_ULT) && metronome->getValue() % 20 == 15 && !link->getStatus()->isInvincible() && !link->isByrna()) {
                    link->getStatus()->updateOniLife(1);
                }
            }
        }
        oldMetronomeValue = metronome->getValue();
    }
}

void Map::draw(List* toDraw, BoundingBox* box, int dstX, int dstY) {

    int srcX = box->getX();
    int srcY = box->getY();
    int srcW = box->getW();
    int srcH = box->getH();

    drawSol(srcX, srcY, srcW, srcH, dstX, dstY);


    effects->get(box, toDraw);
    ennemis->get(box, toDraw);
    items->get(box, toDraw);
    projectiles->get(box, toDraw);
    objects->get(box, toDraw);

    if (toDraw != 0) {
        toDraw->iterateOnFirst();
        while (toDraw->hasNext()) {
            Drawable* d = (Drawable*) toDraw->getNext();
            d->draw(srcX - dstX, srcY - dstY);
        }
    }

    drawAir(srcX, srcY, srcW, srcH, dstX, dstY);
}

void Map::drawSol(int srcX, int srcY, int srcW, int srcH, int dstX, int dstY) {

    int w = 0;
    int h = 0;
    int diffX = 0;
    int diffY = 0;

    for (int j = srcY; j < srcY + srcH; j += CASE) {
        for (int i = srcX; i < srcX + srcW; i += CASE) {

            diffX = (i % CASE);
            diffY = (j % CASE);

            int motif = getMotif(sol[i / CASE][j / CASE]);

            if (motif >= 0) {

                int im = motif / 1000;
                motif -= im * 1000;

                w = CASE - diffX;
                h = CASE - diffY;

                if (i + w > srcX + srcW) {
                    w = srcX + srcW - i;
                }
                if (j + h > srcY + srcH) {
                    h = srcY + srcH - j;
                }

                WindowManager::getInstance()->draw(
                                            images[im],
                                            (motif % 40) * CASE + diffX,
                                            (motif / 40) * CASE + diffY, w, h,
                                            dstX + i - srcX, dstY + j - srcY);
            }
            i -= diffX;
        }
        j -= diffY;
    }


}

void Map::drawAir(int srcX, int srcY, int srcW, int srcH, int dstX, int dstY) {

    int w = 0;
    int h = 0;
    int diffX = 0;
    int diffY = 0;

    for (int j = srcY; j < srcY + srcH; j += CASE) {
        for (int i = srcX; i < srcX + srcW; i += CASE) {

            diffX = (i % CASE);
            diffY = (j % CASE);

            int motif = getMotif(air[i / CASE][j / CASE]);

            if (motif >= 0) {

                int im = motif / 1000;
                motif -= im * 1000;

                w = CASE - diffX;
                h = CASE - diffY;

                if (i + w > srcX + srcW) {
                    w = srcX + srcW - i;
                }
                if (j + h > srcY + srcH) {
                    h = srcY + srcH - j;
                }

                WindowManager::getInstance()->draw(
                                            images[im],
                                            (motif % 40) * CASE + diffX, (motif / 40) * CASE + diffY, w, h,
                                            dstX + i - srcX, dstY + j - srcY);
            }
            i -= diffX;
        }
        j -= diffY;
    }

}

int Map::getMotif(int value) {
    int tmp = metronome->getValue();
    switch (value) {
        case 1 : // fleurs
            switch (tmp % 4) {
                case 0 : return 2;
                case 1 : return 1;
                case 2 : return 3;
                case 3 : return 1;
            }
            break;
        case 36 : case 37 : case 38 : case 39 : case 40 :  case 41 :
        case 42 : case 43 :
            switch (tmp % 4) {
                case 0 : return value;
                case 1 : return value + 8;
                case 2 : return value + 8 * 2;
                case 3 : return value + 8;
            }
            break;
        case 174 : case 177 : case 180 : case 183 : case 186 : case 189 :
        case 192 : case 195 : case 209 : case 212 : case 215 : case 218 :
        case 221 : case 224 : case 231 : case 234 : case 237 : case 240 :
        case 245 : case 248 : case 281 : case 284 : case 836 : case 839 :
        case 842 : case 845 : case 1192 : case 1195 : case 1198 : case 1261 :
            switch (tmp/2 % 3) {
                case 0 : return value;
                case 1 : return value + 1;
                case 2 : return value + 2;
            }
            break;
        case 330 : case 333 : case 972 : case 1239 : case 1242 : case 1245 :
        case 1248 : case 1251 : case 1254 : case 1257 : case 1814 :
            switch (tmp % 4) {
                case 0 : return value;
                case 1 : return value + 1;
                case 2 : return value + 2;
                case 3 : return value + 1;
            }
            break;
        case 529 :
            switch (tmp % 3) {
                case 0 : return 529;
                case 1 : return 532;
                case 2 : return 534;
            }
            break;
        case 531 :
            switch (tmp % 3) {
                case 0 : return 531;
                case 1 : return 533;
                case 2 : return 535;
            }
            break;
        case 1153 : case 1154 : case 1155 : case 1156 : case 1157 : case 1158 : case 1159 :
        case 1160 : case 1161 : case 1162 : case 1163 : case 1164 : case 1165 :
            switch (tmp / 2 % 3) {
                case 0 : return value;
                case 1 : return value + 13;
                case 2 : return value + 26;
            }
            break;
        case 1399 : case 1403 :
            switch (tmp/2 % 4) {
                case 0 : return value;
                case 1 : return value + 1;
                case 2 : return value + 2;
                case 3 : return value + 3;
            }
            break;
        case 1407 :
            switch (tmp % 4) {
                case 0 : return 1407;
                case 1 : return 1400;
                case 2 : return 1401;
                case 3 : return 1402;
            }
            break;
        case 2057 : // pics
            switch (tmp % 6) {
                case 0 : return 2057;
                case 1 : return 2058;
                case 2 : return 2059;
                case 3 : return 2060;
                case 4 : return 2059;
                case 5 : return 2058;
            }
            break;
        case 2233 : case 2234 : case 2235 : case 2236 :
            switch (tmp / 2 % 3) {
                case 0 : return value;
                case 1 : return value + 4;
                case 2 : return value + 8;
            }
            break;
        case 2245 : case 2246 : case 2251 : case 2252 :
            switch (tmp / 2 % 3) {
                case 0 : return value;
                case 1 : return value + 2;
                case 2 : return value + 4;
            }
            break;
        case 2463 : case 2464 : case 2465 : case 2466 : case 2467 : case 2468 : case 2469 :
        case 2470 : case 2471 : case 2472 : case 2473 : case 2474 : case 2475 :
            switch (tmp / 2 % 4) {
                case 0 : return value;
                case 1 : case 3 : return value + 13;
                case 2 : return value + 26;
            }
            break;
        case 3333 : case 3334 : case 3335 : case 3336 : case 3337 : case 3338 : case 3339 : case 3340 : case 3341 :
            switch (tmp / 2 % 3) {
                case 0 : return value;
                case 1 : return value + 9;
                case 2 : return value + 18;
            }
            break;
        default : break;
    }
    return value;
}

short Map::getMur(int x, int y) {
    return mur[x / 8][y / 8];
}

short Map::getSol(int x, int y) {
    return sol[x / CASE][y / CASE];
}

void Map::setSol(int x, int y, short s) {
    int i = x / CASE;
    int j = y / CASE;
    sol[i][j] = s;
}

void Map::setSol(int x, int y, short s, Collision c) {
    int i = x / CASE;
    int j = y / CASE;
    sol[i][j] = s;
    mur[i * 2][j * 2] = c;
    mur[i * 2 + 1][j * 2] = c;
    mur[i * 2][j * 2 + 1] = c;
    mur[i * 2 + 1][j * 2 + 1] = c;
}

void Map::setSol(int x, int y, int w, int h, short s, Collision c) {
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            setSol(x + i * 16, y + j * 16, s, c);
        }
    }
}

void Map::addEffect(Effect* effect) {
    effects->add((Node*) effect);
}

void Map::addEnnemi(Ennemi* ennemi) {
    ennemis->add((Node*) ennemi);
}

void Map::addItem(Item* item) {
    items->add((Node*) item);
}

void Map::addProjectile(Projectile* projectile) {
    projectiles->add((Node*) projectile);
}

void Map::addObject(Object* object) {
    objects->add((Node*) object);
}

void Map::removeObject(Object* object) {
    objects->remove((Node*) object);
}

bool Map::checkCollisions(BoundingBox* box, Collisionable* object, bool checkEnnemis, bool ignoreNotIdle, bool withAvoid, bool onlyBox, bool checkObjects,
                          int safeX, int safeY, int safeW, int safeH) {

    if (box->getX() < bounds.getX() || box->getX() + box->getW() > bounds.getX() + bounds.getW() ||
        box->getY() < bounds.getY() || box->getY() + box->getH() > bounds.getY() + bounds.getH()) {
        return false;
    }

    if (onlyBox) {
        return true;
    }

    for (int j = box->getY() - (box->getY() % 8); j < box->getY() + box->getH(); j += 8) {
        for (int i = box->getX() - (box->getX() % 8); i < box->getX() + box->getW(); i += 8) {
            Collision c = (Collision)getMur(i, j);
            if (!object->isCollision(c)
                || (withAvoid && object->isToAvoid(c))) {
                if (c == DANGER) {
                    Link* lk = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
                    if ((Collisionable*)lk == object) {
                        Direction dir = N;
                        switch (lk->getDirection()) {
                            case N : dir = S; break;
                            case S : dir = N; break;
                            case W : dir = E; break;
                            case E : dir = W; break;
                        }
                        lk->underAttack(dir, MainController::getInstance()->getGameController()->getSave()->isExpert() ? 8 : 1, TA_PHYSIC, TE_NORMAL);
                    }
                }
                return false;
            } else {
                if (c == DANGER_BAS) {
                    Link* lk = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
                    if ((Collisionable*)lk == object) {
                        Direction dir = N;
                        switch (lk->getDirection()) {
                            case N : dir = S; break;
                            case S : dir = N; break;
                            case W : dir = E; break;
                            case E : dir = W; break;
                        }
                        lk->underAttack(dir, MainController::getInstance()->getGameController()->getSave()->isExpert() ? 8 : 1, TA_PHYSIC, TE_NORMAL);
                    }
                }
            }
        }
    }

    if (checkEnnemis) {
        List enn;
        ennemis->get(box, &enn);

        enn.iterateOnFirst();
        while (enn.hasNext()) {
            Ennemi* e = (Ennemi*) enn.getNext();
            if (ignoreNotIdle && !e->isIdle()) continue;
            if (e != object && e->getBoundingBox()->intersect(box)) {
                return false;
            }
        }
    }

    if (checkObjects) {
        List obj;
        objects->get(box, &obj);
        obj.iterateOnFirst();
        while (obj.hasNext()) {
            Object* o = (Object*) obj.getNext();
            if (o != object && o->getBoundingBox()->intersect(box)) {
                BoundingBox safeBox;
                safeBox.setX(safeX);
                safeBox.setY(safeY);
                safeBox.setW(safeW);
                safeBox.setH(safeH);
                if (!o->getBoundingBox()->intersect(&safeBox)) {
                    return false;
                }
            }
            if (o != object && o->getSecondBoundingBox() != 0 && o->getSecondBoundingBox()->intersect(box)) {
                return false;
            }
        }
    }

    return true;
}

bool Map::testDegat(BoundingBox* box, Direction dir, int force, TypeAttack type, TypeEffect effect, bool onEnnemis) {

    bool result = false;

    // on ennemi :
    if (onEnnemis) {
        List enn;
        ennemis->get(box, &enn);

        enn.iterateOnFirst();
        while (enn.hasNext()) {
            Ennemi* e = (Ennemi*) enn.getNext();
            if (e->getBoundingBox()->intersect(box)) {
                e->underAttack(dir, force, type, effect, box);
                result = true;
            }
        }
    }

    // on objects :

    List obj;
    objects->get(box, &obj);

    obj.iterateOnFirst();
    while (obj.hasNext()) {
        Object* o = (Object*) obj.getNext();
        if (o->isAttackable() && o->getBoundingBox()->intersect(box)) {
            o->underAttack(dir, force, type, effect);
            result = true;
        }
    }

    // on projectiles :

    Link* link = MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->getLink();
    if ((type == TA_SWORD || type == TA_SWORD_HOLD) && link->getEpee() >= 2) {
        List proj;
        projectiles->get(box, &proj);

        proj.iterateOnFirst();
        while (proj.hasNext()) {
            Projectile* p = (Projectile*) proj.getNext();
            if (p->getBoundingBox()->intersect(box)) {
                p->renvoie(dir);
                result = true;
            }
        }
    }

    // on map :

    int x0 = box->getX();
    int beginX = x0 - (x0 % CASE);
    int x1 = box->getX() + box->getW() - 1;
    int endX = x1 - (x1 % CASE);

    int y0 = box->getY();
    int beginY = y0 - (y0 % CASE);
    int y1 = box->getY() + box->getH() - 1;
    int endY = y1 - (y1 % CASE);

    if (beginX < 0) beginX = 0;
    if (beginY < 0) beginY = 0;
    if (endX > getW()) endX = getW();
    if (endY > getH()) endY = getH();

    for (int j = beginY; j <= endY; j += CASE) {
        for (int i = beginX; i <= endX; i += CASE) {

            int x = i / CASE;
            int y = j / CASE;
            int val = sol[x][y];

            if (type == TA_SWORD || effect == TE_FEU) {
                // herbe haute
                if (val == 18) {
                    sol[x][y] = 19;
                    mur[x * 2][y * 2] = HERBE;
                    mur[(x * 2) + 1][y * 2] = HERBE;
                    mur[x * 2][(y * 2) + 1] = HERBE;
                    mur[(x * 2) + 1][(y * 2) + 1] = HERBE;

                    if (!testGemmeForce(x, y)) {
                        TypeItem t = MapHelper::getInstance()->getItemRandomizer(TD_HERBE_HAUTE)->selectType();
                        if (t != TI_NO_ITEM) {
                            addItem(ItemHelper::getInstance()->createItem(t, i + 8, j + 8));
                        }
                    }

                    AudioManager::getInstance()->playSound(TS_BUISSON);

                    result = (type == TA_SWORD);
                }
            }
            if (type == TA_SWORD || effect == TE_FEU || type == TA_SWORD_HOLD) {
                if (val == 310 || val == 312 || val == 314 || val == 316 || val == 318) {
                    // buissons
                    Collision c;
                    TypeDons td;
                    int tb;
                    switch (val) {
                        case 310 : c = HERBE; td = TD_BUISSON_VERT; tb = 0; break;
                        case 312 : c = SABLE; td = TD_BUISSON_VERT; tb = 0; break;
                        case 314 : c = HERBE; td = TD_BUISSON_ROSE; tb = 1; break;
                        case 316 : c = SABLE; td = TD_BUISSON_OCRE; tb = 2; break;
                        case 318 : c = NEIGE; td = TD_BUISSON_NEIGE; tb = 3; break;
                        default : c = HERBE; td = TD_BUISSON_VERT; tb = 0; break;
                    }

                    sol[x][y] = val + 1;

                    mur[x * 2][y * 2] = c;
                    mur[(x * 2) + 1][y * 2] = c;
                    mur[x * 2][(y * 2) + 1] = c;
                    mur[(x * 2) + 1][(y * 2) + 1] = c;

                    if (!testGemmeForce(x, y)) {
                        TypeItem t = MapHelper::getInstance()->getItemRandomizer(td)->selectType();
                        if (t != TI_NO_ITEM) {
                            addItem(ItemHelper::getInstance()->createItem(t, i + 8, j + 8));
                        }
                    }
                    addEffect(new Feuilles(x * 16 + 8 - 14, y * 16 + 8 - 22 - 8, tb));
                    AudioManager::getInstance()->playSound(TS_BUISSON);

                    result = (type == TA_SWORD || type == TA_SWORD_HOLD);
                }
            }
            if (effect == TE_FEU && val == 2978) {
                AudioManager::getInstance()->playSound(TS_SURPRISE);
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->setCoffre(7, 46, 1);
                ouvrePorte(29, 141, 4);
                AudioManager::getInstance()->playSound(TS_PLOUF);
                addEffect(new Plouf(29*16+8, 141*16+8));
            }
            if (effect == TE_GLACE) {
                if ((getMur(i, j) == EAU || getMur(i, j) == EAU_PROF)
                       && (getMur(i + 8, j) == EAU || getMur(i + 8, j) == EAU_PROF)
                       && (getMur(i, j + 8) == EAU || getMur(i, j + 8) == EAU_PROF)
                       && (getMur(i + 8, j + 8) == EAU || getMur(i + 8, j + 8) == EAU_PROF)) {

                    sol[x][y] = 118;
                    mur[x * 2][y * 2] = GLACE;
                    mur[(x * 2) + 1][y * 2] = GLACE;
                    mur[x * 2][(y * 2) + 1] = GLACE;
                    mur[(x * 2) + 1][(y * 2) + 1] = GLACE;
                }
                switch (val) {
                    case 20 : sol[x][y] = 119; break;
                    case 21 : sol[x][y] = 120; mur[x * 2][y * 2 + 1] = GLACE; mur[x * 2 + 1][y * 2 + 1] = GLACE; break;
                    case 22 : sol[x][y] = 121; break;
                    case 23 : sol[x][y] = 122; mur[x * 2 + 1][y * 2] = GLACE; mur[x * 2 + 1][y * 2 + 1] = GLACE; break;
                    case 25 : sol[x][y] = 123; mur[x * 2][y * 2] = GLACE; mur[x * 2][y * 2 + 1] = GLACE; break;
                    case 26 : sol[x][y] = 124; break;
                    case 27 : sol[x][y] = 125; mur[x * 2][y * 2] = GLACE; mur[x * 2 + 1][y * 2] = GLACE; break;
                    case 28 : sol[x][y] = 126; break;
                    case 29 : sol[x][y] = 127; mur[x * 2 + 1][y * 2] = GLACE; mur[x * 2][y * 2 + 1] = GLACE; mur[x * 2 + 1][y * 2 + 1] = GLACE; break;
                    case 30 : sol[x][y] = 128; mur[x * 2][y * 2] = GLACE; mur[x * 2][y * 2 + 1] = GLACE; mur[x * 2 + 1][y * 2 + 1] = GLACE; break;
                    case 31 : sol[x][y] = 129; mur[x * 2][y * 2] = GLACE; mur[x * 2 + 1][y * 2] = GLACE; mur[x * 2 + 1][y * 2 + 1] = GLACE; break;
                    case 32 : sol[x][y] = 130; mur[x * 2][y * 2] = GLACE; mur[x * 2 + 1][y * 2] = GLACE; mur[x * 2][y * 2 + 1] = GLACE; break;
                    case 221 : sol[x][y] = 1087; mur[x * 2][y * 2] = GLACE; mur[x * 2 + 1][y * 2] = GLACE; mur[x * 2][y * 2 + 1] = GLACE; break;
                    case 231 : sol[x][y] = 1088; mur[x * 2][y * 2] = GLACE; mur[x * 2 + 1][y * 2] = GLACE; mur[x * 2 + 1][y * 2 + 1] = GLACE; break;
                    case 237 : sol[x][y] = 1089; mur[x * 2][y * 2] = GLACE; mur[x * 2][y * 2 + 1] = GLACE; mur[x * 2 + 1][y * 2 + 1] = GLACE; break;
                    case 245 : sol[x][y] = 1090; mur[x * 2 + 1][y * 2] = GLACE; mur[x * 2][y * 2 + 1] = GLACE; mur[x * 2 + 1][y * 2 + 1] = GLACE; break;
                    case 281 : sol[x][y] = 287; mur[x * 2][y * 2 + 1] = GLACE; mur[x * 2 + 1][y * 2 + 1] = GLACE; break;
                    case 284 : sol[x][y] = 288; mur[x * 2][y * 2 + 1] = GLACE; mur[x * 2 + 1][y * 2 + 1] = GLACE; break;
                    case 972 : sol[x][y] = 971; break; // torche
                    default : break;
                }
            }
            if (type == TA_EXPLO || type == TA_MARTEAU) {
                if (val == 320 || val == 321) {
                    addEffect(new Debris(x * 16 + 8, y * 16 + 8, N, 2));
                    AudioManager::getInstance()->playSound(TS_BREAK);

                    mur[x * 2][y * 2] = HERBE;
                    mur[(x * 2) + 1][y * 2] = HERBE;
                    mur[x * 2][(y * 2) + 1] = HERBE;
                    mur[(x * 2) + 1][(y * 2) + 1] = HERBE;

                    if (val == 320) {
                        sol[x][y] = 140;
                    } else {
                        sol[x][y] = 137;
                    }

                    testGemmeForce(x, y);
                }
            }
            if (type == TA_EXPLO) {
                if (val == 581 || val == 582) { // plaine
                    // fissure
                    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
                    switch (map) {
                        case 4 :
                            AudioManager::getInstance()->playSound(TS_SURPRISE);
                            scene->setCoffre(0, 0, 1);
                            ouvrePorte(5, 33, 0);
                            break;
                        case 11 :
                            AudioManager::getInstance()->playSound(TS_SURPRISE);
                            scene->setCoffre(0, 4, 1);
                            ouvrePorte(20, 69, 0);
                            break;
                    }
                } else if (val == 702 || val == 703) { // eau
                    // fissure
                    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
                    switch (map) {
                        case 5 :
                            AudioManager::getInstance()->playSound(TS_SURPRISE);
                            scene->setCoffre(0, 1, 1);
                            ouvrePorte(145, 17, 0);
                            break;
                    }
                } else if (val == 863 || val == 864) { // plaine ocre
                    // fissure
                    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
                    switch (map) {
                        case 10 :
                            AudioManager::getInstance()->playSound(TS_SURPRISE);
                            scene->setCoffre(0, 3, 1);
                            ouvrePorte(15, 2, 0);
                            break;
                    }
                } else if (val == 1118 || val == 1119) { // grottes
                    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
                    switch (map) {
                        case 15 :
                            AudioManager::getInstance()->playSound(TS_SURPRISE);
                            scene->setCoffre(0, 6, 1);
                            ouvrePorte(9, 13, 0);
                            break;
                    }
                } else if (map > 57) {
                    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
                    // donjon
                    switch (map) {
                        case 60 :
                            if (val == 2009 || val == 2010) {
                                if (bounds.getX() < 320 * 2) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 29, 1);
                                    ouvrePorte(29, 58, 2);
                                } else if (bounds.getX() == 320 * 3 && y < 75) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 30, 1);
                                    ouvrePorte(69, 58, 2);
                                } else if (bounds.getX() == 320 * 3 && y > 75) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 31, 1);
                                    ouvrePorte(69, 88, 2);
                                } else if (bounds.getX() == 320 * 5) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 32, 1);
                                    ouvrePorte(129, 43, 2);
                                } else if (bounds.getY() == 240 * 5) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 33, 1);
                                    ouvrePorte(129, 73, 2);
                                } else if (bounds.getX() == 320 * 9 && bounds.getY() == 240 * 1) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 35, 1);
                                    ouvrePorte(189, 28, 2);
                                } else if (bounds.getX() == 320 * 10 && bounds.getY() == 240 * 1) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 37, 1);
                                    ouvrePorte(209, 43, 2);
                                } else {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 38, 1);
                                    ouvrePorte(209, 58, 2);
                                }
                            } else if (val == 2011 || val == 2012) {
                                if (bounds.getX() == 320 * 6) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 34, 1);
                                    ouvrePorte(138, 66, 3);
                                } else {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 36, 1);
                                    ouvrePorte(198, 21, 3);
                                }
                            }
                            break;
                        case 62 :
                            if (val == 2542) {
                                AudioManager::getInstance()->playSound(TS_SURPRISE);
                                scene->setCoffre(map - 57, 46, 1);
                                ouvrePorte(69, 79, 2);
                            }
                            break;
                        case 63 :
                            if (val == 2739 || val == 2740) {
                                if (bounds.getY() == 240 * 14) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 25, 1);
                                    ouvrePorte(9, 208, 3);
                                } else {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 24, 1);
                                    ouvrePorte(9, 193, 3);
                                }
                            } else if (val == 2736) {
                                AudioManager::getInstance()->playSound(TS_SURPRISE);
                                scene->setCoffre(map - 57, 26, 1);
                                ouvrePorte(9, 81, 2);
                            }
                            break;
                        case 64 :
                            if (val == 2944 || val == 2945) {
                                if (bounds.getX() == 0) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 45, 1);
                                    ouvrePorte(9, 43, 2);
                                } else {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 48, 1);
                                    ouvrePorte(269, 43, 2);
                                }
                            } else if (val == 2946 || val == 2947) {
                                AudioManager::getInstance()->playSound(TS_SURPRISE);
                                scene->setCoffre(map - 57, 47, 1);
                                ouvrePorte(98, 6, 3);
                            }
                            break;
                        case 65 :
                            if (val == 3152 || val == 3153) {
                                AudioManager::getInstance()->playSound(TS_SURPRISE);
                                scene->setCoffre(map - 57, 24, 1);
                                ouvrePorte(89, 43, 2);
                            } else if (val == 3154 || val == 3155) {
                                AudioManager::getInstance()->playSound(TS_SURPRISE);
                                scene->setCoffre(map - 57, 23, 1);
                                ouvrePorte(18, 21, 3);
                            }
                            break;
                        case 66 :
                            if (val == 3318 || val == 3319) {
                                if (bounds.getX() == 320 * 4) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 25, 1);
                                    ouvrePorte(89, 28, 2);
                                } else if (bounds.getX() == 320 * 5) {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 26, 1);
                                    ouvrePorte(109, 13, 2);
                                } else {
                                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                                    scene->setCoffre(map - 57, 27, 1);
                                    ouvrePorte(229, 43, 2);
                                }
                            }
                            break;
                    }
                }
            }
            if (type == TA_MARTEAU) {
                if (val == 388) {
                    AudioManager::getInstance()->playSound(TS_PLOT);
                    mur[x * 2][y * 2] = HERBE;
                    mur[(x * 2) + 1][y * 2] = HERBE;
                    mur[x * 2][(y * 2) + 1] = HERBE;
                    mur[(x * 2) + 1][(y * 2) + 1] = HERBE;
                    sol[x][y] = 389;
                } else if (val == 1067 || val == 1068 || val == 1069) {
                    if (map == 14) {
                        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
                        addEffect(new Debris(25 * 16 + 8, 26 * 16 + 8, N, 2));
                        addEffect(new Debris(26 * 16 + 8, 25 * 16 + 8, N, 2));
                        addEffect(new Debris(27 * 16 + 8, 26 * 16 + 8, N, 2));
                        AudioManager::getInstance()->playSound(TS_BREAK);
                        AudioManager::getInstance()->playSound(TS_SURPRISE);
                        scene->setCoffre(0, 5, 1);
                        ouvrePorte(25, 25, 0);
                    }
                } else if (val == 1237) {
                    AudioManager::getInstance()->playSound(TS_PLOT);
                    setSol(x * 16, y * 16, 1, 1, 1238, TERRE);
                } else if (val == 1814) {
                    AudioManager::getInstance()->playSound(TS_PLOT);
                    setSol(x * 16, y * 16, 1, 1, 1813, PIERRE);
                }
            }
            if (effect == TE_FEU) {
                if (val == 971) {
                    sol[x][y] = 972;
                    allumeTorche(x, y);
                }
            }


        }
    }

    return result;
}

bool Map::testGemmeForce(int x, int y) {

    Inventory* inventory = MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->getLink()->getInventory();

    switch (map) {
        case 1 :
            if (x == 28 && y == 30 && !inventory->hasGemmeForce(0)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 28*16 + 8, 30*16 + 8, 0));
                return true;
            }
            if (x == 86 && y == 6 && !inventory->hasGemmeForce(1)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 86*16 + 8, 6*16 + 8, 1));
                return true;
            }
            if (x == 99 && y == 46 && !inventory->hasGemmeForce(2)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 99*16 + 8, 46*16 + 8, 2));
                return true;
            }
            if (x == 110 && y == 38 && !inventory->hasGemmeForce(3)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 110*16 + 8, 38*16 + 8, 3));
                return true;
            }
            break;
        case 2 :
            if (x == 78 && y == 10 && !inventory->hasGemmeForce(4)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 78*16 + 8, 10*16 + 8, 4));
                return true;
            }
            if (x == 110 && y == 55 && !inventory->hasGemmeForce(5)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 110*16 + 8, 55*16 + 8, 5));
                return true;
            }
            if (x == 42 && y == 13 && !inventory->hasGemmeForce(6)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 42*16 + 8, 13*16 + 8, 6));
                return true;
            }
            // gemme 7 on this map too
            break;
        case 3 :
            if (x == 8 && y == 44 && !inventory->hasGemmeForce(8)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 8*16 + 8, 44*16 + 8, 8));
                return true;
            }
            if (x == 54 && y == 3 && !inventory->hasGemmeForce(9)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 54*16 + 8, 3*16 + 8, 9));
                return true;
            }
            if (x == 12 && y == 4 && !inventory->hasGemmeForce(10)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 12*16 + 8, 4*16 + 8, 10));
                return true;
            }
            // gemme 11 on this map too
            break;
        case 4 :
            if (x == 4 && y == 35 && !inventory->hasGemmeForce(12)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 4*16 + 8, 35*16 + 8, 12));
                return true;
            }
            if (x == 13 && y == 5 && !inventory->hasGemmeForce(13)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 13*16 + 8, 5*16 + 8, 13));
                return true;
            }
            if (x == 31 && y == 40 && !inventory->hasGemmeForce(14)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 31*16 + 8, 40*16 + 8, 14));
                return true;
            }
            if (x == 66 && y == 4 && !inventory->hasGemmeForce(15)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 66*16 + 8, 4*16 + 8, 15));
                return true;
            }
            break;
        case 5 :
            if (x == 24 && y == 4 && !inventory->hasGemmeForce(16)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 24*16 + 8, 4*16 + 8, 16));
                return true;
            }
            if (x == 100 && y == 44 && !inventory->hasGemmeForce(17)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 100*16 + 8, 44*16 + 8, 17));
                return true;
            }
            if (x == 143 && y == 10 && !inventory->hasGemmeForce(18)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 143*16 + 8, 10*16 + 8, 18));
                return true;
            }
            if (x == 73 && y == 48 && !inventory->hasGemmeForce(19)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 73*16 + 8, 48*16 + 8, 19));
                return true;
            }
            if (x == 63 && y == 5 && !inventory->hasGemmeForce(20)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 63*16 + 8, 5*16 + 8, 20));
                return true;
            }
            // gemme 21 on this map too
            break;
        case 6 :
            if (x == 42 && y == 34 && !inventory->hasGemmeForce(22)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 42*16 + 8, 34*16 + 8, 22));
                return true;
            }
            if (x == 65 && y == 13 && !inventory->hasGemmeForce(23)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 65*16 + 8, 13*16 + 8, 23));
                return true;
            }
            if (x == 54 && y == 28 && !inventory->hasGemmeForce(24)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 54*16 + 8, 28*16 + 8, 24));
                return true;
            }
            break;
        case 7 :
            // 25, 26, 27, 28, 29 are on this map too
            if (x == 29 && y == 5 && !inventory->hasGemmeForce(30)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 29*16 + 8, 5*16 + 8, 30));
                return true;
            }
            if (x == 31 && y == 53 && !inventory->hasGemmeForce(31)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 31*16 + 8, 53*16 + 8, 31));
                return true;
            }
            if (x == 47 && y == 17 && !inventory->hasGemmeForce(32)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 47*16 + 8, 17*16 + 8, 32));
                return true;
            }
            if (x == 73 && y == 7 && !inventory->hasGemmeForce(33)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 73*16 + 8, 7*16 + 8, 33));
                return true;
            }
            break;
        case 8 :
            if (x == 30 && y == 19 && !inventory->hasGemmeForce(34)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 30*16 + 8, 19*16 + 8, 34));
                return true;
            }
            if (x == 54 && y == 5 && !inventory->hasGemmeForce(35)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 54*16 + 8, 5*16 + 8, 35));
                return true;
            }
            if (x == 56 && y == 48 && !inventory->hasGemmeForce(36)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 56*16 + 8, 48*16 + 8, 36));
                return true;
            }
            // gemme 37 is on this map too
            break;
        case 9 :
            if (x == 73 && y == 7 && !inventory->hasGemmeForce(38)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 73*16 + 8, 7*16 + 8, 38));
                return true;
            }
            if (x == 5 && y == 8 && !inventory->hasGemmeForce(39)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 5*16 + 8, 8*16 + 8, 39));
                return true;
            }
            if (x == 56 && y == 22 && !inventory->hasGemmeForce(40)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 56*16 + 8, 22*16 + 8, 40));
                return true;
            }
            if (x == 35 && y == 44 && !inventory->hasGemmeForce(41)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 35*16 + 8, 44*16 + 8, 41));
                return true;
            }
            break;
        case 10 :
            if (x == 27 && y == 40 && !inventory->hasGemmeForce(42)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 27*16 + 8, 40*16 + 8, 42));
                return true;
            }
            if (x == 46 && y == 44 && !inventory->hasGemmeForce(43)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 46*16 + 8, 44*16 + 8, 43));
                return true;
            }
            if (x == 13 && y == 41 && !inventory->hasGemmeForce(44)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 13*16 + 8, 41*16 + 8, 44));
                return true;
            }
            if (x == 48 && y == 14 && !inventory->hasGemmeForce(45)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 48*16 + 8, 14*16 + 8, 45));
                return true;
            }
            break;
        case 11 :
            // stones
            if (x == 96 && y == 61 && stones == 0) {
                stones++;
                return true;
            }
            if (x == 117 && y == 48 && stones == 1) {
                stones++;
                return true;
            }
            if (x == 79 && y == 32 && stones == 2) {
                stones++;
                return true;
            }
            if (x == 78 && y == 54 && stones == 3) {
                stones++;
                return true;
            }
            if (x == 89 && y == 51 && stones == 4) {
                AudioManager::getInstance()->playSound(TS_SURPRISE);
                ouvrePorte(89, 51, 1);
                return true;
            }
            // gemmes
            if (x == 5 && y == 13 && !inventory->hasGemmeForce(46)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 5*16 + 8, 13*16 + 8, 46));
                return true;
            }
            if (x == 26 && y == 74 && !inventory->hasGemmeForce(47)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 26*16 + 8, 74*16 + 8, 47));
                return true;
            }
            if (x == 70 && y == 23 && !inventory->hasGemmeForce(48)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 70*16 + 8, 23*16 + 8, 48));
                return true;
            }
            if (x == 71 && y == 79 && !inventory->hasGemmeForce(49)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 71*16 + 8, 79*16 + 8, 49));
                return true;
            }
            if (x == 114 && y == 25 && !inventory->hasGemmeForce(50)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 114*16 + 8, 25*16 + 8, 50));
                return true;
            }
            if (x == 145 && y == 45 && !inventory->hasGemmeForce(51)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 145*16 + 8, 45*16 + 8, 51));
                return true;
            }
            if (x == 168 && y == 12 && !inventory->hasGemmeForce(52)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 168*16 + 8, 12*16 + 8, 52));
                return true;
            }
            // gemme 53 is on this map too
            break;
        case 12 :
            if (x == 75 && y == 5 && !inventory->hasGemmeForce(54)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 75*16 + 8, 5*16 + 8, 54));
                return true;
            }
            if (x == 8 && y == 35 && !inventory->hasGemmeForce(55)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 8*16 + 8, 35*16 + 8, 55));
                return true;
            }
            if (x == 71 && y == 36 && !inventory->hasGemmeForce(56)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 71*16 + 8, 36*16 + 8, 56));
                return true;
            }
            if (x == 35 && y == 35 && !inventory->hasGemmeForce(57)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 35*16 + 8, 35*16 + 8, 57));
                return true;
            }
            break;
        case 13 :
            if (x == 42 && y == 5 && !inventory->hasGemmeForce(58)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 42*16 + 8, 5*16 + 8, 58));
                return true;
            }
            if (x == 36 && y == 29 && !inventory->hasGemmeForce(59)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 36*16 + 8, 29*16 + 8, 59));
                return true;
            }
            if (x == 3 && y == 32 && !inventory->hasGemmeForce(60)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 3*16 + 8, 32*16 + 8, 60));
                return true;
            }
            if (x == 15 && y == 6 && !inventory->hasGemmeForce(61)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 15*16 + 8, 6*16 + 8, 61));
                return true;
            }
            break;
        case 14 :
            if (x == 14 && y == 15 && !inventory->hasGemmeForce(62)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 14*16 + 8, 15*16 + 8, 62));
                return true;
            }
            if (x == 63 && y == 21 && !inventory->hasGemmeForce(63)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 63*16 + 8, 21*16 + 8, 63));
                return true;
            }
            if (x == 112 && y == 33 && !inventory->hasGemmeForce(64)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 112*16 + 8, 33*16 + 8, 64));
                return true;
            }
            if (x == 149 && y == 21 && !inventory->hasGemmeForce(65)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 149*16 + 8, 21*16 + 8, 65));
                return true;
            }
            if (x == 21 && y == 80 && !inventory->hasGemmeForce(66)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 21*16 + 8, 80*16 + 8, 66));
                return true;
            }
            if (x == 54 && y == 43 && !inventory->hasGemmeForce(67)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 54*16 + 8, 43*16 + 8, 67));
                return true;
            }
            if (x == 155 && y == 84 && !inventory->hasGemmeForce(68)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 155*16 + 8, 84*16 + 8, 68));
                return true;
            }
            if (x == 89 && y == 71 && !inventory->hasGemmeForce(69)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 89*16 + 8, 71*16 + 8, 69));
                return true;
            }
            break;
        case 15 :
            if (x == 11 && y == 4 && !inventory->hasGemmeForce(70)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 11*16 + 8, 4*16 + 2, 70));
                return true;
            }
            break;
        case 16 :
            if (x == 15 && y == 12 && !inventory->hasGemmeForce(71)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 15*16 + 8, 12*16 + 2, 71));
                return true;
            }
            break;
        case 18 :
            if (x == 4 && y == 12 && !inventory->hasGemmeForce(72)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 4*16 + 8, 12*16 + 2, 72));
                return true;
            }
            break;
        case 20 :
            if (x == 5 && y == 19 && !inventory->hasGemmeForce(73)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 5*16 + 8, 19*16 + 2, 73));
                return true;
            }
            if (x == 55 && y == 4 && !inventory->hasGemmeForce(74)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 55*16 + 8, 4*16 + 2, 74));
                return true;
            }
            break;
        case 22 :
            if (x == 13 && y == 2 && !inventory->hasGemmeForce(75)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 13*16 + 8, 2*16 + 2, 75));
                return true;
            }
            break;
        case 23 :
            if (x == 13 && y == 9 && !inventory->hasGemmeForce(76)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 13*16 + 8, 9*16 + 2, 76));
                return true;
            }
            break;
        case 24 :
            if (x == 15 && y == 12 && !inventory->hasGemmeForce(77)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 15*16 + 8, 12*16 + 2, 77));
                return true;
            }
            break;
        case 25 :
            if (x == 5 && y == 40 && !inventory->hasGemmeForce(78)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 5*16 + 8, 40*16 + 2, 78));
                return true;
            }
            if (x == 31 && y == 41 && !inventory->hasGemmeForce(79)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 31*16 + 8, 41*16 + 2, 79));
                return true;
            }
            break;
        case 26 :
            if (x == 5 && y == 4 && !inventory->hasGemmeForce(80)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 5*16 + 8, 4*16 + 2, 80));
                return true;
            }
            break;
        case 27 :
            if (x == 7 && y == 34 && !inventory->hasGemmeForce(81)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 7*16 + 8, 34*16 + 2, 81));
                return true;
            }
            break;
        case 28 :
            if (x == 7 && y == 19 && !inventory->hasGemmeForce(82)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 7*16 + 8, 19*16 + 2, 82));
                return true;
            }
            break;
            // gemmes 83 to 87 are on the map 34 (chests game)
        case 38 :
            if (x == 4 && y == 12 && !inventory->hasGemmeForce(88)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 4*16 + 8, 12*16 + 2, 88));
                return true;
            }
            break;
            // gemme 89 is on map 43
        case 44 :
            if (x == 14 && y == 27 && !inventory->hasGemmeForce(90)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 14*16 + 8, 27*16 + 2, 90));
                return true;
            }
            break;
        case 52 :
            if (x == 15 && y == 12 && !inventory->hasGemmeForce(91)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 15*16 + 8, 12*16 + 2, 91));
                return true;
            }
            break;
        case 55 :
            if (x == 4 && y == 12 && !inventory->hasGemmeForce(92)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 4*16 + 8, 12*16 + 2, 92));
                return true;
            }
            break;
        case 58 :
            if (x == 208 && y == 33 && !inventory->hasGemmeForce(93)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 208*16 + 8, 33*16 + 2, 93));
                return true;
            }
            break;
        case 59 :
            if (x == 118 && y == 128 && !inventory->hasGemmeForce(94)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 118*16 + 8, 128*16 + 2, 94));
                return true;
            }
            break;
        case 60 :
            if (x == 95 && y == 104 && !inventory->hasGemmeForce(95)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 95*16 + 8, 104*16 + 2, 95));
                return true;
            }
            break;
        case 61 :
            if (x == 9 && y == 137 && !inventory->hasGemmeForce(96)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 9*16 + 8, 137*16 + 2, 96));
                return true;
            }
            break;
        case 62 :
            if (x == 57 && y == 17 && !inventory->hasGemmeForce(97)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 57*16 + 8, 17*16 + 2, 97));
                return true;
            }
            break;
        case 63 :
            if (x == 26 && y == 56 && !inventory->hasGemmeForce(98)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 26*16 + 8, 56*16 + 2, 98));
                return true;
            }
            break;
        case 64 :
            if (x == 67 && y == 32 && !inventory->hasGemmeForce(99)) {
                addItem(ItemHelper::getInstance()->createItem(TI_GEMME_FORCE, 67*16 + 8, 32*16 + 2, 99));
                return true;
            }
            break;
    }
    return false;
}

int Map::nbGemmeForce() {

    if (nbGemmeForce(map) > 0) {
        return 2;
    }

    int total = 0;

    switch (map) {
        case 1 :
            total = nbGemmeForce(2) + nbGemmeForce(3) + nbGemmeForce(8) + nbGemmeForce(15);
            break;
        case 2 :
            total = nbGemmeForce(1) + nbGemmeForce(3) + nbGemmeForce(5) + nbGemmeForce(8)
                + nbGemmeForce(9) + nbGemmeForce(13) + nbGemmeForce(16) + nbGemmeForce(58);
            break;
        case 3 :
            total = nbGemmeForce(1) + nbGemmeForce(2) + nbGemmeForce(4) + nbGemmeForce(31)
                + nbGemmeForce(32) + nbGemmeForce(33) + nbGemmeForce(34) + nbGemmeForce(35)
                + nbGemmeForce(36) + nbGemmeForce(37) + nbGemmeForce(38) + nbGemmeForce(39);
            break;
        case 4 :
            total = nbGemmeForce(3) + nbGemmeForce(5) + nbGemmeForce(6) + nbGemmeForce(17)
                + nbGemmeForce(18) + nbGemmeForce(61);
            break;
        case 5 :
            total = nbGemmeForce(2) + nbGemmeForce(4) + nbGemmeForce(6) + nbGemmeForce(7)
                + nbGemmeForce(10) + nbGemmeForce(19) + nbGemmeForce(40);
            break;
        case 6 :
            total = nbGemmeForce(4) + nbGemmeForce(5) + nbGemmeForce(20) + nbGemmeForce(21)
                + nbGemmeForce(59);
            break;
        case 7 :
            total = nbGemmeForce(5) + nbGemmeForce(10) + nbGemmeForce(11) + nbGemmeForce(41)
                + nbGemmeForce(42) + nbGemmeForce(43) + nbGemmeForce(44) + nbGemmeForce(45)
                + nbGemmeForce(46) + nbGemmeForce(47);
            break;
        case 8 :
            total = nbGemmeForce(1) + nbGemmeForce(2) + nbGemmeForce(12) + nbGemmeForce(48);
            break;
        case 9 :
            total = nbGemmeForce(2) + nbGemmeForce(10) + nbGemmeForce(11) + nbGemmeForce(13);
            break;
        case 10 :
            total = nbGemmeForce(5) + nbGemmeForce(7) + nbGemmeForce(9) + nbGemmeForce(22)
                + nbGemmeForce(23);
            break;
        case 11 :
            total = nbGemmeForce(7) + nbGemmeForce(9) + nbGemmeForce(24) + nbGemmeForce(25)
                + nbGemmeForce(26) + nbGemmeForce(60);
            break;
        case 12 :
            total = nbGemmeForce(8) + nbGemmeForce(13) + nbGemmeForce(14) + nbGemmeForce(49)
                + nbGemmeForce(50) + nbGemmeForce(51) + nbGemmeForce(52) + nbGemmeForce(53)
                + nbGemmeForce(54) + nbGemmeForce(55) + nbGemmeForce(56) + nbGemmeForce(57);
            break;
        case 13 :
            total = nbGemmeForce(2) + nbGemmeForce(9) + nbGemmeForce(12) + nbGemmeForce(27)
                + nbGemmeForce(63);
            break;
        case 14 :
            total = nbGemmeForce(11) + nbGemmeForce(12) + nbGemmeForce(28) + nbGemmeForce(29)
                + nbGemmeForce(30) + nbGemmeForce(62) + nbGemmeForce(64);
            break;
        case 15 : total = nbGemmeForce(1); break;
        case 16 : total = nbGemmeForce(2); break;
        case 17 : case 18 : total = nbGemmeForce(4); break;
        case 19 : total = nbGemmeForce(5); break;
        case 20 : case 21 : total = nbGemmeForce(6); break;
        case 22 : case 23 : total = nbGemmeForce(10); break;
        case 24 : case 25 : total = nbGemmeForce(11); break;
        case 27 : total = nbGemmeForce(13); break;
        case 28 : case 29 : case 30 : total = nbGemmeForce(14); break;
        case 31 : case 32 : case 33 : case 34 : case 35 : case 36 : case 37 : case 38 : case 39 : total = nbGemmeForce(3); break;
        case 40 : total = nbGemmeForce(5); break;
        case 41 : case 42 : case 43 : case 44 : case 45 : case 46 : case 47 : total = nbGemmeForce(7); break;
        case 48 : total = nbGemmeForce(8); break;
        case 49 : case 50 : case 51 : case 52 : case 53 : case 54 : case 55 : case 56 : case 57 : total = nbGemmeForce(12); break;
        case 58 : total = nbGemmeForce(2); break;
        case 59 : total = nbGemmeForce(6); break;
        case 60 : total = nbGemmeForce(11); break;
        case 61 : total = nbGemmeForce(4); break;
        case 62 : total = nbGemmeForce(14); break;
        case 63 : total = nbGemmeForce(13); break;
        case 64 : total = nbGemmeForce(14); break;
        case 66 : total = nbGemmeForce(30); break;
    }

    return total > 0 ? 1 : 0;
}

int Map::nbGemmeForce(int mapId) {

    Inventory* inventory = MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->getLink()->getInventory();

    int total = 0;
    int current = 0;

    switch (mapId) {
        case 1 :
            total = 4;
            if (inventory->hasGemmeForce(0)) current++;
            if (inventory->hasGemmeForce(1)) current++;
            if (inventory->hasGemmeForce(2)) current++;
            if (inventory->hasGemmeForce(3)) current++;
            break;
        case 2 :
            total = 4;
            if (inventory->hasGemmeForce(4)) current++;
            if (inventory->hasGemmeForce(5)) current++;
            if (inventory->hasGemmeForce(6)) current++;
            if (inventory->hasGemmeForce(7)) current++;
            break;
        case 3 :
            total = 4;
            if (inventory->hasGemmeForce(8)) current++;
            if (inventory->hasGemmeForce(9)) current++;
            if (inventory->hasGemmeForce(10)) current++;
            if (inventory->hasGemmeForce(11)) current++;
            break;
        case 4 :
            total = 4;
            if (inventory->hasGemmeForce(12)) current++;
            if (inventory->hasGemmeForce(13)) current++;
            if (inventory->hasGemmeForce(14)) current++;
            if (inventory->hasGemmeForce(15)) current++;
            break;
        case 5 :
            total = 6;
            if (inventory->hasGemmeForce(16)) current++;
            if (inventory->hasGemmeForce(17)) current++;
            if (inventory->hasGemmeForce(18)) current++;
            if (inventory->hasGemmeForce(19)) current++;
            if (inventory->hasGemmeForce(20)) current++;
            if (inventory->hasGemmeForce(21)) current++;
            break;
        case 6 :
            total = 3;
            if (inventory->hasGemmeForce(22)) current++;
            if (inventory->hasGemmeForce(23)) current++;
            if (inventory->hasGemmeForce(24)) current++;
            break;
        case 7 :
            total = 9;
            if (inventory->hasGemmeForce(25)) current++;
            if (inventory->hasGemmeForce(26)) current++;
            if (inventory->hasGemmeForce(27)) current++;
            if (inventory->hasGemmeForce(28)) current++;
            if (inventory->hasGemmeForce(29)) current++;
            if (inventory->hasGemmeForce(30)) current++;
            if (inventory->hasGemmeForce(31)) current++;
            if (inventory->hasGemmeForce(32)) current++;
            if (inventory->hasGemmeForce(33)) current++;
            break;
        case 8 :
            total = 4;
            if (inventory->hasGemmeForce(34)) current++;
            if (inventory->hasGemmeForce(35)) current++;
            if (inventory->hasGemmeForce(36)) current++;
            if (inventory->hasGemmeForce(37)) current++;
            break;
        case 9 :
            total = 4;
            if (inventory->hasGemmeForce(38)) current++;
            if (inventory->hasGemmeForce(39)) current++;
            if (inventory->hasGemmeForce(40)) current++;
            if (inventory->hasGemmeForce(41)) current++;
            break;
        case 10 :
            total = 4;
            if (inventory->hasGemmeForce(42)) current++;
            if (inventory->hasGemmeForce(43)) current++;
            if (inventory->hasGemmeForce(44)) current++;
            if (inventory->hasGemmeForce(45)) current++;
            break;
        case 11 :
            total = 8;
            if (inventory->hasGemmeForce(46)) current++;
            if (inventory->hasGemmeForce(47)) current++;
            if (inventory->hasGemmeForce(48)) current++;
            if (inventory->hasGemmeForce(49)) current++;
            if (inventory->hasGemmeForce(50)) current++;
            if (inventory->hasGemmeForce(51)) current++;
            if (inventory->hasGemmeForce(52)) current++;
            if (inventory->hasGemmeForce(53)) current++;
            break;
        case 12 :
            total = 4;
            if (inventory->hasGemmeForce(54)) current++;
            if (inventory->hasGemmeForce(55)) current++;
            if (inventory->hasGemmeForce(56)) current++;
            if (inventory->hasGemmeForce(57)) current++;
            break;
        case 13 :
            total = 4;
            if (inventory->hasGemmeForce(58)) current++;
            if (inventory->hasGemmeForce(59)) current++;
            if (inventory->hasGemmeForce(60)) current++;
            if (inventory->hasGemmeForce(61)) current++;
            break;
        case 14 :
            total = 8;
            if (inventory->hasGemmeForce(62)) current++;
            if (inventory->hasGemmeForce(63)) current++;
            if (inventory->hasGemmeForce(64)) current++;
            if (inventory->hasGemmeForce(65)) current++;
            if (inventory->hasGemmeForce(66)) current++;
            if (inventory->hasGemmeForce(67)) current++;
            if (inventory->hasGemmeForce(68)) current++;
            if (inventory->hasGemmeForce(69)) current++;
            break;
        case 15 :
            total = 1;
            if (inventory->hasGemmeForce(70)) current++;
            break;
        case 16 :
            total = 1;
            if (inventory->hasGemmeForce(71)) current++;
            break;
        case 18 :
            total = 1;
            if (inventory->hasGemmeForce(72)) current++;
            break;
        case 20 :
            total = 2;
            if (inventory->hasGemmeForce(73)) current++;
            if (inventory->hasGemmeForce(74)) current++;
            break;
        case 22 :
            total = 1;
            if (inventory->hasGemmeForce(75)) current++;
            break;
        case 23 :
            total = 1;
            if (inventory->hasGemmeForce(76)) current++;
            break;
        case 24 :
            total = 1;
            if (inventory->hasGemmeForce(77)) current++;
            break;
        case 25 :
            total = 2;
            if (inventory->hasGemmeForce(78)) current++;
            if (inventory->hasGemmeForce(79)) current++;
            break;
        case 26 :
            total = 1;
            if (inventory->hasGemmeForce(80)) current++;
            break;
        case 27 :
            total = 1;
            if (inventory->hasGemmeForce(81)) current++;
            break;
        case 28 :
            total = 1;
            if (inventory->hasGemmeForce(82)) current++;
            break;
        case 34 :
            total = 5;
            if (inventory->hasGemmeForce(83)) current++;
            if (inventory->hasGemmeForce(84)) current++;
            if (inventory->hasGemmeForce(85)) current++;
            if (inventory->hasGemmeForce(86)) current++;
            if (inventory->hasGemmeForce(87)) current++;
            break;
        case 38 :
            total = 1;
            if (inventory->hasGemmeForce(88)) current++;
            break;
        case 43 :
            total = 1;
            if (inventory->hasGemmeForce(89)) current++;
            break;
        case 44 :
            total = 1;
            if (inventory->hasGemmeForce(90)) current++;
            break;
        case 52 :
            total = 1;
            if (inventory->hasGemmeForce(91)) current++;
            break;
        case 55 :
            total = 1;
            if (inventory->hasGemmeForce(92)) current++;
            break;
        case 58 : case 59 : case 60 : case 61 : case 62 : case 63 : case 64 :
            total = 1;
            if (inventory->hasGemmeForce(93 + mapId - 58)) current++;
            break;
    }

    return total - current;
}

void Map::pickUpItems(BoundingBox* box) {

    List toPickUp;
    items->get(box, &toPickUp);

    toPickUp.iterateOnFirst();
    while (toPickUp.hasNext()) {
        Item* i = (Item*) toPickUp.getNext();
        if (i->getBoundingBox()->intersect(box) && i->isAlive()) {
            i->action();
        }
    }
}

Item* Map::getItem(BoundingBox* box) {
    List toPickUp;
    items->get(box, &toPickUp);

    toPickUp.iterateOnFirst();
    while (toPickUp.hasNext()) {
        Item* i = (Item*) toPickUp.getNext();
        if (i->getBoundingBox()->intersect(box) && i->isReady() && i->isAlive() && i->isPickable()) {
            return i;
        }
    }
    return 0;
}

void Map::killItems(BoundingBox* box) {
    List toPickUp;
    items->get(box, &toPickUp);

    toPickUp.iterateOnFirst();
    while (toPickUp.hasNext()) {
        Item* i = (Item*) toPickUp.getNext();
        if (i->getBoundingBox()->intersect(box) && i->isAlive()) {
            i->killNode();
        }
    }
}

void Map::killEnnemis(BoundingBox* box) {
    List toKill;
    ennemis->get(box, &toKill);

    toKill.iterateOnFirst();
    while (toKill.hasNext()) {
        Ennemi* e = (Ennemi*) toKill.getNext();
        if (e->getBoundingBox()->intersect(box) && e->isAlive()) {
            e->killNode();
        }
    }
}

Portable* Map::pickUpObject(BoundingBox* box) {

    List toPickUp;
    objects->get(box, &toPickUp);

    toPickUp.iterateOnFirst();
    while (toPickUp.hasNext()) {
        Portable* p = dynamic_cast<Portable*>(toPickUp.getNext());
        if (p != 0 && p->isCarriable() && p->getHandableBox()->intersect(box)) {
            return p;
        }
    }

    // tas de pierres
    for (int j = box->getY(); j < box->getY() + box->getH(); j += 8) {
        for (int i = box->getX(); i < box->getX() + box->getW(); i += 8) {
            short s = getSol(i, j);
            if (s >= 1833 && s <= 1836) {
                Portable* p = new Pierre(i - (i%16), j - (j%16), 3, this, false);
                addObject(p);
                return p;
            }
        }
    }

    return 0;
}

void Map::pushObject(BoundingBox* box, Direction dir) {
    List toPush;
    objects->get(box, &toPush);

    toPush.iterateOnFirst();
    while (toPush.hasNext()) {
        Poussable* p = dynamic_cast<Poussable*>(toPush.getNext());
        if (p != 0) {
            p->pousse(dir);
            return;
        }
    }
}

bool Map::tryToTalk(BoundingBox* box, Direction dir) {
    List toTalk;

    // try to interact with objects or npcs
    objects->get(box, &toTalk);

    toTalk.iterateOnFirst();
    while (toTalk.hasNext()) {
        Object* p = dynamic_cast<Object*>(toTalk.getNext());
        if (p != 0 && p->action(dir)) {
            return true;
        }
    }
    return false;
}

bool Map::testTransition(int x, int y) {
    if (map > 14) return false;
    return ((x <= 8 && map != 1 && map != 8 && map != 12 && map != 14) ||
            (y <= 0 && map != 10 && map != 11 && map != 13 && map != 14) ||
            (x >= w - 24 && map != 5 && map != 6 && map != 7 && map != 11) ||
            (y >= h - 32 && map != 1 && map != 3 && map != 4 && map != 6));
}

bool Map::testInnerTransition(int x, int y) {
    if (map < 15) return false;
    return ((x <= bounds.getX() + 8 && bounds.getX() > 0) ||
            (y <= bounds.getY() && bounds.getY() > 0) ||
            (x >= bounds.getX() + bounds.getW() - 24 && bounds.getX() + bounds.getW() < w) ||
            (y >= bounds.getY() + bounds.getH() - 32 && bounds.getY() + bounds.getH() < h));
}

bool Map::testTeleport(int x, int y) {
    switch (map) {
        case 1 :
            if (x == 41 * 16 + 8 && y == 4 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(15, 9 * 16 + 8, 42 * 16, N, false, true);
                return true;
            }
            break;
        case 2 :
            if (x == 13 * 16 + 8 && y == 10 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(16, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 105 * 16 + 8 && y == 8 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(58, 49 * 16 + 8, 72 * 16, N, false, true);
                return true;
            }
            break;
        case 3 :
            if (x == 19 * 16 + 8 && y == 43 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(31, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 36 * 16 + 8 && y == 37 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(32, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 36 * 16 + 8 && y == 48 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(33, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 50 * 16 + 8 && y == 49 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(34, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 62 * 16 + 8 && y == 32 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(35, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 69 * 16 + 8 && y == 24 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(36, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 59 * 16 + 8 && y == 23 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(37, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 65 * 16 + 8 && y == 13 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(38, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 54 * 16 + 8 && y == 14 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(39, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            break;
        case 4 :
            if (x == 5 * 16 + 8 && y == 33 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(17, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 70 * 16 + 8 && y == 2 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(18, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 27 * 16 + 8 && y == 42 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(61, 49 * 16 + 8, 222 * 16, N, false, true);
                return true;
            }
            if (x == 40 * 16 + 8 && y == 39 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(61, 89 * 16 + 8, 207 * 16, N, false, true);
                return true;
            }
            if (x == 34 * 16 + 8 && y == 21 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(61, 69 * 16 + 8, 177 * 16, N, false, true);
                return true;
            }
            break;
        case 5 :
            if (x == 145 * 16 + 8 && y == 17 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(19, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 41 * 16 + 8 && y == 25 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(40, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            break;
        case 6 :
            if (x == 10 * 16 + 8 && y == 40 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(20, 9 * 16 + 8, 27 * 16, N, false, true);
                return true;
            }
            if (x == 44 * 16 + 8 && y == 32 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(20, 49 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 67 * 16 + 8 && y == 46 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(21, 9 * 16 + 8, 27 * 16, N, false, true);
                return true;
            }
            if (x == 19 * 16 && y == 16 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(59, 69 * 16 + 8, 222 * 16, N, false, true);
                return true;
            }
            break;
        case 7 :
            if (x == 17 * 16 + 8 && y == 26 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(41, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 28 * 16 + 8 && y == 26 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(42, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 39 * 16 + 8 && y == 26 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(43, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 31 * 16 + 8 && y == 18 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(44, 9 * 16 + 8, 27 * 16, N, false, true);
                return true;
            }
            if (x == 39 * 16 + 8 && y == 18 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(45, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 53 * 16 + 8 && y == 20 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(46, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 61 * 16 + 8 && y == 20 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(47, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            break;
        case 8 :
            if (x == 58 * 16 + 8 && y == 34 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(48, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            break;
        case 9 :
            if (x == 39 * 16 + 8 && y == 27 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(65, 49 * 16 + 8, 72 * 16, N, false, true);
                return true;
            }
            break;
        case 10 :
            if (x == 15 * 16 + 8 && y == 2 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(22, 9 * 16 + 8, 27 * 16, N, false, true);
                return true;
            }
            if (x == 52 * 16 + 8 && y == 29 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(23, 9 * 16 + 8, 42 * 16, N, false, true);
                return true;
            }
            break;
        case 11 :
            if (x == 20 * 16 + 8 && y == 69 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(24, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 52 * 16 + 8 && y == 21 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(25, 9 * 16 + 8, 42 * 16, N, false, true);
                return true;
            }
            if (x == 185 * 16 + 8 && y == 17 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(26, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x >= 20 * 16 && x<= 21 * 16 && y == 27 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(60, 49 * 16 + 8, 117 * 16, N, false, true);
                return true;
            }
            break;
        case 12 :
            if (x == 14 * 16 + 8 && y == 12 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(49, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 22 * 16 + 8 && y == 12 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(50, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 30 * 16 + 8 && y == 12 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(51, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 48 * 16 + 8 && y == 12 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(52, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 59 * 16 + 8 && y == 12 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(53, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 70 * 16 + 8 && y == 12 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(54, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 52 * 16 + 8 && y == 36 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(55, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 64 * 16 + 8 && y == 40 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(56, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 56 * 16 + 8 && y == 48 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(57, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            break;
        case 13 :
            if (x == 28 * 16 + 8 && y == 9 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(27, 9 * 16 + 8, 42 * 16, N, false, true);
                return true;
            }
            if (x == 10 * 16 + 8 && y == 11 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(63, 9 * 16 + 8, 222 * 16, N, false, true);
                return true;
            }
            if (x == 43 * 16 + 8 && y == 8 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(63, 109 * 16 + 8, 222 * 16, N, false, true);
                return true;
            }
            break;
        case 14 :
            if (x == 114 * 16 + 8 && y == 70 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(28, 9 * 16 + 8, 27 * 16, N, false, true);
                return true;
            }
            if (x == 151 * 16 + 8 && y == 74 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(28, 29 * 16 + 8, 27 * 16, N, false, true);
                return true;
            }
            if (x == 86 * 16 + 8 && y == 62 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(29, 9 * 16 + 8, 27 * 16, N, false, true);
                return true;
            }
            if (x == 57 * 16 + 8 && y == 69 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(30, 9 * 16 + 8, 12 * 16, N, false, true);
                return true;
            }
            if (x == 26 * 16 && y == 25 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(62, 49 * 16 + 8, 222 * 16, N, false, true);
                return true;
            }
            if (x == 133 * 16 + 8 && y == 27 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(64, 209 * 16 + 8, 42 * 16, N, false, true);
                return true;
            }
            break;
        case 15 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(1, 41 * 16 + 8, 5 * 16, S, false, true);
                return true;
            }
            break;
        case 16 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(2, 13 * 16 + 8, 11 * 16, S, false, true);
                return true;
            }
            break;
        case 17 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(4, 5 * 16 + 8, 34 * 16, S, false, true);
                return true;
            }
            break;
        case 18 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(4, 70 * 16 + 8, 3 * 16, S, false, true);
                return true;
            }
            break;
        case 19 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(5, 145 * 16 + 8, 18 * 16, S, false, true);
                return true;
            }
            break;
        case 20 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(6, 10 * 16 + 8, 41 * 16, S, false, true);
                return true;
            }
            if (y >= 240 - 32 && x > 640) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(6, 44 * 16 + 8, 33 * 16, S, false, true);
                return true;
            }
            break;
        case 21 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(6, 67 * 16 + 8, 47 * 16, S, false, true);
                return true;
            }
            break;
        case 22 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(10, 15 * 16 + 8, 3 * 16, S, false, true);
                return true;
            }
            break;
        case 23 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(10, 52 * 16 + 8, 30 * 16, S, false, true);
                return true;
            }
            break;
        case 24 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(11, 20 * 16 + 8, 70 * 16, S, false, true);
                return true;
            }
            break;
        case 25 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(11, 52 * 16 + 8, 22 * 16, S, false, true);
                return true;
            }
            break;
        case 26 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(11, 185 * 16 + 8, 18 * 16, S, false, true);
                return true;
            }
            break;
        case 27 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(13, 28 * 16 + 8, 10 * 16, S, false, true);
                return true;
            }
            break;
        case 28 :
            if (y >= h - 32 && x >= 320) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(14, 151 * 16 + 8, 75 * 16, S, false, true);
                return true;
            }
            if (y >= h - 32 && x < 320) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(14, 114 * 16 + 8, 71 * 16, S, false, true);
                return true;
            }
            break;
        case 29 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(14, 86 * 16 + 8, 63 * 16, S, false, true);
                return true;
            }
            break;
        case 30 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(14, 57 * 16 + 8, 70 * 16, S, false, true);
                return true;
            }
            if (y >= 6 * 16 && y <= 7 * 16 && x >= 8 * 16 + 8 && x <= 10 * 16 + 8 &&
                MainController::getInstance()->getGameController()->getSceneController()->getScene()->getCoffre(0, 9)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(66, 29 * 16 + 8, 55 * 16 + 8, N, false, true);
                AudioManager::getInstance()->playSound(TS_TELEPORT);
                return true;
            }
            break;
        case 31 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(3, 19 * 16 + 8, 43 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 32 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(3, 36 * 16 + 8, 37 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 33 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(3, 36 * 16 + 8, 48 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 34 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(3, 50 * 16 + 8, 49 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 35 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(3, 62 * 16 + 8, 32 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 36 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(3, 69 * 16 + 8, 24 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 37 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(3, 59 * 16 + 8, 23 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 38 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(3, 65 * 16 + 8, 13 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 39 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(3, 54 * 16 + 8, 14 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 40 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(5, 41 * 16 + 8, 25 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 41 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(7, 17 * 16 + 8, 26 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 42 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(7, 28 * 16 + 8, 26 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 43 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(7, 39 * 16 + 8, 26 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 44 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(7, 31 * 16 + 8, 18 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 45 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(7, 39 * 16 + 8, 18 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 46 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(7, 53 * 16 + 8, 20 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 47 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(7, 61 * 16 + 8, 20 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 48 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(8, 58 * 16 + 8, 35 * 16, S, false, true);
                return true;
            }
            break;
        case 49 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(12, 14 * 16 + 8, 12 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 50 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(12, 22 * 16 + 8, 12 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 51 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(12, 30 * 16 + 8, 12 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 52 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(12, 48 * 16 + 8, 12 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 53 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(12, 59 * 16 + 8, 12 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 54 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(12, 70 * 16 + 8, 12 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 55 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(12, 52 * 16 + 8, 36 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 56 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(12, 64 * 16 + 8, 40 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 57 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(12, 56 * 16 + 8, 48 * 16 + 8, S, false, true);
                return true;
            }
            break;
        case 58 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(2, 105 * 16 + 8, 8 * 16 + 8, S, false, true);
                return true;
            }
            // escaliers
            if ((x == 9 * 16 + 8 && y == 51 * 16)
                || (x == 9 * 16 + 8 && y == 66 * 16)
                || (x == 29 * 16 + 8 && y == 6 * 16)
                || (x == 69 * 16 + 8 && y == 6 * 16)
                || (x == 69 * 16 + 8 && y == 66 * 16)
                || (x == 89 * 16 + 8 && y == 6 * 16)
                || (x == 89 * 16 + 8 && y == 66 * 16)
                || (x == 109 * 16 + 8 && y == 6 * 16)
                || (x == 129 * 16 + 8 && y == 66 * 16)
                || (x == 189 * 16 + 8 && y == 51 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(58, x + 100 * 16, y + 24, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 109 * 16 + 8 && y == 51 * 16)
                || (x == 109 * 16 + 8 && y == 66 * 16)
                || (x == 129 * 16 + 8 && y == 6 * 16)
                || (x == 169 * 16 + 8 && y == 6 * 16)
                || (x == 169 * 16 + 8 && y == 66 * 16)
                || (x == 189 * 16 + 8 && y == 6 * 16)
                || (x == 189 * 16 + 8 && y == 66 * 16)
                || (x == 209 * 16 + 8 && y == 6 * 16)
                || (x == 229 * 16 + 8 && y == 66 * 16)
                || (x == 289 * 16 + 8 && y == 51 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(58, x - 100 * 16, y - 24, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            break;
        case 59 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(6, 19 * 16, 16 * 16 + 8, S, false, true);
                return true;
            }
            // escaliers
            if ((x == 9 * 16 + 8 && y == 111 * 16)
                || (x == 9 * 16 + 8 && y == 156 * 16)
                || (x == 9 * 16 + 8 && y == 201 * 16)
                || (x == 9 * 16 + 8 && y == 216 * 16)
                || (x == 29 * 16 + 8 && y == 81 * 16)
                || (x == 29 * 16 + 8 && y == 126 * 16)
                || (x == 29 * 16 + 8 && y == 171 * 16)
                || (x == 29 * 16 + 8 && y == 186 * 16)
                || (x == 49 * 16 + 8 && y == 141 * 16)
                || (x == 49 * 16 + 8 && y == 156 * 16)
                || (x == 49 * 16 + 8 && y == 171 * 16)
                || (x == 49 * 16 + 8 && y == 186 * 16)
                || (x == 69 * 16 + 8 && y == 81 * 16)
                || (x == 69 * 16 + 8 && y == 126 * 16)
                || (x == 69 * 16 + 8 && y == 186 * 16)
                || (x == 69 * 16 + 8 && y == 216 * 16)
                || (x == 89 * 16 + 8 && y == 96 * 16)
                || (x == 89 * 16 + 8 && y == 126 * 16)
                || (x == 89 * 16 + 8 && y == 141 * 16)
                || (x == 109 * 16 + 8 && y == 96 * 16)
                || (x == 109 * 16 + 8 && y == 186 * 16)
                || (x == 109 * 16 + 8 && y == 201 * 16)
                || (x == 129 * 16 + 8 && y == 156 * 16)
                || (x == 129 * 16 + 8 && y == 186 * 16)
                || (x == 129 * 16 + 8 && y == 201 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(59, x, y + 24 - 75 * 16, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 9 * 16 + 8 && y == 36 * 16)
                || (x == 9 * 16 + 8 && y == 81 * 16)
                || (x == 9 * 16 + 8 && y == 126 * 16)
                || (x == 9 * 16 + 8 && y == 141 * 16)
                || (x == 29 * 16 + 8 && y == 6 * 16)
                || (x == 29 * 16 + 8 && y == 51 * 16)
                || (x == 29 * 16 + 8 && y == 96 * 16)
                || (x == 29 * 16 + 8 && y == 111 * 16)
                || (x == 49 * 16 + 8 && y == 66 * 16)
                || (x == 49 * 16 + 8 && y == 81 * 16)
                || (x == 49 * 16 + 8 && y == 96 * 16)
                || (x == 49 * 16 + 8 && y == 111 * 16)
                || (x == 69 * 16 + 8 && y == 6 * 16)
                || (x == 69 * 16 + 8 && y == 51 * 16)
                || (x == 69 * 16 + 8 && y == 111 * 16)
                || (x == 69 * 16 + 8 && y == 141 * 16)
                || (x == 89 * 16 + 8 && y == 21 * 16)
                || (x == 89 * 16 + 8 && y == 51 * 16)
                || (x == 89 * 16 + 8 && y == 66 * 16)
                || (x == 109 * 16 + 8 && y == 21 * 16)
                || (x == 109 * 16 + 8 && y == 111 * 16)
                || (x == 109 * 16 + 8 && y == 126 * 16)
                || (x == 129 * 16 + 8 && y == 81 * 16)
                || (x == 129 * 16 + 8 && y == 111 * 16)
                || (x == 129 * 16 + 8 && y == 126 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(59, x, y - 24 + 75 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            break;
        case 60 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(11, 20 * 16 + 8, 28 * 16 + 8, S, false, true);
                return true;
            }
            // escaliers
            if (x == 65 * 16 + 8 && y == 6 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(60, x - 40 * 16, y + 24 + 15 * 16, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 9 * 16 + 8 && y == 4 * 16)
                || (x == 49 * 16 + 8 && y == 36 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(60, x + 20 * 16, y + 24 + 60 * 16, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 9 * 16 + 8 && y == 111 * 16)
                || (x == 89 * 16 + 8 && y == 51 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(60, x + 120 * 16, y + 24 - 15 * 16, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if (x == 25 * 16 + 8 && y == 21 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(60, x + 40 * 16, y - 24 - 15 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            if ((x == 29 * 16 + 8 && y == 64 * 16)
                || (x == 69 * 16 + 8 && y == 96 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(60, x - 20 * 16, y - 24 - 60 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            if ((x == 129 * 16 + 8 && y == 96 * 16)
                || (x == 209 * 16 + 8 && y == 36 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(60, x - 120 * 16, y - 24 + 15 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            break;
        case 61 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(4, 27 * 16 + 8, 43 * 16 + 8, S, false, true);
                return true;
            }
            if (y >= h - 240 - 32 && bounds.getX() == 320 * 4) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(4, 40 * 16 + 8, 40 * 16 + 8, S, false, true);
                return true;
            }
            if (y >= h - 240*3 - 32 && bounds.getX() == 320 * 3) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(4, 34 * 16 + 8, 22 * 16 + 8, S, false, true);
                return true;
            }

            // escaliers
            if ((x == 29 * 16 + 8 && y == 216 * 16)
                || (x == 49 * 16 + 8 && y == 171 * 16)
                || (x == 89 * 16 + 8 && y == 171 * 16)
                || (x == 109 * 16 + 8 && y == 201 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(61, x + (eau1Up ? 0 : 140 * 16), y + 24 - 75 * 16, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 149 * 16 + 8 && y == 128 * 16)
                || (x == 209 * 16 + 8 && y == 111 * 16)
                || (x == 269 * 16 + 8 && y == 141 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(61, x - (eau2Up ? 140 * 16 : 0), y + 24 - 75 * 16, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 9 * 16 + 8 && y == 53 * 16)
                || (x == 69 * 16 + 8 && y == 36 * 16)
                || (x == 129 * 16 + 8 && y == 66 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(61, x + (eau1Up ? 0 : 140 * 16), y - 24 + 75 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            if ((x == 149 * 16 + 8 && y == 53 * 16)
                || (x == 209 * 16 + 8 && y == 36 * 16)
                || (x == 269 * 16 + 8 && y == 66 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(61, x - (eau1Up ? 140 * 16 : 0), y - 24 + 75 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            if ((x == 29 * 16 + 8 && y == 141 * 16)
                || (x == 49 * 16 + 8 && y == 96 * 16)
                || (x == 89 * 16 + 8 && y == 96 * 16)
                || (x == 109 * 16 + 8 && y == 126 * 16)
                || (x == 169 * 16 + 8 && y == 141 * 16)
                || (x == 189 * 16 + 8 && y == 96 * 16)
                || (x == 229 * 16 + 8 && y == 96 * 16)
                || (x == 249 * 16 + 8 && y == 126 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(61, x - (eau1Up ? 0 : 140 * 16), y - 24 + 75 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            break;
        case 62 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(14, 26 * 16, 26 * 16 + 8, S, false, true);
                return true;
            }
            // escaliers
            if ((x == 29 * 16 + 8 && y == 126 * 16)
                || (x == 29 * 16 + 8 && y == 186 * 16)
                || (x == 29 * 16 + 8 && y == 216 * 16)
                || (x == 49 * 16 + 8 && y == 81 * 16)
                || (x == 69 * 16 + 8 && y == 216 * 16)
                || (x == 89 * 16 + 8 && y == 81 * 16)
                || (x == 89 * 16 + 8 && y == 141 * 16)
                || (x == 109 * 16 + 8 && y == 216 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(62, x, y + 24 - 75 * 16, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 29 * 16 + 8 && y == 51 * 16)
                || (x == 29 * 16 + 8 && y == 111 * 16)
                || (x == 29 * 16 + 8 && y == 141 * 16)
                || (x == 49 * 16 + 8 && y == 6 * 16)
                || (x == 69 * 16 + 8 && y == 141 * 16)
                || (x == 89 * 16 + 8 && y == 6 * 16)
                || (x == 89 * 16 + 8 && y == 66 * 16)
                || (x == 109 * 16 + 8 && y == 141 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(62, x, y - 24 + 75 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            break;
        case 63 :
            if (y >= h - 32 && x < 320) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(13, 10 * 16 + 8, 12 * 16, S, false, true);
                return true;
            }
            if (y >= h - 32 && x > 320) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(13, 43 * 16 + 8, 9 * 16, S, false, true);
                return true;
            }
            // escaliers
            if ((x == 9 * 16 + 8 && y == 141 * 16)
                || (x == 9 * 16 + 8 && y == 186 * 16)
                || (x == 29 * 16 + 8 && y == 171 * 16)
                || (x == 29 * 16 + 8 && y == 186 * 16)
                || (x == 49 * 16 + 8 && y == 111 * 16)
                || (x == 49 * 16 + 8 && y == 126 * 16)
                || (x == 89 * 16 + 8 && y == 126 * 16)
                || (x == 89 * 16 + 8 && y == 216 * 16)
                || (x == 109 * 16 + 8 && y == 81 * 16)
                || (x == 109 * 16 + 8 && y == 141 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(63, x, y + 24 - 75 * 16, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 9 * 16 + 8 && y == 66 * 16)
                || (x == 9 * 16 + 8 && y == 111 * 16)
                || (x == 29 * 16 + 8 && y == 96 * 16)
                || (x == 29 * 16 + 8 && y == 111 * 16)
                || (x == 49 * 16 + 8 && y == 36 * 16)
                || (x == 49 * 16 + 8 && y == 51 * 16)
                || (x == 89 * 16 + 8 && y == 51 * 16)
                || (x == 89 * 16 + 8 && y == 141 * 16)
                || (x == 109 * 16 + 8 && y == 6 * 16)
                || (x == 109 * 16 + 8 && y == 66 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(63, x, y - 24 + 75 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            break;
        case 64 :
            if (y >= 240 * 3 - 32 && y <= 240 * 5 && x > 320 * 10 && x < 320 * 11) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(14, 133 * 16 + 8, 27 * 16 + 8, S, false, true);
                return true;
            }
            // escaliers
            if ((x == 9 * 16 + 8 && y == 81 * 16)
                || (x == 9 * 16 + 8 && y == 141 * 16)
                || (x == 29 * 16 + 8 && y == 126 * 16)
                || (x == 89 * 16 + 8 && y == 111 * 16)
                || (x == 109 * 16 + 8 && y == 141 * 16)
                || (x == 129 * 16 + 8 && y == 81 * 16)
                || (x == 129 * 16 + 8 && y == 96 * 16)
                || (x == 249 * 16 + 8 && y == 141 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(64, x, y + 24 - 75 * 16, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 149 * 16 + 8 && y == 51 * 16)
                || (x == 189 * 16 + 8 && y == 21 * 16)
                || (x == 229 * 16 + 8 && y == 21 * 16)
                || (x == 269 * 16 + 8 && y == 66 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(64, x - 140 * 16, y + 24 + 75 * 16, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 9 * 16 + 8 && y == 6 * 16)
                || (x == 9 * 16 + 8 && y == 66 * 16)
                || (x == 29 * 16 + 8 && y == 51 * 16)
                || (x == 89 * 16 + 8 && y == 36 * 16)
                || (x == 109 * 16 + 8 && y == 66 * 16)
                || (x == 129 * 16 + 8 && y == 6 * 16)
                || (x == 129 * 16 + 8 && y == 21 * 16)
                || (x == 249 * 16 + 8 && y == 66 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(64, x, y - 24 + 75 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            if ((x == 9 * 16 + 8 && y == 126 * 16)
                || (x == 49 * 16 + 8 && y == 96 * 16)
                || (x == 89 * 16 + 8 && y == 96 * 16)
                || (x == 129 * 16 + 8 && y == 141 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(64, x + 140 * 16, y - 24 - 75 * 16, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            break;
        case 65 :
            if (y >= h - 32) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(9, 39 * 16 + 8, 28 * 16, S, false, true);
                return true;
            }
            // escaliers
            if (x == 149 * 16 + 8 && y == 51 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(65, x - 100 * 16, y + 24, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if (x == 49 * 16 + 8 && y == 51 * 16) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(65, x + 100 * 16, y - 24, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            break;
        case 66 :
            if (y >= 56 * 16 && x >= 28 * 16 + 8 && x <= 30 * 16 + 8) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(30, 9 * 16 + 8, 7 * 16 + 8, S, false, true);
                AudioManager::getInstance()->playSound(TS_TELEPORT);
                return true;
            }
            // escaliers
            if ((x == 49 * 16 + 8 && y == 51 * 16)
                || (x == 109 * 16 + 8 && y == 51 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(66, x + 80 * 16, y + 24, S, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_DOWN);
                return true;
            }
            if ((x == 129 * 16 + 8 && y == 51 * 16)
                || (x == 189 * 16 + 8 && y == 51 * 16)) {
                MainController::getInstance()->getGameController()->getTeleportController()->setTeleport(66, x - 80 * 16, y - 24, N, false, false, true);
                AudioManager::getInstance()->playSound(TS_ESCALIERS_UP);
                return true;
            }
            break;
        default : return false;
    }
    return false;
}

void Map::testAnim(int x, int y, Direction dir) {

    // open door
    if (getSol(x, y + 8 - 1) == 458 && dir == N) {
        ouvrePorteMaison(x, y);
        AudioManager::getInstance()->playSound(TS_DOOR);
    }


    if (map >= 58) {
        Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

        // others
        if (map == 58 && bounds.getX() == 320 * 2 && bounds.getY() == 240 * 3 && !scene->getCoffre(1, 32) && mur[49 * 2 + 1][43 * 2 + 1] != MUR
            && scene->getLink()->getX() >= 320 * 2 + 16 * 8 && scene->getLink()->getX() <= 320 * 2 + 16 * 11 && scene->getLink()->getY() <= 240 * 3 + 16 * 2) {
            AudioManager::getInstance()->playSound(TS_DOOR);
            fermePorte(49, 43, 0);
            addEffect(new FumeeBlanche(45*16+8, 52*16));
            addEnnemi(new Ennemi001(45*16,51*16+6));
            addEffect(new FumeeBlanche(54*16+8, 52*16));
            addEnnemi(new Ennemi001(54*16,51*16+6));
            sol[45][52] = 1410;
            sol[54][52] = 1410;
        }

        // doors with keys
        if (scene->getCoffre(map - 57, 3)) {
            int i = 0;
            int j = 0;

            switch (map) {
                case 58 :
                    if (getSol(x, y + 7) == 1491 && dir == N) { i = x/16; j = ((y + 7)/16) - 3;}
                    if (getSol(x, y + 24) == 1495 && dir == S) { i = x/16; j = (y + 24)/16;}
                    if (getSol(x - 1, y + 16) == 1504 && dir == W) { i = ((x-1)/16) - 3; j = y/16;}
                    if (getSol(x + 16, y + 16) == 1510 && dir == E) { i = (x+16)/16; j = y/16;}
                    break;
                case 59 :
                    if (getSol(x, y + 7) == 1721 && dir == N) { i = x/16; j = ((y + 7)/16) - 3;}
                    if (getSol(x, y + 24) == 1723 && dir == S) { i = x/16; j = (y + 24)/16;}
                    if (getSol(x - 1, y + 16) == 1730 && dir == W) { i = ((x-1)/16) - 3; j = y/16;}
                    if (getSol(x + 16, y + 16) == 1735 && dir == E) { i = (x+16)/16; j = y/16;}
                    break;
                case 60 :
                    if (getSol(x, y + 7) == 1919 && dir == N) { i = x/16; j = ((y + 7)/16) - 3;}
                    if (getSol(x, y + 24) == 1921 && dir == S) { i = x/16; j = (y + 24)/16;}
                    if (getSol(x - 1, y + 16) == 1928 && dir == W) { i = ((x-1)/16) - 3; j = y/16;}
                    if (getSol(x + 16, y + 16) == 1933 && dir == E) { i = (x+16)/16; j = y/16;}
                    break;
                case 61 :
                    if (getSol(x, y + 7) == 2151 && dir == N) { i = x/16; j = ((y + 7)/16) - 3;}
                    if (getSol(x, y + 24) == 2153 && dir == S) { i = x/16; j = (y + 24)/16;}
                    if (getSol(x - 1, y + 16) == 2160 && dir == W) { i = ((x-1)/16) - 3; j = y/16;}
                    if (getSol(x + 16, y + 16) == 2165 && dir == E) { i = (x+16)/16; j = y/16;}
                    break;
                case 62 :
                    if (getSol(x, y + 7) == 2372 && dir == N) { i = x/16; j = ((y + 7)/16) - 3;}
                    if (getSol(x, y + 24) == 2374 && dir == S) { i = x/16; j = (y + 24)/16;}
                    if (getSol(x - 1, y + 16) == 2381 && dir == W) { i = ((x-1)/16) - 3; j = y/16;}
                    if (getSol(x + 16, y + 16) == 2386 && dir == E) { i = (x+16)/16; j = y/16;}
                    break;
                case 63 :
                    if (getSol(x, y + 7) == 2636 && dir == N) { i = x/16; j = ((y + 7)/16) - 3;}
                    if (getSol(x, y + 24) == 2640 && dir == S) { i = x/16; j = (y + 24)/16;}
                    if (getSol(x - 1, y + 16) == 2650 && dir == W) { i = ((x-1)/16) - 3; j = y/16;}
                    if (getSol(x + 16, y + 16) == 2654 && dir == E) { i = (x+16)/16; j = y/16;}
                    break;
                case 64 :
                    if (getSol(x, y + 7) == 2854 && dir == N) { i = x/16; j = ((y + 7)/16) - 3;}
                    if (getSol(x, y + 24) == 2856 && dir == S) { i = x/16; j = (y + 24)/16;}
                    if (getSol(x - 1, y + 16) == 2863 && dir == W) { i = ((x-1)/16) - 3; j = y/16;}
                    if (getSol(x + 16, y + 16) == 2868 && dir == E) { i = (x+16)/16; j = y/16;}
                    break;
                case 65 :
                    if (getSol(x, y + 7) == 3074 && dir == N) { i = x/16; j = ((y + 7)/16) - 3;}
                    if (getSol(x, y + 24) == 3076 && dir == S) { i = x/16; j = (y + 24)/16;}
                    if (getSol(x - 1, y + 16) == 3083 && dir == W) { i = ((x-1)/16) - 3; j = y/16;}
                    if (getSol(x + 16, y + 16) == 3088 && dir == E) { i = (x+16)/16; j = y/16;}
                    break;
                case 66 :
                    if (getSol(x, y + 7) == 3252 && dir == N) { i = x/16; j = ((y + 7)/16) - 3;}
                    if (getSol(x, y + 24) == 3254 && dir == S) { i = x/16; j = (y + 24)/16;}
                    break;
            }

            if (i != 0) {
                int k = 0;
                switch (map) {
                    case 58 :
                        if (bounds.getX() == 0 && bounds.getY() == 0) k = 11;
                        else if (bounds.getX() == 0 && bounds.getY() == 240) k = 12;
                        else if (bounds.getX() == 320 && bounds.getY() == 0) k = 13;
                        else if (bounds.getX() == 320*3 && scene->getLink()->getX() < 320*4) k = 14;
                        else if (bounds.getX() == 320*4) k = 15;
                        else if (bounds.getX() == 320*3 && scene->getLink()->getX() > 320*4) k = 16;
                        else k = 17;
                        break;
                    case 59 :
                        if (bounds.getX() == 0 && bounds.getY() == 240 * 8) k = 13;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240 * 5) k = 14;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240 * 1) k = 15;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 0) k = 16;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240 * 13) k = 17;
                        else if (bounds.getX() == 320*4 && bounds.getY() == 240 * 4) k = 18;
                        else if (bounds.getX() == 320*5 && bounds.getY() == 240 * 3) k = 19;
                        else if (bounds.getX() == 320*5 && bounds.getY() == 240 * 4) k = 20;
                        else k = 21;
                        break;
                    case 60 :
                        if (bounds.getX() == 320*2 && scene->getLink()->getX() < 320*2 + 160) k = 12;
                        else if (bounds.getX() == 320*2 && scene->getLink()->getX() > 320*2 + 160) k = 13;
                        else if (bounds.getX() == 320*5) k = 14;
                        else if (bounds.getX() == 320*6) k = 15;
                        else if (bounds.getX() == 320*7) k = 16;
                        else if (bounds.getX() == 320*8) k = 17;
                        else if (bounds.getX() == 320*9) k = 18;
                        else k = 19;
                        break;
                    case 61 :
                        if (bounds.getX()%(320*7) == 320*1) k = 15;
                        else if (bounds.getX()%(320*7) == 320*2 && bounds.getY() == 240 * 5) k = 16;
                        else if (bounds.getX()%(320*7) == 320*2 && bounds.getY() == 240 * 14) k = 17;
                        else if (bounds.getX()%(320*7) == 320*3 && bounds.getY() == 240 * 2 && scene->getLink()->getX()%(320*7) < 320*3 + 64) k = 18;
                        else if (bounds.getX()%(320*7) == 320*3 && bounds.getY() == 240 * 2 && scene->getLink()->getX()%(320*7) > 320*4 - 64) k = 20;
                        else if (bounds.getX()%(320*7) == 320*3 && bounds.getY() == 240 * 2) k = 19;
                        else if ((bounds.getX()%(320*7) == 320*4 || bounds.getX()%(320*7) == 320*5) && bounds.getY() == 240 * 3 && scene->getLink()->getX()%(320*7) < 320*5 + 64 && scene->getLink()->getY() < 240*4) k = 21;
                        else if ((bounds.getX()%(320*7) == 320*4 || bounds.getX()%(320*7) == 320*5) && bounds.getY() == 240 * 3 && scene->getLink()->getX()%(320*7) < 320*5 + 64 && scene->getLink()->getY() > 240*4) k = 22;
                        else if (bounds.getX()%(320*7) == 320*4 && bounds.getY() == 240 * 13) k = 23;
                        else if (bounds.getX()%(320*7) == 320*5 && bounds.getY() == 240 * 3) k = 24;
                        else k = 25;
                        break;
                    case 62 :
                        if (bounds.getX() == 320*1 && bounds.getY() == 240*6) k = 14;
                        else if (bounds.getX() == 320*1 && bounds.getY() == 240*10) k = 15;
                        else if (bounds.getX() == 0 && bounds.getY() == 240*12) k = 16;
                        else if (bounds.getX() == 320*1 && bounds.getY() == 240*3) k = 17;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240*12) k = 18;
                        else if (bounds.getX() == 320*3 && bounds.getY() == 0) k = 19;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240*5) k = 20;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240*6) k = 21;
                        else k = 22;
                        break;
                    case 63 :
                        if (bounds.getX() == 320*1 && bounds.getY() == 240*7 && scene->getLink()->getY() > 240*7 + 120) k = 9;
                        else if (bounds.getX() == 320*1 && bounds.getY() == 240*7) k = 10;
                        else if (bounds.getX() == 320*1 && bounds.getY() == 240*8) k = 29;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240*6) k = 11;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240*8) k = 12;
                        else k = 13;
                        break;
                    case 64 :
                        if (bounds.getX() == 0 && bounds.getY() == 240*3) k = 15;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240*2 && scene->getLink()->getX() < 320*2 + 96) k = 16;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240*2) k = 17;
                        else if (bounds.getX() == 320*2 && bounds.getY() == 240*7) k = 18;
                        else if (bounds.getX() == 320*3 && bounds.getY() == 240*9) k = 19;
                        else if (bounds.getX() == 320*4 && bounds.getY() == 240*4) k = 20;
                        else if (bounds.getX() == 320*7 && bounds.getY() == 240*9) k = 21;
                        else if (bounds.getX() == 320*10 && bounds.getY() == 240*2 && scene->getLink()->getX() < 320*10 + 96) k = 22;
                        else if (bounds.getX() == 320*10 && bounds.getY() == 240*2) k = 23;
                        else if (bounds.getX() == 320*11 && bounds.getY() == 240*2) k = 24;
                        else k = 25;
                        break;
                    case 65 :
                        if (bounds.getX() == 320*7 && bounds.getY() == 240*3) k = 11;
                        else if (bounds.getX() == 320*6 && bounds.getY() == 240*3) k = 12;
                        else if (bounds.getX() == 320*6 && bounds.getY() == 240*2) k = 13;
                        else if (bounds.getX() == 320*6 && bounds.getY() == 240) k = 14;
                        else if (bounds.getX() == 320*7 && bounds.getY() == 240) k = 15;
                        else if (bounds.getX() == 320*8 && bounds.getY() == 240) k = 16;
                        else k = 17;
                        break;
                    case 66 :
                        if (bounds.getX() == 320*3) k = 8;
                        else if (bounds.getX() == 320*4) k = 9;
                        else if (bounds.getY() == 0) k = 10;
                        else k = 11;
                        break;
                }
                if (k != 0) {
                    scene->setCoffre(map - 57, k, 1);
                }

                if (map == 61) {
                    ouvrePorte(i%140, j, (dir == N || dir == S) ? 0 : 1);
                    ouvrePorte((i%140) + 140, j, (dir == N || dir == S) ? 0 : 1);
                } else {
                    ouvrePorte(i, j, (dir == N || dir == S) ? 0 : 1);
                }
                AudioManager::getInstance()->playSound(TS_DOOR);
                scene->setCoffre(map - 57, 3, scene->getCoffre(map - 57, 3) - 1);
            }
        }
        // boss
        if (scene->getCoffre(map - 57, 2) == 1 && dir == N) {
            int value = false;
            switch (map) {
                case 58 : value = 1521; break;
                case 59 : value = 1741; break;
                case 60 : value = 1939; break;
                case 61 : value = 2171; break;
                case 62 : value = 2392; break;
                case 63 : value = 2666; break;
                case 64 : value = 2874; break;
                case 65 : value = 3094; break;
                case 66 : value = 3260; break;
            }
            if (getSol(x, y + 7) == value) {
                if (map == 61) {
                    ouvrePorte((x/16)%140, ((y + 7)/16) - 3, 0);
                    ouvrePorte((x/16)%140 + 140, ((y + 7)/16) - 3, 0);
                } else {
                    ouvrePorte(x/16, ((y + 7)/16) - 3, 0);
                }
                AudioManager::getInstance()->playSound(TS_DOOR);
                scene->setCoffre(map - 57, 2, 2);
            }
        }
    }
}

void Map::ouvrePorteMaison(int x, int y) {
    int i = x / 16;
    int j = (y + 8) / 16;

    setSol(x, y + 8 - 1, 464, SOL_BOIS);
    setSol(x + 16, y + 8 - 1, 465, SOL_BOIS);

    mur[i * 2][j * 2 - 2] = MUR;
    mur[i * 2][j * 2 - 1] = MUR;
    mur[i * 2 + 3][j * 2 - 2] = MUR;
    mur[i * 2 + 3][j * 2 - 1] = MUR;
}

void Map::testAnimRoom() {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    switch (map) {
        case 21 :
            if (bounds.getY() == 0) {
                if (!link->getInventory()->hasCoeur(0) && mur[9 * 2 + 1][13 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(9, 13, 0);
                } else if (!link->getInventory()->hasCoeur(0) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(18);
                } else if (!link->getInventory()->hasObject(PALMES) && link->getInventory()->hasCoeur(0) && mur[9 * 2 + 1][13 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_PLOUF);
                    addEffect(new Plouf(9*16 + 8, 7*16 + 8));
                    addObject(new Pnj(9*16, 6*16 + 8, 30, 319));
                } else if (link->getInventory()->hasObject(PALMES) && mur[9 * 2 + 1][13 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(9, 13, 0);
                }
            }
            break;
        case 23 :
            if (bounds.getY() == 240) {
                if (!link->getInventory()->hasCoeur(1) && mur[9 * 2 + 1][28 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(9, 28, 0);
                } else if (!link->getInventory()->hasCoeur(1) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(18);
                } else if (!link->getInventory()->hasObject(MARTEAU) && link->getInventory()->hasCoeur(1) && mur[9 * 2 + 1][13 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(9, 13, 0);
                } else if (link->getInventory()->hasObject(MARTEAU) && mur[9 * 2 + 1][28 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(9, 28, 0);
                }
            }
            break;
        case 27 :
            if (bounds.getY() == 240) {
                if (!link->getInventory()->hasCoeur(2) && mur[29 * 2 + 1][28 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(29, 28, 0);
                } else if (!link->getInventory()->hasObject(PERLE_LUNE) && link->getInventory()->hasCoeur(2) && mur[29 * 2 + 1][13 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(29, 13, 0);
                } else if (link->getInventory()->hasObject(PERLE_LUNE) && mur[29 * 2 + 1][28 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(29, 28, 0);
                }
            }
            break;
        case 58 :
            if (bounds.getX() == 0 && bounds.getY() == 240 && mur[9 * 2 + 1][13 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 13, 0);
            }
            if (bounds.getX() == 320 * 14 && bounds.getY() == 240) {
                if (!link->getInventory()->hasCoeur(5) && mur[289 * 2 + 1][28 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(289, 28, 0);
                } else if (!link->getInventory()->hasCoeur(5) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(20);
                } else if (!link->getInventory()->hasMedaillons(0) && link->getInventory()->hasCoeur(5) && mur[289 * 2 + 1][13 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(289, 13, 0);
                } else if (link->getInventory()->hasMedaillons(0) && mur[289 * 2 + 1][28 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(289, 28, 0);
                }
            }
            if (bounds.getX() == 320 * 13 && bounds.getY() == 240 * 4) {
                if (!scene->getCoffre(1, 31) && mur[260 * 2][67 * 2] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(258, 66, 1);
                } else if (!scene->getCoffre(1, 31) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(28);
                    scene->setCoffre(1, 31, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(258, 66, 1);
                    ouvrePorte(278, 66, 1);
                }
            }
            if (bounds.getX() == 320 * 2 && bounds.getY() == 240 * 3) {
                if (!scene->getCoffre(1, 32) && mur[49 * 2 + 1][58 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    fermePorte(49, 58, 0);
                } else if (!scene->getCoffre(1, 32) && nbEnnemis() == 0) {
                    scene->setCoffre(1, 32, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(49, 58, 0);
                    ouvrePorte(49, 43, 0);
                }
            }
            if (bounds.getX() == 320 * 7 && bounds.getY() == 0) {
                if (sol[144][7] == 1639 && nbEnnemis() == 0) {
                    sol[144][7] = 1412;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(144*16, 7*16, TI_BOUSSOLE, 0);
                }
            }
            if (bounds.getX() == 320 * 13 && bounds.getY() == 0) {
                if (sol[275][7] == 1639 && nbEnnemis() == 0) {
                    sol[275][7] = 1414;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(275*16, 7*16, TI_CLE, 10);
                }
            }
            break;
        case 59 :
            if (bounds.getX() == 320 * 3 && bounds.getY() == 240 * 11 && mur[69 * 2 + 1][163 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(69, 163, 0);
            }
            if (bounds.getX() == 320 * 6 && bounds.getY() == 0 && mur[129 * 2 + 1][13 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(129, 13, 0);
            }

            if (bounds.getX() == 320 * 3 && bounds.getY() == 240 * 2) {
                if (!link->getInventory()->hasCoeur(6) && mur[69 * 2 + 1][43 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(69, 43, 0);
                } else if (!link->getInventory()->hasCoeur(6) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(20);
                } else if (!link->getInventory()->hasMedaillons(1) && link->getInventory()->hasCoeur(6) && mur[69 * 2 + 1][28 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(69, 28, 0);
                } else if (link->getInventory()->hasMedaillons(1) && mur[69 * 2 + 1][43 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(69, 43, 0);
                }
            }
            if (bounds.getX() == 320 * 6 && bounds.getY() == 240 * 4) {
                if (!scene->getCoffre(2, 33) && mur[120 * 2][67 * 2] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(118, 66, 1);
                } else if (!scene->getCoffre(2, 33) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(29);
                    scene->setCoffre(2, 33, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(118, 66, 1);
                    ouvrePorte(129, 58, 0);
                }
            }

            if (bounds.getX() == 320 * 2 && bounds.getY() == 240 * 2) {
                if (sol[54][37] == 1639 && nbEnnemis() == 0) {
                    sol[54][37] = 1645;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(54*16, 37*16, TI_CLE, 8);
                }
            }
            if (bounds.getX() == 320 * 5 && bounds.getY() == 240 * 10) {
                if (sol[114][157] == 1639 && nbEnnemis() == 0) {
                    sol[114][157] = 1641;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(114*16, 157*16, TI_CLE, 11);
                }
            }
            if (bounds.getX() == 320 && bounds.getY() == 240 * 10) {
                if (sol[25][157] == 1639 && nbEnnemis() == 0) {
                    sol[25][157] = 1641;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(25*16, 157*16, TI_BOUSSOLE, 0);
                }
            }
            break;
        case 60 :
            if (bounds.getX() == 0 && bounds.getY() == 240 * 3 && mur[20 * 2][52 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(18, 51, 1);
            }
            if (bounds.getX() == 320 * 3 && bounds.getY() == 240 * 7 && mur[80 * 2][112 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(78, 111, 1);
            }
            if (bounds.getX() == 320 * 5 && (bounds.getY() == 240 * 3 || bounds.getY() == 240 * 4) && mur[109 * 2 + 1][58 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(109, 58, 0);
            }
            if (bounds.getX() == 320 * 11 && bounds.getY() == 240 * 3 && mur[229 * 2 + 1][58 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(229, 58, 0);
            }

            if (bounds.getX() == 320 * 2 && bounds.getY() == 240 * 4) {
                if (!link->getInventory()->hasCoeur(7) && mur[49 * 2 + 1][73 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(49, 73, 0);
                } else if (!link->getInventory()->hasCoeur(7) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(20);
                } else if (!link->getInventory()->hasMedaillons(2) && link->getInventory()->hasCoeur(7) && mur[49 * 2 + 1][58 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(49, 58, 0);
                } else if (link->getInventory()->hasMedaillons(2) && mur[49 * 2 + 1][73 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(49, 73, 0);
                }
            }
            if (bounds.getX() == 320 * 7 && bounds.getY() == 240 * 1) {
                if (!scene->getCoffre(3, 28) && mur[149 * 2 + 1][28 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(149, 28, 0);
                } else if (!scene->getCoffre(3, 28) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(30);
                    scene->setCoffre(3, 28, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(158, 21, 1);
                    ouvrePorte(149, 28, 0);
                }
            }

            if (bounds.getX() == 320 * 3 && bounds.getY() == 240 * 3) {
                if (sol[63][52] == 1639 && nbEnnemis() == 0) {
                    sol[63][52] = 1839;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(63*16, 52*16, TI_BOUSSOLE, 0);
                }
            }
            if (bounds.getX() == 320 * 7 && bounds.getY() == 240 * 7) {
                if (sol[156][112] == 1639 && nbEnnemis() == 0) {
                    sol[156][112] = 1837;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(156*16, 112*16, TI_CARTE_DONJON, 0);
                }
            }
            break;
        case 61 :
            if (bounds.getX() == 320 * 2 && (bounds.getY() == 240 * 10 || bounds.getY() == 240 * 11) && mur[49 * 2 + 1][163 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(49, 163, 0);
            }
            if (bounds.getX() == 320 * 4 && (bounds.getY() == 240 * 10 || bounds.getY() == 240 * 11) && mur[89 * 2 + 1][163 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 163, 0);
            }
            if (bounds.getX() % (320 * 7) == 320 * 6 && bounds.getY() == 0 && mur[120 * 2][7 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(118, 6, 1);
                ouvrePorte(258, 6, 1);
                ouvrePorte(138, 171, 1);
            }
            if (bounds.getX() % (320 * 7) == 320 * 2 && bounds.getY() == 240 * 8 && mur[49 * 2 + 1][133 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(49, 133, 0);
                ouvrePorte(189, 133, 0);
            }

            if (bounds.getX() % (320 * 7) == 320 * 3 && bounds.getY() == 240) {
                if (!link->getInventory()->hasCoeur(8) && mur[69 * 2 + 1][28 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(69, 28, 0);
                    fermePorte(209, 28, 0);
                } else if (!link->getInventory()->hasCoeur(8) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(20);
                } else if (!link->getInventory()->hasMedaillons(3) && link->getInventory()->hasCoeur(8) && mur[69 * 2 + 1][13 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(69, 13, 0);
                    ouvrePorte(209, 13, 0);
                } else if (link->getInventory()->hasMedaillons(3) && mur[69 * 2 + 1][28 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(69, 28, 0);
                    ouvrePorte(209, 28, 0);
                }
            }
            if (bounds.getX() % (320 * 7) == 0 && bounds.getY() == 240) {
                if (!scene->getCoffre(4, 37) && mur[149 * 2 + 1][28 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(9, 28, 0);
                    fermePorte(149, 28, 0);
                } else if (!scene->getCoffre(4, 37) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(31);
                    scene->setCoffre(4, 37, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(9, 13, 0);
                    ouvrePorte(149, 13, 0);
                }
            }

            if (bounds.getX() % (320 * 7) == 320 * 4 && bounds.getY() == 240 * 5) {
                if (sol[96][82] == 1639 && nbEnnemis() == 0) {
                    sol[96][82] = 2072;
                    sol[236][82] = 2072;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(96*16, 82*16, TI_CLE, 10);
                    addCoffre(236*16, 82*16, TI_CLE, 10);
                }
            }
            break;
        case 62 :
            if (bounds.getX() == 0 && bounds.getY() == 240 * 8 && mur[9 * 2 + 1][118 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 118, 0);
            }
            if (bounds.getX() == 0 && bounds.getY() == 240 * 14 && mur[18 * 2 + 1][217 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(18, 216, 1);
            }
            if (bounds.getX() == 320 * 5 && (bounds.getY() == 240 * 13 || bounds.getY() == 240 * 14) && mur[109 * 2 + 1][208 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(109, 208, 0);
            }

            if (bounds.getX() == 320 * 2 && bounds.getY() == 240 * 2) {
                if (!link->getInventory()->hasCoeur(9) && mur[49 * 2 + 1][43 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(49, 43, 0);
                } else if (!link->getInventory()->hasCoeur(9) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(20);
                } else if (!link->getInventory()->hasMedaillons(4) && link->getInventory()->hasCoeur(9) && mur[49 * 2 + 1][28 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(49, 28, 0);
                } else if (link->getInventory()->hasMedaillons(4) && mur[49 * 2 + 1][43 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(49, 43, 0);
                }
            }
            if (bounds.getX() == 0 && bounds.getY() == 240 * 6) {
                if (!scene->getCoffre(5, 47) && mur[18 * 2 + 1][97 * 2] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(18, 96, 1);
                } else if (!scene->getCoffre(5, 47) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(32);
                    scene->setCoffre(5, 47, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(9, 88, 0);
                    ouvrePorte(18, 96, 1);
                }
            }

            if (bounds.getX() == 320 * 5 && bounds.getY() == 240 * 6) {
                if (!scene->getCoffre(5, 48) && mur[98 * 2 + 1][97 * 2] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(98, 96, 1);
                } else if (!scene->getCoffre(5, 48) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(32);
                    scene->setCoffre(5, 48, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(98, 96, 1);
                }
            }
            if (bounds.getX() == 320 * 5 && bounds.getY() == 240) {
                if (!scene->getCoffre(5, 49) && nbEnnemis() != 0) {
                    AudioManager::getInstance()->playMusic(19);
                } else if (!scene->getCoffre(5, 49) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(32);
                    scene->setCoffre(5, 49, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(109, 13, 0);
                }
            }
            break;
        case 63 :
            if (bounds.getX() == 0 && bounds.getY() == 0 && mur[29 * 2 + 1][28 * 2 + 1] == MUR && !nbEnnemis()) {
                scene->getAnimationInGame()->startAnim(38);
            }
            if (bounds.getX() == 320 * 3 && bounds.getY() == 240 * 2 && mur[69 * 2 + 1][28 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(69, 28, 0);
            }
            if (bounds.getX() == 320 * 4 && bounds.getY() == 0 && mur[78 * 2 + 1][7 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(78, 6, 1);
            }
            if (bounds.getX() == 0 && bounds.getY() == 240 * 8 && mur[9 * 2 + 1][133 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 133, 0);
            }
            if (bounds.getX() == 320 * 5 && bounds.getY() == 240 * 8 && mur[109 * 2 + 1][118 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(109, 118, 0);
            }
            if (bounds.getX() == 320 * 3 && bounds.getY() == 240 * 10 && mur[58 * 2 + 1][157 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(58, 156, 1);
            }

            if (bounds.getX() == 320 * 2 && bounds.getY() == 240) {
                if (!link->getInventory()->hasCoeur(10) && mur[49 * 2 + 1][28 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(49, 28, 0);
                } else if (!link->getInventory()->hasCoeur(10) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(20);
                } else if (!link->getInventory()->hasMedaillons(5) && link->getInventory()->hasCoeur(10) && mur[49 * 2 + 1][13 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(49, 13, 0);
                } else if (link->getInventory()->hasMedaillons(5) && mur[49 * 2 + 1][28 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(49, 28, 0);
                }
            }
            if (bounds.getX() == 320 * 3 && bounds.getY() == 240 * 6) {
                if (!scene->getCoffre(6, 27) && mur[69 * 2 + 1][103 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(69, 103, 0);
                } else if (!scene->getCoffre(6, 27) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(33);
                    scene->setCoffre(6, 27, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(69, 88, 0);
                    ouvrePorte(69, 103, 0);
                }
            }

            if (bounds.getX() == 0 && bounds.getY() == 240 * 3) {
                if (sol[3][52] == 1639 && nbEnnemis() == 0) {
                    sol[3][52] = 2563;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(3*16, 52*16, TI_CLE, 4);
                }
            }
            if (bounds.getX() == 320 && bounds.getY() == 240 * 10) {
                if (sol[23][157] == 1639 && nbEnnemis() == 0) {
                    sol[23][157] = 2559;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(23*16, 157*16, TI_CLE, 5);
                }
            }
            break;
        case 64 :
            if (bounds.getX() == 320 && bounds.getY() == 240 * 7 && mur[18 * 2 + 1][112 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(18, 111, 1);
            }
            if (bounds.getX() == 320 && bounds.getY() == 240 * 9 && mur[38 * 2 + 1][142 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 141, 1);
            }
            if (bounds.getX() == 320 * 6 && bounds.getY() == 240 * 2 && mur[129 * 2 + 1][43 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(129, 43, 0);
            }
            if (bounds.getX() == 320 * 6 && bounds.getY() == 240 * 8 && mur[129 * 2 + 1][118 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(129, 118, 0);
            }
            if (bounds.getX() == 320 * 7 && bounds.getY() == 240 * 8 && mur[149 * 2 + 1][133 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(149, 133, 0);
            }
            if (bounds.getX() == 320 * 9 && bounds.getY() == 240 * 5 && mur[189 * 2 + 1][88 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(189, 88, 0);
            }
            if (bounds.getX() == 320 * 11 && bounds.getY() == 0 && mur[218 * 2 + 1][7 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(218, 6, 1);
            }
            if (bounds.getX() == 320 * 12 && bounds.getY() == 240 * 2 && mur[249 * 2 + 1][28 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(249, 28, 0);
            }

            if (bounds.getX() == 320 * 3 && bounds.getY() == 240) {
                if (!link->getInventory()->hasCoeur(11) && mur[69 * 2 + 1][28 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(69, 28, 0);
                } else if (!link->getInventory()->hasCoeur(11) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(20);
                } else if (!link->getInventory()->hasMedaillons(6) && link->getInventory()->hasCoeur(11) && mur[69 * 2 + 1][13 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(69, 13, 0);
                } else if (link->getInventory()->hasMedaillons(6) && mur[69 * 2 + 1][28 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(69, 28, 0);
                }
            }
            if (bounds.getX() == 320 * 3 && bounds.getY() == 240 * 6) {
                if (!scene->getCoffre(7, 49) && mur[69 * 2 + 1][103 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(69, 103, 0);
                } else if (!scene->getCoffre(7, 49) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(34);
                    scene->setCoffre(7, 49, 1);
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(69, 88, 0);
                    ouvrePorte(69, 103, 0);
                }
            }

            if (bounds.getX() == 320 * 2 && bounds.getY() == 0) {
                if (sol[56][7] == 1639 && nbEnnemis() == 0) {
                    sol[56][7] = 2778;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(56*16, 7*16, TI_CLE, 6);
                }
            }
            if (bounds.getX() == 320 * 2 && bounds.getY() == 240 * 5) {
                if (sol[56][82] == 1639 && nbEnnemis() == 0) {
                    sol[56][82] = 2777;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(56*16, 82*16, TI_CLE, 7);
                }
            }
            if (bounds.getX() == 320 * 5 && bounds.getY() == 0) {
                if (sol[103][7] == 1639 && nbEnnemis() == 0) {
                    sol[103][7] = 2778;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(103*16, 7*16, TI_CLE, 8);
                }
            }
            if (bounds.getX() == 320 * 7 && bounds.getY() == 0) {
                if (sol[143][7] == 1639 && nbEnnemis() == 0) {
                    sol[143][7] = 2776;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(143*16, 7*16, TI_CLE, 11);
                }
            }
            break;
        case 65 :
            if (bounds.getX() == 0 && bounds.getY() == 240 * 2 && mur[9 * 2 + 1][28 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 28, 0);
            }
            if (bounds.getX() == 320*3 && bounds.getY() == 240 && mur[69 * 2 + 1][13 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(69, 13, 0);
            }
            if (bounds.getX() == 320*4 && bounds.getY() == 240 && mur[89 * 2 + 1][13 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(89, 13, 0);
            }

            if (bounds.getX() == 320 * 2 && bounds.getY() == 240) {
                if (!link->getInventory()->hasCoeur(12) && mur[49 * 2 + 1][28 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(19);
                    fermePorte(49, 28, 0);
                } else if (!link->getInventory()->hasCoeur(12) && nbEnnemis() == 0) {
                    AudioManager::getInstance()->playMusic(20);
                } else if (link->getEpee() < 5 && link->getInventory()->hasCoeur(12) && mur[49 * 2 + 1][13 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(49, 13, 0);
                } else if (link->getEpee() == 5 && mur[49 * 2 + 1][28 * 2 + 1] == MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    ouvrePorte(49, 28, 0);
                }
            }

            if (bounds.getX() == 0 && bounds.getY() == 0) {
                if (sol[3][7] == 1639 && nbEnnemis() == 0) {
                    sol[3][7] = 2997;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(3*16, 7*16, TI_CLE, 4);
                }
            }
            if (bounds.getX() == 0 && bounds.getY() == 240 * 4) {
                if (sol[3][67] == 1639 && nbEnnemis() == 0) {
                    sol[3][67] = 2997;
                    AudioManager::getInstance()->playSound(TS_SURPRISE);
                    addCoffre(3*16, 67*16, TI_CLE, 6);
                }
            }
            break;
        case 66 :
            if (bounds.getX() == 0 && bounds.getY() == 0 && mur[9 * 2 + 1][13 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 13, 0);
            }
            if (bounds.getX() == 0 && bounds.getY() == 240 * 2 && mur[9 * 2 + 1][43 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(9, 43, 0);
            }
            if (bounds.getX() == 320 * 2 && bounds.getY() == 0 && mur[38 * 2 + 1][7 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(38, 6, 1);
            }
            if (bounds.getX() == 320 * 7 && bounds.getY() == 240 && mur[149 * 2 + 1][13 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(149, 13, 0);
            }
            if (bounds.getX() == 320 * 9 && bounds.getY() == 0 && mur[178 * 2 + 1][7 * 2] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(178, 6, 1);
            }
            if (bounds.getX() == 320 * 10 && bounds.getY() == 240 * 3 && mur[209 * 2 + 1][43 * 2 + 1] == MUR && !nbEnnemis()) {
                AudioManager::getInstance()->playSound(TS_DOOR);
                ouvrePorte(209, 43, 0);
            }

            if (bounds.getX() == 320 * 9 && bounds.getY() == 240) {
                if (mur[189 * 2 + 1][28 * 2 + 1] != MUR) {
                    AudioManager::getInstance()->playSound(TS_DOOR);
                    AudioManager::getInstance()->playMusic(38);
                    fermePorte(189, 28, 0);
                } else if (nbEnnemis() == 0) {
                    scene->getAnimationInGame()->startAnim(50);
                }
            }
            break;
    }
    if (link->isFatalAnim()) {
        MainController::getInstance()->getGameController()->getSceneController()->getScene()->getAnimationInGame()->startAnim(43);
    }
}

void Map::addCoffre(int x, int y, TypeItem type, int id) {
    Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
    addObject(new Coffre(x, y, 0, false, type, id));
    BoundingBox b(x,y,16,16);
    if (b.intersect(link->getBoundingBox())) {
        link->setY(y-24);
    }
}

void Map::permuteBlocs(int step) {
    // 1 = orange up - blue down
    switch (map) {
        case 3 :
            if (step == 0) {
                setSol(70 * 16, 46 * 16, 1, 3, 552, PIERRE);
            } else {
                setSol(70 * 16, 46 * 16, 1, 3, 554, MURRET);
            }
            break;
        case 20 :
            if (step == 0) {
                setSol(6 * 16, 2 * 16, 1, 2, 552, PIERRE);
                setSol(13 * 16, 9 * 16, 1, 2, 552, PIERRE);
                setSol(16 * 16, 6 * 16, 1, 3, 555, MURRET);
            } else {
                setSol(6 * 16, 2 * 16, 1, 2, 554, MURRET);
                setSol(13 * 16, 9 * 16, 1, 2, 554, MURRET);
                setSol(16 * 16, 6 * 16, 1, 3, 553, PIERRE);
            }
            break;
        case 60 :
            if (step == 0) {
                // plots oranges bas
                setSol(7*16, 19*16, 6, 5, 552, PIERRE); setSol(8*16, 20*16, 4, 3, 1842, PIERRE);
                setSol(29*16, 18*16, 2, 1, 552, PIERRE);
                setSol(28*16, 68*16, 4, 1, 552, PIERRE);
                setSol(28*16, 93*16, 4, 1, 552, PIERRE);
                setSol(28*16, 101*16, 4, 1, 552, PIERRE);
                setSol(49*16, 33*16, 2, 1, 552, PIERRE);
                setSol(69*16, 80*16, 2, 1, 552, PIERRE);
                setSol(72*16, 5*16, 4, 1, 552, PIERRE);
                setSol(72*16, 8*16, 4, 1, 552, PIERRE);
                setSol(72*16, 6*16, 1, 2, 552, PIERRE);
                setSol(75*16, 6*16, 1, 2, 552, PIERRE);
                setSol(94*16, 96*16, 2, 1, 552, PIERRE);
                setSol(94*16, 113*16, 2, 1, 552, PIERRE);
                setSol(107*16, 35*16, 6, 1, 552, PIERRE);
                setSol(108*16, 78*16, 4, 1, 552, PIERRE);
                setSol(126*16, 35*16, 4, 1, 552, PIERRE);
                setSol(129*16, 48*16, 2, 1, 552, PIERRE);
                setSol(151*16, 83*16, 2, 1, 552, PIERRE);
                setSol(156*16, 34*16, 1, 7, 552, PIERRE);
                setSol(188*16, 33*16, 4, 1, 552, PIERRE);
                setSol(224*16, 90*16, 2, 1, 552, PIERRE);

                // plots bleus hauts
                setSol(23*16, 94*16, 1, 7, 555, MURRET);
                setSol(25*16, 84*16, 1, 2, 555, MURRET);
                setSol(29*16, 26*16, 2, 1, 555, MURRET);
                setSol(36*16, 94*16, 1, 7, 555, MURRET);
                setSol(36*16, 109*16, 1, 7, 555, MURRET);
                setSol(88*16, 64*16, 4, 1, 555, MURRET);
                setSol(88*16, 99*16, 4, 1, 555, MURRET);
                setSol(88*16, 110*16, 4, 1, 555, MURRET);
                setSol(116*16, 51*16, 1, 3, 555, MURRET);
                setSol(119*16, 39*16, 1, 2, 555, MURRET);
                setSol(127*16, 93*16, 6, 1, 555, MURRET);
                setSol(136*16, 34*16, 1, 7, 555, MURRET);
                setSol(154*16, 79*16, 1, 2, 555, MURRET);
                setSol(175*16, 64*16, 1, 7, 555, MURRET);
                setSol(196*16, 34*16, 1, 7, 555, MURRET);
                setSol(228*16, 90*16, 4, 1, 555, MURRET);
                setSol(208*16, 56*16, 4, 1, 555, MURRET);
            } else {
                // plots oranges hauts
                setSol(7*16, 19*16, 6, 5, 554, MURRET); setSol(8*16, 20*16, 4, 3, 1842, PIERRE);
                setSol(29*16, 18*16, 2, 1, 554, MURRET);
                setSol(28*16, 68*16, 4, 1, 554, MURRET);
                setSol(28*16, 93*16, 4, 1, 554, MURRET);
                setSol(28*16, 101*16, 4, 1, 554, MURRET);
                setSol(49*16, 33*16, 2, 1, 554, MURRET);
                setSol(69*16, 80*16, 2, 1, 554, MURRET);
                setSol(72*16, 5*16, 4, 1, 554, MURRET);
                setSol(72*16, 8*16, 4, 1, 554, MURRET);
                setSol(72*16, 6*16, 1, 2, 554, MURRET);
                setSol(75*16, 6*16, 1, 2, 554, MURRET);
                setSol(94*16, 96*16, 2, 1, 554, MURRET);
                setSol(94*16, 113*16, 2, 1, 554, MURRET);
                setSol(107*16, 35*16, 6, 1, 554, MURRET);
                setSol(108*16, 78*16, 4, 1, 554, MURRET);
                setSol(126*16, 35*16, 4, 1, 554, MURRET);
                setSol(129*16, 48*16, 2, 1, 554, MURRET);
                setSol(151*16, 83*16, 2, 1, 554, MURRET);
                setSol(156*16, 34*16, 1, 7, 554, MURRET);
                setSol(188*16, 33*16, 4, 1, 554, MURRET);
                setSol(224*16, 90*16, 2, 1, 554, MURRET);

                // plots bleus bas
                setSol(23*16, 94*16, 1, 7, 553, PIERRE);
                setSol(25*16, 84*16, 1, 2, 553, PIERRE);
                setSol(29*16, 26*16, 2, 1, 553, PIERRE);
                setSol(36*16, 94*16, 1, 7, 553, PIERRE);
                setSol(36*16, 109*16, 1, 7, 553, PIERRE);
                setSol(88*16, 64*16, 4, 1, 553, PIERRE);
                setSol(88*16, 99*16, 4, 1, 553, PIERRE);
                setSol(88*16, 110*16, 4, 1, 553, PIERRE);
                setSol(116*16, 51*16, 1, 3, 553, PIERRE);
                setSol(119*16, 39*16, 1, 2, 553, PIERRE);
                setSol(127*16, 93*16, 6, 1, 553, PIERRE);
                setSol(136*16, 34*16, 1, 7, 553, PIERRE);
                setSol(154*16, 79*16, 1, 2, 553, PIERRE);
                setSol(175*16, 64*16, 1, 7, 553, PIERRE);
                setSol(196*16, 34*16, 1, 7, 553, PIERRE);
                setSol(228*16, 90*16, 4, 1, 553, PIERRE);
                setSol(208*16, 56*16, 4, 1, 553, PIERRE);
            }
            break;
    }
}

int Map::nbPouleInBox() {
    if (map != 3) return 0;
    BoundingBox b(71*16, 32*16, 4*16, 7*16);
    List list;
    objects->get(&b, &list);

    int total = 0;

    list.iterateOnFirst();
    while (list.hasNext()) {
        Poule* poule = dynamic_cast<Poule*>(list.getNext());
        if (poule != 0) {
            total++;
        }
    }

    return total;
}

int Map::nbEnnemis() {
    List list;
    ennemis->get(&bounds, &list);
    return list.size();
}

void Map::syncJarre(int x, int y) {
    if (map != 61) return;

    List list;
    BoundingBox b1(x % (320*7), y, 16, 16);
    BoundingBox b2(x % (320*7) + 320*7, y, 16, 16);
    objects->get(&b1, &list);
    objects->get(&b2, &list);

    list.iterateOnFirst();
    while (list.hasNext()) {
        Jarre* jarre = dynamic_cast<Jarre*>(list.getNext());
        if (jarre != 0) {
            jarre->removeItem();
        }
    }
}

void Map::closeChests(int nb, int objId, bool gemme) {
    if (map != 34) return;

    int chests[9] = {0,0,0,0,0,0,0,0,0};

    for (int i = 0; i < nb; i++) {
        int randomValue = (int)((float)rand() / RAND_MAX * (9 - i));
        if (i > 0) {
            for (int j = 0; j <= randomValue; j++) {
                if (chests[j] == 1) {
                    randomValue++;
                }
            }
        }
        chests[randomValue] = 1;
    }

    BoundingBox b(6*16, 6*16, 5*16, 5*16);
    List list;
    objects->get(&b, &list);

    int num = 0;
    list.iterateOnFirst();
    while (list.hasNext()) {
        Coffre* coffre = dynamic_cast<Coffre*>(list.getNext());
        if (coffre != 0) {
            if (chests[num]) {
                coffre->close(gemme ? TI_GEMME_FORCE : TI_COQUILLAGE, objId);
            } else {
                coffre->close(TI_NO_ITEM);
            }
            num++;
            if (num == 9) { // should never happen
                return;
            }
        }
    }
}

void Map::openChests(int x, int y) {
    if (map != 34 && map != 61) return;
    List list;

    if (map == 34) {
        BoundingBox b(6*16, 6*16, 5*16, 5*16);
        objects->get(&b, &list);
    } else if (map == 61) {
        BoundingBox b1(x % (320*7), y, 16, 16);
        BoundingBox b2(x % (320*7) + 320*7, y, 16, 16);
        objects->get(&b1, &list);
        objects->get(&b2, &list);
    }

    list.iterateOnFirst();
    while (list.hasNext()) {
        Coffre* coffre = dynamic_cast<Coffre*>(list.getNext());
        if (coffre != 0) {
            coffre->open();
        }
    }
}

bool Map::areChestsClosed() {
    if (map != 34) return false;
    BoundingBox b(6*16, 6*16, 16, 16);
    List list;
    objects->get(&b, &list);

    list.iterateOnFirst();
    while (list.hasNext()) {
        Coffre* coffre = dynamic_cast<Coffre*>(list.getNext());
        if (coffre != 0) {
            return !coffre->isOpened();
        }
    }

    return false;
}

void Map::startCibles() {
    if (map != 43) return;
    resetRoom();
    gameScore = 0;
}

void Map::endCibles() {
    if (map != 43) return;
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    if (scene->getCoffre(0, 11) < gameScore) {
        scene->setCoffre(0, 11, gameScore);
    }
    gameScore = -1;
}

bool Map::isGameStarted() {
    return (map == 43 && nbEnnemis());
}

int Map::getGameScore() {
    return map == 43 ? gameScore : 0;
}

void Map::updateScore(int total) {
    gameScore += total;
}

void Map::ouvrePorte(int x, int y, int type) {
    if (map == 11 && type == 1) {
        setSol(x * 16, y * 16, 919, TROU);
        return;
    }
    switch (map) {
        case 4 : case 11 :
            sol[x][y]=322;
            sol[x+1][y]=323;
            sol[x][y+1]=324;
            sol[x+1][y+1]=325;
            mur[x * 2 + 1][y * 2 + 1] = HERBE; mur[x * 2 + 2][y * 2 + 1] = HERBE;
            mur[x * 2 + 1][y * 2 + 2] = HERBE; mur[x * 2 + 2][y * 2 + 2] = HERBE;
            mur[x * 2 + 1][y * 2 + 3] = HERBE; mur[x * 2 + 2][y * 2 + 3] = HERBE;
            break;
        case 5 :
            sol[x][y]=322;
            sol[x+1][y]=323;
            sol[x][y+1]=704;
            sol[x+1][y+1]=705;
            mur[x * 2 + 1][y * 2 + 1] = EAU; mur[x * 2 + 2][y * 2 + 1] = EAU;
            mur[x * 2 + 1][y * 2 + 2] = EAU; mur[x * 2 + 2][y * 2 + 2] = EAU;
            mur[x * 2 + 1][y * 2 + 3] = EAU; mur[x * 2 + 2][y * 2 + 3] = EAU;
            break;
        case 9 :
            sol[x][y-1]=824;
            sol[x+1][y-1]=826;
            sol[x][y]=322;
            sol[x+1][y]=323;
            sol[x][y+1]=704;
            sol[x+1][y+1]=705;
            sol[x][y+2]=35;
            sol[x+1][y+2]=35;
            mur[x * 2 + 1][y * 2 + 1] = EAU; mur[x * 2 + 2][y * 2 + 1] = EAU;
            mur[x * 2 + 1][y * 2 + 2] = EAU; mur[x * 2 + 2][y * 2 + 2] = EAU;
            mur[x * 2 + 1][y * 2 + 3] = EAU; mur[x * 2 + 2][y * 2 + 3] = EAU;
            break;
        case 10 :
            sol[x][y]=322;
            sol[x+1][y]=323;
            sol[x][y+1]=326;
            sol[x+1][y+1]=327;
            mur[x * 2 + 1][y * 2 + 1] = HERBE; mur[x * 2 + 2][y * 2 + 1] = HERBE;
            mur[x * 2 + 1][y * 2 + 2] = HERBE; mur[x * 2 + 2][y * 2 + 2] = HERBE;
            mur[x * 2 + 1][y * 2 + 3] = HERBE; mur[x * 2 + 2][y * 2 + 3] = HERBE;
            break;
        case 14 :
            sol[x][y]=1078;
            sol[x+1][y]=1079;
            sol[x+2][y]=1080;
            sol[x][y+1]=1081;
            sol[x+1][y+1]=1082;
            sol[x+2][y+1]=1083;
            sol[x][y+2]=1084;
            sol[x+1][y+2]=1085;
            sol[x+2][y+2]=1086;
            mur[x * 2 + 2][y * 2] = PIERRE; mur[x * 2 + 3][y * 2] = PIERRE;
            mur[x * 2 + 2][y * 2 + 1] = PIERRE; mur[x * 2 + 3][y * 2 + 1] = PIERRE;
            mur[x * 2 + 2][y * 2 + 2] = PIERRE; mur[x * 2 + 3][y * 2 + 2] = PIERRE;
            mur[x * 2 + 2][y * 2 + 3] = PIERRE; mur[x * 2 + 3][y * 2 + 3] = PIERRE;
            mur[x * 2 + 2][y * 2 + 4] = PIERRE; mur[x * 2 + 3][y * 2 + 4] = PIERRE;
            break;
        case 15 : case 21 : case 23 : case 25 : case 27 : case 29 :
            sol[x][y]=1130; sol[x + 1][y]=1131;
            sol[x][y + 1]=1132; sol[x + 1][y + 1]=1133;
            air[x][y + 1]=1134; air[x + 1][y + 1]=1135;
            sol[x][y + 2]=1124; sol[x + 1][y + 2]=1125;
            air[x][y + 2]=1128; air[x + 1][y + 2]=1129;
            sol[x][y + 3]=1126; sol[x + 1][y + 3]=1127;
            mur[x * 2 + 1][y * 2 + 1] = TERRE; mur[x * 2 + 2][y * 2 + 1] = TERRE;
            mur[x * 2 + 1][y * 2 + 2] = TERRE; mur[x * 2 + 2][y * 2 + 2] = TERRE;
            mur[x * 2 + 1][y * 2 + 3] = TERRE; mur[x * 2 + 2][y * 2 + 3] = TERRE;
            mur[x * 2 + 1][y * 2 + 4] = TERRE; mur[x * 2 + 2][y * 2 + 4] = TERRE;
            mur[x * 2 + 1][y * 2 + 5] = TERRE; mur[x * 2 + 2][y * 2 + 5] = TERRE;
            mur[x * 2 + 1][y * 2 + 6] = TERRE; mur[x * 2 + 2][y * 2 + 6] = TERRE;
            break;
        case 28 :
            sol[x][y]=1143;
            sol[x][y+1]=1145;
            sol[x][y+2]=1147;
            sol[x+1][y]=1144;
            sol[x+1][y+1]=1146; air[x+1][y+1]=1149;
            sol[x+1][y+2]=1148;

            sol[x+2][y]=1136;
            sol[x+2][y+1]=1138; air[x+2][y+1]=1142;
            sol[x+2][y+2]=1140;
            sol[x+3][y]=1137;
            sol[x+3][y+1]=1139;
            sol[x+3][y+2]=1141;

            mur[x * 2 + 1][y * 2 + 2] = TERRE; mur[x * 2 + 2][y * 2 + 2] = TERRE;
            mur[x * 2 + 1][y * 2 + 3] = TERRE; mur[x * 2 + 2][y * 2 + 3] = TERRE;
            mur[x * 2 + 3][y * 2 + 2] = TERRE; mur[x * 2 + 4][y * 2 + 2] = TERRE;
            mur[x * 2 + 3][y * 2 + 3] = TERRE; mur[x * 2 + 4][y * 2 + 3] = TERRE;
            mur[x * 2 + 5][y * 2 + 2] = TERRE; mur[x * 2 + 6][y * 2 + 2] = TERRE;
            mur[x * 2 + 5][y * 2 + 3] = TERRE; mur[x * 2 + 6][y * 2 + 3] = TERRE;
            break;
        case 58 :
            if (type == 0) {
                sol[x][y]=1471; sol[x + 1][y]=1472;
                sol[x][y + 1]=1473; sol[x + 1][y + 1]=1474;
                sol[x][y + 2]=1466; sol[x + 1][y + 2]=1467;
                sol[x][y + 3]=1468; sol[x + 1][y + 3]=1469;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 1) {
                sol[x][y]=1482;
                sol[x][y+1]=1484;
                sol[x][y+2]=1486;
                sol[x+1][y]=1517;
                sol[x+1][y+1]=1485;
                sol[x+1][y+2]=1518;

                sol[x+2][y]=1515;
                sol[x+2][y+1]=1477;
                sol[x+2][y+2]=1516;
                sol[x+3][y]=1476;
                sol[x+3][y+1]=1478;
                sol[x+3][y+2]=1480;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            } else if (type == 2) {
                // dalles
                sol[x][y]=1602;
                sol[x+1][y]=1603;
                sol[x][y+1]=1604;
                sol[x+1][y+1]=1605;
                mur[x * 2][y * 2] = PIERRE; mur[x * 2 + 1][y * 2] = PIERRE;
                mur[x * 2][y * 2+1] = PIERRE; mur[x * 2 + 1][y * 2+1] = PIERRE;
                mur[x * 2+2][y * 2] = PIERRE; mur[x * 2 + 3][y * 2] = PIERRE;
                mur[x * 2+2][y * 2+1] = PIERRE; mur[x * 2 + 3][y * 2+1] = PIERRE;
                mur[x * 2][y * 2+2] = PIERRE; mur[x * 2 + 1][y * 2+2] = PIERRE;
                mur[x * 2][y * 2+3] = PIERRE; mur[x * 2 + 1][y * 2+3] = PIERRE;
                mur[x * 2+2][y * 2+2] = PIERRE; mur[x * 2 + 3][y * 2+2] = PIERRE;
                mur[x * 2+2][y * 2+3] = PIERRE; mur[x * 2 + 3][y * 2+3] = PIERRE;
            }
            break;
        case 59 :
            if (type == 0) {
                sol[x][y]=1699; sol[x + 1][y]=1700;
                sol[x][y + 1]=1701; sol[x + 1][y + 1]=1702;
                sol[x][y + 2]=1693; sol[x + 1][y + 2]=1694;
                sol[x][y + 3]=1695; sol[x + 1][y + 3]=1696;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 1) {
                sol[x][y]=1712;
                sol[x][y+1]=1714;
                sol[x][y+2]=1716;
                sol[x+1][y]=1713;
                sol[x+1][y+1]=1715;
                sol[x+1][y+2]=1717;

                sol[x+2][y]=1705;
                sol[x+2][y+1]=1707;
                sol[x+2][y+2]=1709;
                sol[x+3][y]=1706;
                sol[x+3][y+1]=1708;
                sol[x+3][y+2]=1710;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            }
            break;
        case 60 :
            if (type == 0) {
                sol[x][y]=1897; sol[x + 1][y]=1898;
                sol[x][y + 1]=1899; sol[x + 1][y + 1]=1900;
                sol[x][y + 2]=1891; sol[x + 1][y + 2]=1892;
                sol[x][y + 3]=1893; sol[x + 1][y + 3]=1894;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 1) {
                sol[x][y]=1910;
                sol[x][y+1]=1912;
                sol[x][y+2]=1914;
                sol[x+1][y]=1911;
                sol[x+1][y+1]=1913;
                sol[x+1][y+2]=1915;

                sol[x+2][y]=1903;
                sol[x+2][y+1]=1905;
                sol[x+2][y+2]=1907;
                sol[x+3][y]=1904;
                sol[x+3][y+1]=1906;
                sol[x+3][y+2]=1908;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            } else if (type == 2) {
                sol[x][y]=2019; sol[x + 1][y]=2020;
                sol[x][y + 1]=2021; sol[x + 1][y + 1]=2022; air[x][y + 1]=2023; air[x + 1][y + 1]=2024;
                sol[x][y + 2]=2013; sol[x + 1][y + 2]=2014; air[x][y + 2]=2017; air[x + 1][y + 2]=2018;
                sol[x][y + 3]=2015; sol[x + 1][y + 3]=2016;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 3) {
                sol[x][y]=2032;
                sol[x][y+1]=2034;
                sol[x][y+2]=2036;
                sol[x+1][y]=2033;
                sol[x+1][y+1]=2035; air[x+1][y+1]=2038;
                sol[x+1][y+2]=2037;

                sol[x+2][y]=2025;
                sol[x+2][y+1]=2027; air[x+2][y+1]=2031;
                sol[x+2][y+2]=2029;
                sol[x+3][y]=2026;
                sol[x+3][y+1]=2028;
                sol[x+3][y+2]=2030;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            }
            break;
        case 61 :
            if (type == 0) {
                sol[x][y]=2129; sol[x + 1][y]=2130;
                sol[x][y + 1]=2131; sol[x + 1][y + 1]=2132;
                sol[x][y + 2]=2123; sol[x + 1][y + 2]=2124;
                sol[x][y + 3]=2125; sol[x + 1][y + 3]=2126;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 1) {
                sol[x][y]=2142;
                sol[x][y+1]=2144;
                sol[x][y+2]=2146;
                sol[x+1][y]=2143;
                sol[x+1][y+1]=2145;
                sol[x+1][y+2]=2147;

                sol[x+2][y]=2135;
                sol[x+2][y+1]=2137;
                sol[x+2][y+2]=2139;
                sol[x+3][y]=2136;
                sol[x+3][y+1]=2138;
                sol[x+3][y+2]=2140;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            }
            break;
        case 62 :
            if (type == 0) {
                sol[x][y]=2350; sol[x + 1][y]=2351;
                sol[x][y + 1]=2352; sol[x + 1][y + 1]=2353;
                sol[x][y + 2]=2344; sol[x + 1][y + 2]=2345;
                sol[x][y + 3]=2346; sol[x + 1][y + 3]=2347;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 1) {
                sol[x][y]=2363;
                sol[x][y+1]=2365;
                sol[x][y+2]=2367;
                sol[x+1][y]=2364;
                sol[x+1][y+1]=2366;
                sol[x+1][y+2]=2368;

                sol[x+2][y]=2356;
                sol[x+2][y+1]=2358;
                sol[x+2][y+2]=2360;
                sol[x+3][y]=2357;
                sol[x+3][y+1]=2359;
                sol[x+3][y+2]=2361;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            } else if (type == 2) {
                setSol(x*16, y*16, 1, 1, 2450, TROU);
                setSol((x+1)*16, y*16, 1, 1, 2452, TROU);
                setSol(x*16, (y+1)*16, 1, 1, 2455, TROU);
                setSol((x+1)*16, (y+1)*16, 1, 1, 2457, TROU);
            } else if (type == 3) {
                setSol(x*16, y*16, 1, 1, 2450, TROU); setSol((x+1)*16, y*16, 6, 1, 2451, TROU); setSol((x+7)*16, y*16, 1, 1, 2452, TROU);
                setSol(x*16, (y+1)*16, 1, 5, 2453, TROU); setSol((x+1)*16, (y+1)*16, 6, 5, 1233, TROU); setSol((x+7)*16, (y+1)*16, 1, 5, 2454, TROU);
                setSol(x*16, (y+6)*16, 1, 1, 2455, TROU); setSol((x+1)*16, (y+6)*16, 6, 1, 2456, TROU); setSol((x+7)*16, (y+6)*16, 1, 1, 2457, TROU);
            } else if (type == 4) {
                setSol(x*16, y*16, 1, 1, 2463, TROU); setSol((x+1)*16, y*16, 4, 1, 2464, TROU); setSol((x+5)*16, y*16, 1, 1, 2465, TROU);
                setSol(x*16, (y+1)*16, 1, 3, 2466, TROU); setSol((x+1)*16, (y+1)*16, 4, 3, 2475, TROU); setSol((x+5)*16, (y+1)*16, 1, 3, 2467, TROU);
                setSol(x*16, (y+4)*16, 1, 1, 2468, TROU); setSol((x+1)*16, (y+4)*16, 4, 1, 2469, TROU); setSol((x+5)*16, (y+4)*16, 1, 1, 2470, TROU);
            }
            break;
        case 63 :
            if (type == 0) {
                sol[x][y]=2616; sol[x + 1][y]=2617;
                sol[x][y + 1]=2618; sol[x + 1][y + 1]=2619;
                sol[x][y + 2]=2611; sol[x + 1][y + 2]=2612;
                sol[x][y + 3]=2613; sol[x + 1][y + 3]=2614;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 1) {
                sol[x][y]=2627;
                sol[x][y+1]=2629;
                sol[x][y+2]=2631;
                sol[x+1][y]=2662;
                sol[x+1][y+1]=2630;
                sol[x+1][y+2]=2663;

                sol[x+2][y]=2660;
                sol[x+2][y+1]=2622;
                sol[x+2][y+2]=2661;
                sol[x+3][y]=2621;
                sol[x+3][y+1]=2623;
                sol[x+3][y+2]=2625;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            } else if (type == 2) {
                setSol(x*16, y*16, 1, 1, 2724, TROU);
                setSol((x+1)*16, y*16, 1, 1, 2726, TROU);
                setSol(x*16, (y+1)*16, 1, 1, 2729, TROU);
                setSol((x+1)*16, (y+1)*16, 1, 1, 2731, TROU);
            } else if (type == 3) {
                sol[x][y]=2751; sol[x + 1][y]=2752;
                sol[x][y + 1]=2753; sol[x + 1][y + 1]=2754; air[x][y + 1]=2755; air[x + 1][y + 1]=2756;
                sol[x][y + 2]=2745; sol[x + 1][y + 2]=2746; air[x][y + 2]=2749; air[x + 1][y + 2]=2750;
                sol[x][y + 3]=2747; sol[x + 1][y + 3]=2748;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            }
            break;
        case 64 :
            if (type == 0) {
                sol[x][y]=2832; sol[x + 1][y]=2833;
                sol[x][y + 1]=2834; sol[x + 1][y + 1]=2835;
                sol[x][y + 2]=2826; sol[x + 1][y + 2]=2827;
                sol[x][y + 3]=2828; sol[x + 1][y + 3]=2829;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 1) {
                sol[x][y]=2845;
                sol[x][y+1]=2847;
                sol[x][y+2]=2849;
                sol[x+1][y]=2846;
                sol[x+1][y+1]=2848;
                sol[x+1][y+2]=2850;

                sol[x+2][y]=2838;
                sol[x+2][y+1]=2840;
                sol[x+2][y+2]=2842;
                sol[x+3][y]=2839;
                sol[x+3][y+1]=2841;
                sol[x+3][y+2]=2843;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            } else if (type == 2) {
                sol[x][y]=2954; sol[x + 1][y]=2955;
                sol[x][y + 1]=2956; sol[x + 1][y + 1]=2957; air[x][y + 1]=2958; air[x + 1][y + 1]=2959;
                sol[x][y + 2]=2948; sol[x + 1][y + 2]=2949; air[x][y + 2]=2952; air[x + 1][y + 2]=2953;
                sol[x][y + 3]=2950; sol[x + 1][y + 3]=2951;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 3) {
                sol[x][y]=2967;
                sol[x][y+1]=2969;
                sol[x][y+2]=2971;
                sol[x+1][y]=2968;
                sol[x+1][y+1]=2970; air[x+1][y+1]=2973;
                sol[x+1][y+2]=2972;

                sol[x+2][y]=2960;
                sol[x+2][y+1]=2962; air[x+2][y+1]=2966;
                sol[x+2][y+2]=2964;
                sol[x+3][y]=2961;
                sol[x+3][y+1]=2963;
                sol[x+3][y+2]=2965;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            } else if (type == 4) {
                setSol(x*16, y*16, 1, 1, 2932, TROU);
                setSol((x+1)*16, y*16, 1, 1, 2934, TROU);
                setSol(x*16, (y+1)*16, 1, 1, 2937, TROU);
                setSol((x+1)*16, (y+1)*16, 1, 1, 2939, TROU);
            }
            break;
        case 65 :
            if (type == 0) {
                sol[x][y]=3052; sol[x + 1][y]=3053;
                sol[x][y + 1]=3054; sol[x + 1][y + 1]=3055;
                sol[x][y + 2]=3046; sol[x + 1][y + 2]=3047;
                sol[x][y + 3]=3048; sol[x + 1][y + 3]=3049;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 1) {
                sol[x][y]=3065;
                sol[x][y+1]=3067;
                sol[x][y+2]=3069;
                sol[x+1][y]=3066;
                sol[x+1][y+1]=3068;
                sol[x+1][y+2]=3070;

                sol[x+2][y]=3058;
                sol[x+2][y+1]=3060;
                sol[x+2][y+2]=3062;
                sol[x+3][y]=3059;
                sol[x+3][y+1]=3061;
                sol[x+3][y+2]=3063;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            } else if (type == 2) {
                sol[x][y]=3162; sol[x + 1][y]=3163;
                sol[x][y + 1]=3164; sol[x + 1][y + 1]=3165; air[x][y + 1]=3166; air[x + 1][y + 1]=3167;
                sol[x][y + 2]=3156; sol[x + 1][y + 2]=3157; air[x][y + 2]=3160; air[x + 1][y + 2]=3161;
                sol[x][y + 3]=3158; sol[x + 1][y + 3]=3159;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 3) {
                sol[x][y]=3175;
                sol[x][y+1]=3177;
                sol[x][y+2]=3179;
                sol[x+1][y]=3176;
                sol[x+1][y+1]=3178; air[x+1][y+1]=3181;
                sol[x+1][y+2]=3180;

                sol[x+2][y]=3168;
                sol[x+2][y+1]=3170; air[x+2][y+1]=3174;
                sol[x+2][y+2]=3172;
                sol[x+3][y]=3169;
                sol[x+3][y+1]=3171;
                sol[x+3][y+2]=3173;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            }
            break;
        case 66 :
            if (type == 0) {
                sol[x][y]=3230; sol[x + 1][y]=3231;
                sol[x][y + 1]=3232; sol[x + 1][y + 1]=3233;
                sol[x][y + 2]=3224; sol[x + 1][y + 2]=3225;
                sol[x][y + 3]=3226; sol[x + 1][y + 3]=3227;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            } else if (type == 1) {
                sol[x][y]=3243;
                sol[x][y+1]=3245;
                sol[x][y+2]=3247;
                sol[x+1][y]=3244;
                sol[x+1][y+1]=3246;
                sol[x+1][y+2]=3248;

                sol[x+2][y]=3236;
                sol[x+2][y+1]=3238;
                sol[x+2][y+2]=3240;
                sol[x+3][y]=3237;
                sol[x+3][y+1]=3239;
                sol[x+3][y+2]=3241;

                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 3][y * 2 + 2] = PIERRE; mur[x * 2 + 4][y * 2 + 2] = PIERRE;
                mur[x * 2 + 3][y * 2 + 3] = PIERRE; mur[x * 2 + 4][y * 2 + 3] = PIERRE;
                mur[x * 2 + 5][y * 2 + 2] = PIERRE; mur[x * 2 + 6][y * 2 + 2] = PIERRE;
                mur[x * 2 + 5][y * 2 + 3] = PIERRE; mur[x * 2 + 6][y * 2 + 3] = PIERRE;
            } else if (type == 2) {
                sol[x][y]=3326; sol[x + 1][y]=3327;
                sol[x][y + 1]=3328; sol[x + 1][y + 1]=3329; air[x][y + 1]=3330; air[x + 1][y + 1]=3331;
                sol[x][y + 2]=3320; sol[x + 1][y + 2]=3321; air[x][y + 2]=3324; air[x + 1][y + 2]=3325;
                sol[x][y + 3]=3322; sol[x + 1][y + 3]=3323;
                mur[x * 2 + 1][y * 2 + 1] = PIERRE; mur[x * 2 + 2][y * 2 + 1] = PIERRE;
                mur[x * 2 + 1][y * 2 + 2] = PIERRE; mur[x * 2 + 2][y * 2 + 2] = PIERRE;
                mur[x * 2 + 1][y * 2 + 3] = PIERRE; mur[x * 2 + 2][y * 2 + 3] = PIERRE;
                mur[x * 2 + 1][y * 2 + 4] = PIERRE; mur[x * 2 + 2][y * 2 + 4] = PIERRE;
                mur[x * 2 + 1][y * 2 + 5] = PIERRE; mur[x * 2 + 2][y * 2 + 5] = PIERRE;
                mur[x * 2 + 1][y * 2 + 6] = PIERRE; mur[x * 2 + 2][y * 2 + 6] = PIERRE;
            }
            break;
    }
}

void Map::fermePorte(int x, int y, int type) {
    switch (map) {
        case 21 : case 23 : case 27 : case 29 :
            sol[x][y]=1205; sol[x + 1][y]=1206;
            sol[x][y + 1]=1207; sol[x + 1][y + 1]=1208;
            sol[x][y + 2]=1201; sol[x + 1][y + 2]=1202;
            sol[x][y + 3]=1203; sol[x + 1][y + 3]=1204;
            mur[x * 2 + 1][y * 2 + 1] = MUR; mur[x * 2 + 2][y * 2 + 1] = MUR;
            mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
            mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
            mur[x * 2 + 1][y * 2 + 4] = MUR; mur[x * 2 + 2][y * 2 + 4] = MUR;
            mur[x * 2 + 1][y * 2 + 5] = MUR; mur[x * 2 + 2][y * 2 + 5] = MUR;
            mur[x * 2 + 1][y * 2 + 6] = MUR; mur[x * 2 + 2][y * 2 + 6] = MUR;
            break;
        case 58 :
            if (type == 0) {
                sol[x][y]=1527; sol[x + 1][y]=1528;
                sol[x][y + 1]=1529; sol[x + 1][y + 1]=1530;
                sol[x][y + 2]=1523; sol[x + 1][y + 2]=1524;
                sol[x][y + 3]=1525; sol[x + 1][y + 3]=1526;
                mur[x * 2 + 1][y * 2 + 1] = MUR; mur[x * 2 + 2][y * 2 + 1] = MUR;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 1][y * 2 + 4] = MUR; mur[x * 2 + 2][y * 2 + 4] = MUR;
                mur[x * 2 + 1][y * 2 + 5] = MUR; mur[x * 2 + 2][y * 2 + 5] = MUR;
                mur[x * 2 + 1][y * 2 + 6] = MUR; mur[x * 2 + 2][y * 2 + 6] = MUR;
            } else if (type == 1) {
                sol[x][y]=1537; sol[x+1][y]=1538;
                sol[x][y+1]=1539; sol[x+1][y+1]=1540;
                sol[x][y+2]=1541; sol[x+1][y+2]=1542;
                sol[x+2][y]=1531; sol[x+3][y]=1532;
                sol[x+2][y+1]=1533; sol[x+3][y+1]=1534;
                sol[x+2][y+2]=1535; sol[x+3][y+2]=1536;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 3][y * 2 + 2] = MUR; mur[x * 2 + 4][y * 2 + 2] = MUR;
                mur[x * 2 + 3][y * 2 + 3] = MUR; mur[x * 2 + 4][y * 2 + 3] = MUR;
                mur[x * 2 + 5][y * 2 + 2] = MUR; mur[x * 2 + 6][y * 2 + 2] = MUR;
                mur[x * 2 + 5][y * 2 + 3] = MUR; mur[x * 2 + 6][y * 2 + 3] = MUR;
            }
            break;
        case 59 :
            if (type == 0) {
                sol[x][y]=1747; sol[x + 1][y]=1748;
                sol[x][y + 1]=1749; sol[x + 1][y + 1]=1750;
                sol[x][y + 2]=1743; sol[x + 1][y + 2]=1744;
                sol[x][y + 3]=1745; sol[x + 1][y + 3]=1746;
                mur[x * 2 + 1][y * 2 + 1] = MUR; mur[x * 2 + 2][y * 2 + 1] = MUR;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 1][y * 2 + 4] = MUR; mur[x * 2 + 2][y * 2 + 4] = MUR;
                mur[x * 2 + 1][y * 2 + 5] = MUR; mur[x * 2 + 2][y * 2 + 5] = MUR;
                mur[x * 2 + 1][y * 2 + 6] = MUR; mur[x * 2 + 2][y * 2 + 6] = MUR;
            } else if (type == 1) {
                sol[x][y]=1757; sol[x+1][y]=1758;
                sol[x][y+1]=1759; sol[x+1][y+1]=1760;
                sol[x][y+2]=1761; sol[x+1][y+2]=1762;
                sol[x+2][y]=1751; sol[x+3][y]=1752;
                sol[x+2][y+1]=1753; sol[x+3][y+1]=1754;
                sol[x+2][y+2]=1755; sol[x+3][y+2]=1756;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 3][y * 2 + 2] = MUR; mur[x * 2 + 4][y * 2 + 2] = MUR;
                mur[x * 2 + 3][y * 2 + 3] = MUR; mur[x * 2 + 4][y * 2 + 3] = MUR;
                mur[x * 2 + 5][y * 2 + 2] = MUR; mur[x * 2 + 6][y * 2 + 2] = MUR;
                mur[x * 2 + 5][y * 2 + 3] = MUR; mur[x * 2 + 6][y * 2 + 3] = MUR;
            }
            break;
        case 60 :
            if (type == 0) {
                sol[x][y]=1945; sol[x + 1][y]=1946;
                sol[x][y + 1]=1947; sol[x + 1][y + 1]=1948;
                sol[x][y + 2]=1941; sol[x + 1][y + 2]=1942;
                sol[x][y + 3]=1943; sol[x + 1][y + 3]=1944;
                mur[x * 2 + 1][y * 2 + 1] = MUR; mur[x * 2 + 2][y * 2 + 1] = MUR;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 1][y * 2 + 4] = MUR; mur[x * 2 + 2][y * 2 + 4] = MUR;
                mur[x * 2 + 1][y * 2 + 5] = MUR; mur[x * 2 + 2][y * 2 + 5] = MUR;
                mur[x * 2 + 1][y * 2 + 6] = MUR; mur[x * 2 + 2][y * 2 + 6] = MUR;
            } else if (type == 1) {
                sol[x][y]=1955; sol[x+1][y]=1956;
                sol[x][y+1]=1957; sol[x+1][y+1]=1958;
                sol[x][y+2]=1959; sol[x+1][y+2]=1960;
                sol[x+2][y]=1949; sol[x+3][y]=1950;
                sol[x+2][y+1]=1951; sol[x+3][y+1]=1952;
                sol[x+2][y+2]=1953; sol[x+3][y+2]=1954;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 3][y * 2 + 2] = MUR; mur[x * 2 + 4][y * 2 + 2] = MUR;
                mur[x * 2 + 3][y * 2 + 3] = MUR; mur[x * 2 + 4][y * 2 + 3] = MUR;
                mur[x * 2 + 5][y * 2 + 2] = MUR; mur[x * 2 + 6][y * 2 + 2] = MUR;
                mur[x * 2 + 5][y * 2 + 3] = MUR; mur[x * 2 + 6][y * 2 + 3] = MUR;
            }
            break;
        case 61 :
            if (type == 0) {
                sol[x][y]=2177; sol[x + 1][y]=2178;
                sol[x][y + 1]=2179; sol[x + 1][y + 1]=2180;
                sol[x][y + 2]=2173; sol[x + 1][y + 2]=2174;
                sol[x][y + 3]=2175; sol[x + 1][y + 3]=2176;
                mur[x * 2 + 1][y * 2 + 1] = MUR; mur[x * 2 + 2][y * 2 + 1] = MUR;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 1][y * 2 + 4] = MUR; mur[x * 2 + 2][y * 2 + 4] = MUR;
                mur[x * 2 + 1][y * 2 + 5] = MUR; mur[x * 2 + 2][y * 2 + 5] = MUR;
                mur[x * 2 + 1][y * 2 + 6] = MUR; mur[x * 2 + 2][y * 2 + 6] = MUR;
            } else if (type == 1) {
                sol[x][y]=2187; sol[x+1][y]=2188;
                sol[x][y+1]=2189; sol[x+1][y+1]=2190;
                sol[x][y+2]=2191; sol[x+1][y+2]=2192;
                sol[x+2][y]=2181; sol[x+3][y]=2182;
                sol[x+2][y+1]=2183; sol[x+3][y+1]=2184;
                sol[x+2][y+2]=2185; sol[x+3][y+2]=2186;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 3][y * 2 + 2] = MUR; mur[x * 2 + 4][y * 2 + 2] = MUR;
                mur[x * 2 + 3][y * 2 + 3] = MUR; mur[x * 2 + 4][y * 2 + 3] = MUR;
                mur[x * 2 + 5][y * 2 + 2] = MUR; mur[x * 2 + 6][y * 2 + 2] = MUR;
                mur[x * 2 + 5][y * 2 + 3] = MUR; mur[x * 2 + 6][y * 2 + 3] = MUR;
            }
            break;
        case 62 :
            if (type == 0) {
                sol[x][y]=2398; sol[x + 1][y]=2399;
                sol[x][y + 1]=2400; sol[x + 1][y + 1]=2401;
                sol[x][y + 2]=2394; sol[x + 1][y + 2]=2395;
                sol[x][y + 3]=2396; sol[x + 1][y + 3]=2397;
                mur[x * 2 + 1][y * 2 + 1] = MUR; mur[x * 2 + 2][y * 2 + 1] = MUR;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 1][y * 2 + 4] = MUR; mur[x * 2 + 2][y * 2 + 4] = MUR;
                mur[x * 2 + 1][y * 2 + 5] = MUR; mur[x * 2 + 2][y * 2 + 5] = MUR;
                mur[x * 2 + 1][y * 2 + 6] = MUR; mur[x * 2 + 2][y * 2 + 6] = MUR;
            } else if (type == 1) {
                sol[x][y]=2408; sol[x+1][y]=2409;
                sol[x][y+1]=2410; sol[x+1][y+1]=2411;
                sol[x][y+2]=2412; sol[x+1][y+2]=2413;
                sol[x+2][y]=2402; sol[x+3][y]=2403;
                sol[x+2][y+1]=2404; sol[x+3][y+1]=2405;
                sol[x+2][y+2]=2406; sol[x+3][y+2]=2407;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 3][y * 2 + 2] = MUR; mur[x * 2 + 4][y * 2 + 2] = MUR;
                mur[x * 2 + 3][y * 2 + 3] = MUR; mur[x * 2 + 4][y * 2 + 3] = MUR;
                mur[x * 2 + 5][y * 2 + 2] = MUR; mur[x * 2 + 6][y * 2 + 2] = MUR;
                mur[x * 2 + 5][y * 2 + 3] = MUR; mur[x * 2 + 6][y * 2 + 3] = MUR;
            }
            break;
        case 63 :
            if (type == 0) {
                sol[x][y]=2672; sol[x + 1][y]=2673;
                sol[x][y + 1]=2674; sol[x + 1][y + 1]=2675;
                sol[x][y + 2]=2668; sol[x + 1][y + 2]=2669;
                sol[x][y + 3]=2670; sol[x + 1][y + 3]=2671;
                mur[x * 2 + 1][y * 2 + 1] = MUR; mur[x * 2 + 2][y * 2 + 1] = MUR;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 1][y * 2 + 4] = MUR; mur[x * 2 + 2][y * 2 + 4] = MUR;
                mur[x * 2 + 1][y * 2 + 5] = MUR; mur[x * 2 + 2][y * 2 + 5] = MUR;
                mur[x * 2 + 1][y * 2 + 6] = MUR; mur[x * 2 + 2][y * 2 + 6] = MUR;
            } else if (type == 1) {
                sol[x][y]=2682; sol[x+1][y]=2683;
                sol[x][y+1]=2684; sol[x+1][y+1]=2685;
                sol[x][y+2]=2686; sol[x+1][y+2]=2687;
                sol[x+2][y]=2676; sol[x+3][y]=2677;
                sol[x+2][y+1]=2678; sol[x+3][y+1]=2679;
                sol[x+2][y+2]=2680; sol[x+3][y+2]=2681;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 3][y * 2 + 2] = MUR; mur[x * 2 + 4][y * 2 + 2] = MUR;
                mur[x * 2 + 3][y * 2 + 3] = MUR; mur[x * 2 + 4][y * 2 + 3] = MUR;
                mur[x * 2 + 5][y * 2 + 2] = MUR; mur[x * 2 + 6][y * 2 + 2] = MUR;
                mur[x * 2 + 5][y * 2 + 3] = MUR; mur[x * 2 + 6][y * 2 + 3] = MUR;
            }
            break;
        case 64 :
            if (type == 0) {
                sol[x][y]=2880; sol[x + 1][y]=2881;
                sol[x][y + 1]=2882; sol[x + 1][y + 1]=2883;
                sol[x][y + 2]=2876; sol[x + 1][y + 2]=2877;
                sol[x][y + 3]=2878; sol[x + 1][y + 3]=2879;
                mur[x * 2 + 1][y * 2 + 1] = MUR; mur[x * 2 + 2][y * 2 + 1] = MUR;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 1][y * 2 + 4] = MUR; mur[x * 2 + 2][y * 2 + 4] = MUR;
                mur[x * 2 + 1][y * 2 + 5] = MUR; mur[x * 2 + 2][y * 2 + 5] = MUR;
                mur[x * 2 + 1][y * 2 + 6] = MUR; mur[x * 2 + 2][y * 2 + 6] = MUR;
            } else if (type == 1) {
                sol[x][y]=2890; sol[x+1][y]=2891;
                sol[x][y+1]=2892; sol[x+1][y+1]=2893;
                sol[x][y+2]=2894; sol[x+1][y+2]=2895;
                sol[x+2][y]=2884; sol[x+3][y]=2885;
                sol[x+2][y+1]=2886; sol[x+3][y+1]=2887;
                sol[x+2][y+2]=2888; sol[x+3][y+2]=2889;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 3][y * 2 + 2] = MUR; mur[x * 2 + 4][y * 2 + 2] = MUR;
                mur[x * 2 + 3][y * 2 + 3] = MUR; mur[x * 2 + 4][y * 2 + 3] = MUR;
                mur[x * 2 + 5][y * 2 + 2] = MUR; mur[x * 2 + 6][y * 2 + 2] = MUR;
                mur[x * 2 + 5][y * 2 + 3] = MUR; mur[x * 2 + 6][y * 2 + 3] = MUR;
            }
            break;
        case 65 :
            if (type == 0) {
                sol[x][y]=3100; sol[x + 1][y]=3101;
                sol[x][y + 1]=3102; sol[x + 1][y + 1]=3103;
                sol[x][y + 2]=3096; sol[x + 1][y + 2]=3097;
                sol[x][y + 3]=3098; sol[x + 1][y + 3]=3099;
                mur[x * 2 + 1][y * 2 + 1] = MUR; mur[x * 2 + 2][y * 2 + 1] = MUR;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 1][y * 2 + 4] = MUR; mur[x * 2 + 2][y * 2 + 4] = MUR;
                mur[x * 2 + 1][y * 2 + 5] = MUR; mur[x * 2 + 2][y * 2 + 5] = MUR;
                mur[x * 2 + 1][y * 2 + 6] = MUR; mur[x * 2 + 2][y * 2 + 6] = MUR;
            } else if (type == 1) {
                sol[x][y]=3110; sol[x+1][y]=3111;
                sol[x][y+1]=3112; sol[x+1][y+1]=3113;
                sol[x][y+2]=3114; sol[x+1][y+2]=3115;
                sol[x+2][y]=3104; sol[x+3][y]=3105;
                sol[x+2][y+1]=3106; sol[x+3][y+1]=3107;
                sol[x+2][y+2]=3108; sol[x+3][y+2]=3109;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 3][y * 2 + 2] = MUR; mur[x * 2 + 4][y * 2 + 2] = MUR;
                mur[x * 2 + 3][y * 2 + 3] = MUR; mur[x * 2 + 4][y * 2 + 3] = MUR;
                mur[x * 2 + 5][y * 2 + 2] = MUR; mur[x * 2 + 6][y * 2 + 2] = MUR;
                mur[x * 2 + 5][y * 2 + 3] = MUR; mur[x * 2 + 6][y * 2 + 3] = MUR;
            }
            break;
        case 66 :
            if (type == 0) {
                sol[x][y]=3266; sol[x + 1][y]=3267;
                sol[x][y + 1]=3268; sol[x + 1][y + 1]=3269;
                sol[x][y + 2]=3262; sol[x + 1][y + 2]=3263;
                sol[x][y + 3]=3264; sol[x + 1][y + 3]=3265;
                mur[x * 2 + 1][y * 2 + 1] = MUR; mur[x * 2 + 2][y * 2 + 1] = MUR;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 1][y * 2 + 4] = MUR; mur[x * 2 + 2][y * 2 + 4] = MUR;
                mur[x * 2 + 1][y * 2 + 5] = MUR; mur[x * 2 + 2][y * 2 + 5] = MUR;
                mur[x * 2 + 1][y * 2 + 6] = MUR; mur[x * 2 + 2][y * 2 + 6] = MUR;
            } else if (type == 1) {
                sol[x][y]=3276; sol[x+1][y]=3277;
                sol[x][y+1]=3278; sol[x+1][y+1]=3279;
                sol[x][y+2]=3280; sol[x+1][y+2]=3281;
                sol[x+2][y]=3270; sol[x+3][y]=3271;
                sol[x+2][y+1]=3272; sol[x+3][y+1]=3273;
                sol[x+2][y+2]=3274; sol[x+3][y+2]=3275;
                mur[x * 2 + 1][y * 2 + 2] = MUR; mur[x * 2 + 2][y * 2 + 2] = MUR;
                mur[x * 2 + 1][y * 2 + 3] = MUR; mur[x * 2 + 2][y * 2 + 3] = MUR;
                mur[x * 2 + 3][y * 2 + 2] = MUR; mur[x * 2 + 4][y * 2 + 2] = MUR;
                mur[x * 2 + 3][y * 2 + 3] = MUR; mur[x * 2 + 4][y * 2 + 3] = MUR;
                mur[x * 2 + 5][y * 2 + 2] = MUR; mur[x * 2 + 6][y * 2 + 2] = MUR;
                mur[x * 2 + 5][y * 2 + 3] = MUR; mur[x * 2 + 6][y * 2 + 3] = MUR;
            }
            break;
    }
}

void Map::removeStele() {

    int x = 0;
    int y = 0;

    switch (map) {
        case 9 : x = 58 * 16; y = 23 * 16; break;
        case 29 : x = 9 * 16; y = 5 * 16; break;
        default : return;
    }

    BoundingBox b(x, y, 16, 16);

    List list;
    objects->get(&b, &list);

    list.iterateOnFirst();
    while (list.hasNext()) {
        Stele* stele = dynamic_cast<Stele*>(list.getNext());
        if (stele != 0) {
            stele->explose();
        }
    }
}

void Map::songPlayed(int id) {
    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();
    Link* link = scene->getLink();
    BoundingBox b;

    switch (id) {
        case 0 :
            b.setW(6 * 16);
            b.setH(5 * 16);
            if (map == 9 && !scene->getFees(0)) {
                b.setX(37 * 16);
                b.setY(35 * 16);
                if (b.intersect(link->getBoundingBox())) {
                    scene->getAnimationInGame()->startAnim(10);
                    scene->setFees(0, 1);
                }
            } else if (map == 16 && !scene->getFees(1)) {
                b.setX(7 * 16);
                b.setY(7 * 16);
                if (b.intersect(link->getBoundingBox())) {
                    scene->getAnimationInGame()->startAnim(14);
                    scene->setFees(1, 1);
                }
            } else if (map == 17 && !scene->getFees(2)) {
                b.setX(7 * 16);
                b.setY(7 * 16);
                if (b.intersect(link->getBoundingBox())) {
                    scene->getAnimationInGame()->startAnim(15);
                    scene->setFees(2, 1);
                }
            } else if (map == 18 && !scene->getFees(3)) {
                b.setX(7 * 16);
                b.setY(7 * 16);
                if (b.intersect(link->getBoundingBox())) {
                    scene->getAnimationInGame()->startAnim(16);
                    scene->setFees(3, 1);
                }
            } else if (map == 19 && !scene->getFees(4)) {
                b.setX(7 * 16);
                b.setY(7 * 16);
                if (b.intersect(link->getBoundingBox())) {
                    scene->getAnimationInGame()->startAnim(15);
                    scene->setFees(4, 1);
                }
            } else if (map == 24 && !scene->getFees(5)) {
                b.setX(7 * 16);
                b.setY(7 * 16);
                if (b.intersect(link->getBoundingBox())) {
                    scene->getAnimationInGame()->startAnim(17);
                    scene->setFees(5, 1);
                }
            } else if (map == 25 && !scene->getFees(6)) {
                b.setX(7 * 16);
                b.setY(7 * 16);
                if (b.intersect(link->getBoundingBox())) {
                    scene->getAnimationInGame()->startAnim(15);
                    scene->setFees(6, 1);
                }
            } else if (map == 28 && !scene->getFees(7)) {
                b.setX(27 * 16);
                b.setY(7 * 16);
                if (b.intersect(link->getBoundingBox())) {
                    scene->getAnimationInGame()->startAnim(18);
                    scene->setFees(7, 1);
                }
            } else if (map == 65 && link->getEpee() < 5) {
                b.setX(47 * 16);
                b.setY(7 * 16);
                if (b.intersect(link->getBoundingBox())) {
                    scene->getAnimationInGame()->startAnim(42);
                    scene->setCoffre(8, 25, 1);
                }
            }
            break;
        case 1 :
            if (map < 15) {
                MainController::getInstance()->getGameController()->displayMap(map, true);
                AudioManager::getInstance()->playSound(TS_MENU1);
            }
            break;
        case 2 :
            if (map == 30 && link->getInventory()->nbMedaillons() == 7) {
                AudioManager::getInstance()->playSound(TS_SURPRISE);
                scene->setCoffre(0, 9, 1);
                sol[9][7] = 1261;
                sol[10][7] = 1261;
            }
            break;
        default : break;
    }
}

int Map::getEtage() {
    switch (map) {
        case 58 : return -(bounds.getX() / (100 * 16));
        case 59 : return (bounds.getY() / (75 * 16)) - 2;
        case 60 :
            if (bounds.getX() >= 100 * 16) return -1;
            if (bounds.getY() >= 45 * 16) return 0;
            if (bounds.getX() < 60 * 16) return 1;
            return 2;
        case 61 : return (bounds.getY() / (75 * 16)) - 2;
        case 62 : return (bounds.getY() / (75 * 16)) - 2;
        case 63 : return (bounds.getY() / (75 * 16)) - 2;
        case 64 :
            if (bounds.getX() < 140 * 16) return (bounds.getY() / (75 * 16)) - 2;
            return (bounds.getY() / (75 * 16));
        case 65 : return bounds.getX() / (100 * 16);
        case 66 : return -(bounds.getX() / (80 * 16));
        default : return 0;
    }
}

void Map::startAnimOniLink() {
    if (map != 66) return;

    Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();
    if (link->getStatus()->getVirtualLife() <= 0) return;

    Ennemi153* enn = getFinalBoss();
    if (enn != 0 && enn->finalForm()) {
        MainController::getInstance()->getGameController()->getSceneController()->getScene()->getAnimationInGame()->startAnim(44);
        return;
    }
    link->setFatalAnim();
    MainController::getInstance()->getGameController()->getSceneController()->getScene()->getAnimationInGame()->startAnim(43);
}

Ennemi153* Map::getFinalBoss() {
    if (map != 66 || bounds.getX() != 320 * 9 || bounds.getY() != 240) return 0;

    List l;
    ennemis->get(&bounds, &l);
    l.iterateOnFirst();
    while (l.hasNext()) {
        Ennemi153* enn = dynamic_cast<Ennemi153*>(l.getNext());
        if (enn != 0) return enn;
    }
    return 0;
}
