#include "Sword.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

#include "../../MainController.h"

Sword::Sword(int i, bool o) : onilink(o) {
    force = i;
    if (onilink) {
        image = ResourceManager::getInstance()->loadImage("data/images/link/epeeOni.png", true);
    } else {
        ostringstream os;
        os << i;
        string filename = "data/images/link/epee" + os.str() + ".png";
        image = ResourceManager::getInstance()->loadImage(filename, true);
    }
    effects = ResourceManager::getInstance()->loadImage("data/images/link/effects.png", true);
}

Sword::~Sword() {
    ResourceManager::getInstance()->free(image);
    ResourceManager::getInstance()->free(effects);
}

int Sword::getForce() {
    return force;
}

bool Sword::hit(int offsetX, int offsetY, Direction dir, Animation anim, int step, int charge, int forceTotal) {

    BoundingBox box;
    int i, j, a, b;

    if (anim == HIT_EPEE) {
        switch (dir) {
            case N :
                switch(step) {
                    case 0 : i=9; j=7; a=14; b=6; if (force > 1 || onilink) { j=6; a=16; b=7;} break;
                    case 1 : i=9; j=-6; a=14; b=18; if (force > 1 || onilink) { i=3; j=-13; a=22; b=24;} break;
                    case 2 : i=0; j=-8; a=16; b=16; if (force > 1 || onilink) { j=-14; a=16; b=24;} break;
                    case 3 : i=-4; j=-6; a=14; b=14; if (force > 1 || onilink) { i=-14; a=24; b=16;} break;
                    case 4 : i=-12; j=-6; a=14; b=22; if (force > 1 || onilink) { i=-14; a=16;} break;
                    default : return false;
                }
                break;
            case S :
                switch(step) {
                    case 0 : i=-5; j=12; a=12; b=6; if (force > 1 || onilink) { i=-7; j=14; a=14; b=8;} break;
                    case 1 : i=-6; j=12; a=14; b=14; if (force > 1 || onilink) { i=-5; j=14; b=21;} break;
                    case 2 : i=0; j=21; a=16; b=12; if (force > 1 || onilink) { j=20; b=19;} break;
                    case 3 : i=9; j=17; a=15; b=14; if (force > 1 || onilink) { b=16;} break;
                    case 4 : i=16; j=15; a=12; b=14; if (force > 1 || onilink) { a=14; b=13;} break;
                    default : return false;
                }
                break;
            case W :
                switch(step) {
                    case 0 : i=-2; j=1; a=6; b=12; if (force > 1 || onilink) { i=-3; j=-1; a=8; b=14;} break;
                    case 1 : i=-10; j=0; a=12; b=14; if (force > 1 || onilink) { i=-14; a=18; b=14;} break;
                    case 2 : i=-13; j=0; a=16; b=16; if (force > 1 || onilink) { i=-20; a=24; b=20;} break;
                    case 3 : i=-12; j=13; a=14; b=10; if (force > 1 || onilink) { i=-14; j=12; a=16; b=18;} break;
                    case 4 : i=-7; j=16; a=14; b=18; if (force > 1 || onilink) { i=-9; j=16; a=16; b=19;} break;
                    default : return false;
                }
                i += 2;
                break;
            case E :
                switch(step) {
                    case 0 : i=12; j=1; a=6; b=12; if (force > 1 || onilink) { i=11; j=-1; a=8; b=14;} break;
                    case 1 : i=14; j=0; a=12; b=14; if (force > 1 || onilink) { i=12; a=18; b=14;} break;
                    case 2 : i=13; j=0; a=16; b=16; if (force > 1 || onilink) { i=12; a=24; b=20;} break;
                    case 3 : i=14; j=13; a=14; b=10; if (force > 1 || onilink) { i=14; j=12; a=16; b=18;} break;
                    case 4 : i=9; j=16; a=14; b=18; if (force > 1 || onilink) { i=9; j=16; a=16; b=19;} break;
                    default : return false;
                }
                i -= 2;
                break;
        }
        box.setX(i + offsetX);
        box.setY(j + offsetY);
        box.setW(a);
        box.setH(b);

        MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->getMap()->pickUpItems(&box);

        return MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->
            testDegat(&box, dir, forceTotal, TA_SWORD, TE_NORMAL);

    } else if (anim == SPIN_EPEE) {

        bool tmp = false;

        switch (dir) {
            case N :
                if (step==0 || step==5 || (step>=21 && step<=26)) {i=-5; j=17; a=8; b=14; dir=S; break;}
                if (step>=1 && step<=4) {i=3; j=20; a=8; b=14; dir=S;break;}
                if (step==6) {i=-14; j=15; a=14; b=14; dir=W; break;}
                if (step==7 || step==8) {i=-14; j=7; a=14; b=14; dir=W; break;}
                if (step==9 || step==10) {i=-14; j=-6; a=22; b=14; dir=N; break;}
                if (step==11 || step==12) {i=4; j=-6; a=14; b=14; dir=N; break;}
                if (step==13 || step==14) {i=16; j=-6; a=14; b=21; dir=E; break;}
                if (step==15 || step==16) {i=16; j=10; a=14; b=14; dir=E; break;}
                if (step==17 || step==18) {i=7; j=21; a=14; b=14; dir=S; break;}
                if (step==19 || step==20) {i=-2; j=21; a=14; b=14; dir=S; break;}
                return false;
            case S :
                if (step==0 || step==5 || (step>=21 && step<=26)) {i=13; j=-4; a=8; b=14; dir=N; break;}
                if (step>=1 && step<=4) {i=6; j=-8; a=8; b=14; dir=N;break;}
                if (step==6) {i=16; j=1; a=14; b=14; dir=E; break;}
                if (step==7 || step==8) {i=16; j=10; a=14; b=14; dir=E; break;}
                if (step==9 || step==10) {i=7; j=21; a=22; b=14; dir=S; break;}
                if (step==11 || step==12) {i=-2; j=21; a=14; b=14; dir=S; break;}
                if (step==13 || step==14) {i=-14; j=15; a=14; b=21; dir=W; break;}
                if (step==15 || step==16) {i=-14; j=7; a=14; b=14; dir=W; break;}
                if (step==17 || step==18) {i=-6; j=-6; a=14; b=14; dir=N; break;}
                if (step==19 || step==20) {i=11; j=16; a=14; b=14; dir=N; break;}
                return false;
            case W :
                if (step==0 || step==5 || (step>=21 && step<=26)) {i=11; j=16; a=14; b=8; dir=E; break;}
                if (step>=1 && step<=4) {i=13; j=9; a=8; b=14; dir=E;break;}
                if (step==6) {i=7; j=20; a=14; b=14; dir=S; break;}
                if (step==7 || step==8) {i=-2; j=20; a=14; b=14; dir=S; break;}
                if (step==9 || step==10) {i=-14; j=15; a=14; b=20; dir=W; break;}
                if (step==11 || step==12) {i=-14; j=7; a=14; b=14; dir=W; break;}
                if (step==13 || step==14) {i=-6; j=-6; a=14; b=21; dir=N; break;}
                if (step==15 || step==16) {i=4; j=-6; a=14; b=14; dir=N; break;}
                if (step==17 || step==18) {i=16; j=1; a=14; b=14; dir=E; break;}
                if (step==19 || step==20) {i=16; j=10; a=14; b=14; dir=E; break;}
                return false;
            case E :
                if (step==0 || step==5 || (step>=21 && step<=26)) {i=-10; j=5; a=14; b=8; dir=W; break;}
                if (step>=1 && step<=4) {i=-10; j=12; a=14; b=8; dir=W;break;}
                if (step==6) {i=-6; j=-6; a=14; b=14; dir=N; break;}
                if (step==7 || step==8) {i=4; j=-6; a=14; b=14; dir=N; break;}
                if (step==9 || step==10) {i=16; j=-6; a=14; b=21; dir=E; break;}
                if (step==11 || step==12) {i=16; j=10; a=14; b=14; dir=E; break;}
                if (step==13 || step==14) {i=7; j=21; a=21; b=14; dir=S; break;}
                if (step==15 || step==16) {i=-2; j=21; a=14; b=14; dir=S; break;}
                if (step==17 || step==18) {i=-14; j=15; a=14; b=14; dir=W; break;}
                if (step==19 || step==20) {i=-14; j=7; a=14; b=14; dir=W; break;}
                return false;
        }

        box.setX(i + offsetX);
        box.setY(j + offsetY);
        box.setW(a);
        box.setH(b);

        MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->getMap()->pickUpItems(&box);

        tmp = MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->
            testDegat(&box, dir, forceTotal * 2, TA_SWORD, TE_NORMAL);

        // light
        i = 0; j = 0;
        Direction dir2 = dir;
        switch (step) {
            case 0 :
                box.setW(5); box.setH(5);
                if (dir==N) { i += 7; j -= 8;}
                if (dir==S) { i += 6; j += 30;}
                if (dir==W) { i -= 12; j += 17;}
                if (dir==E) { i += 23; j += 15;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 1 :
                box.setW(1); box.setH(1);
                if (dir==N) { i += 9; j -= 6; dir2=N;}
                if (dir==S) { i += 8; j += 32; dir2=S;}
                if (dir==W) { i -= 10; j += 19; dir2=W;}
                if (dir==E) { i += 25; j += 17; dir2=E;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(8); box.setH(8);
                if (dir==N) { i -= 3; j += 35; dir2=S;}
                if (dir==S) { i -= 5; j -= 44; dir2=N;}
                if (dir==W) { i += 32; j -= 13; dir2=E;}
                if (dir==E) { i -= 38; j -= 1; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 2 :
                box.setW(1); box.setH(1);
                if (dir==N) { i += 9; j -= 6;}
                if (dir==S) { i += 8; j += 32;}
                if (dir==W) { i -= 10; j += 19;}
                if (dir==E) { i += 25; j += 17;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(14); box.setH(14);
                if (dir==N) { i -= 6; j += 32; dir2=S;}
                if (dir==S) { i -= 8; j -= 47; dir2=N;}
                if (dir==W) { i += 29; j -= 16; dir2=E;}
                if (dir==E) { i -= 41; j -= 4; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 3 :
                box.setW(14); box.setH(14);
                if (dir==N) { i += 3; j += 26; dir2=S;}
                if (dir==S) { j -= 15; dir2=N;}
                if (dir==W) { i += 19; j += 3; dir2=E;}
                if (dir==E) { i -= 15; j += 13; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 4 :
            case 5 :
                box.setW(14); box.setH(14);
                if (dir==N) { i += 3; j += 26; dir2=S;}
                if (dir==S) { j -= 15; dir2=N;}
                if (dir==W) { i += 19; j += 3; dir2=E;}
                if (dir==E) { i -= 15; j += 13; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 6 :
                box.setW(16); box.setH(16);
                if (dir==N) { i += 2; j += 25; dir2=S;}
                if (dir==S) { i -= 1; j -= 15; dir2=N;}
                if (dir==W) { i += 19; j += 2; dir2=E;}
                if (dir==E) { i -= 17; j += 12; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 7 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 5; j += 27; dir2=S;}
                if (dir==S) { i += 17; j -= 6; dir2=N;}
                if (dir==W) { i += 20; j += 19; dir2=E;}
                if (dir==E) { i -= 13; j -= 1; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 8 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 11; j += 22; dir2=S;}
                if (dir==S) { i += 22; dir2=N;}
                if (dir==W) { i += 14; j += 25; dir2=E;}
                if (dir==E) { i -= 8; j -= 7; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 7; j += 6;}
                if (dir==S) { i -= 7; j -= 7;}
                if (dir==W) { i += 7; j -= 7;}
                if (dir==E) { i -= 5; j += 8;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 9 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 14; j += 15; dir2=W;}
                if (dir==S) { i += 23; j += 7; dir2=E;}
                if (dir==W) { i += 8; j += 28; dir2=S;}
                if (dir==E) { i -= 2; j -= 10; dir2=N;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 5; j += 9; dir2=S;}
                if (dir==S) { i -= 3; j -= 8; dir2=N;}
                if (dir==W) { i += 9; j -= 4; dir2=E;}
                if (dir==E) { i -= 8; j += 6; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 9; j += 6;}
                if (dir==S) { i -= 7; j -= 6;}
                if (dir==W) { i += 6; j -= 6;}
                if (dir==E) { i -= 4; j += 9;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 10 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 15; j += 7; dir2=W;}
                if (dir==S) { i += 22; j += 15; dir2=E;}
                if (dir==W) { j += 28; dir2=S;}
                if (dir==E) { i += 6; j -= 11; dir2=N;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 11; j += 11;}
                if (dir==S) { i += 1; j -= 9;}
                if (dir==W) i += 10;
                if (dir==E) { i -= 10; j += 3;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 6; j += 8; dir2=S;}
                if (dir==S) { i -= 4; j -= 7; dir2=N;}
                if (dir==W) { i += 9; j -= 4; dir2=E;}
                if (dir==E) { i -= 6; j += 6; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 11; j += 6;}
                if (dir==S) { i -= 5; j -= 4;}
                if (dir==W) { i += 7; j -= 7;}
                if (dir==E) { i -= 3; j += 10;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 11 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 13; dir2=W;}
                if (dir==S) { i += 18; j += 22; dir2=E;}
                if (dir==W) { i -= 7; j += 25; dir2=S;}
                if (dir==E) { i += 13; j -= 9; dir2=N;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i -= 2; j += 10;}
                if (dir==S) { i += 5; j -= 8;}
                if (dir==W) { i += 9; j += 4;}
                if (dir==E) { i -= 9; j -= 1;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 3; j += 10;}
                if (dir==S) { j -= 9;}
                if (dir==W) { i += 11; j -= 1;}
                if (dir==E) { i -= 8; j += 3;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 9; j += 9; dir2=S;}
                if (dir==S) { i -= 3; j -= 7; dir2=N;}
                if (dir==W) { i += 9; j -= 4; dir2=E;}
                if (dir==E) { i -= 6; j += 8; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 12 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 8; j -= 6; dir2=W;}
                if (dir==S) { i += 12; j += 27; dir2=E;}
                if (dir==W) { i -= 13; j += 19; dir2=S;}
                if (dir==E) { i += 19; j -= 4; dir2=N;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i -= 5; j += 8;}
                if (dir==S) { i += 8; j -= 6;}
                if (dir==W) { i += 8; j += 8;}
                if (dir==E) { i -= 7; j += 4;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i -= 1; j += 11;}
                if (dir==S) { i += 4; j -= 8;}
                if (dir==W) { i += 10; j += 3;}
                if (dir==E) { i -= 9; j -= 1;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 5; j += 10;}
                if (dir==S) { j -= 9;}
                if (dir==W) { i += 11; j -= 1;}
                if (dir==E) { i -= 7; j += 4;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 13 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 2; j -= 10;}
                if (dir==S) { i += 6; j += 29;}
                if (dir==W) { i -= 15; j += 12;}
                if (dir==E) { i += 22; j += 3;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i -= 8; j += 6; dir2=W;}
                if (dir==S) { i += 9; j -= 2; dir2=E;}
                if (dir==W) { i += 5; j += 10; dir2=S;}
                if (dir==E) { i -= 5; j -= 7; dir2=N;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i -= 4; j += 9;}
                if (dir==S) { i += 8; j -= 7;}
                if (dir==W) { i += 8; j += 6;}
                if (dir==E) { i -= 7; j -= 4;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 1; j += 11;}
                if (dir==S) { i += 4; j -= 8;}
                if (dir==W) { i += 12; j += 3;}
                if (dir==E) i -= 8;
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 14 :
                box.setW(8); box.setH(8);
                if (dir==N) { i += 6; j -= 11;}
                if (dir==S) { i -= 2; j += 28;}
                if (dir==W) { i -= 15; j += 4;}
                if (dir==E) { i += 23; j += 11;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i -= 10; j += 3;}
                if (dir==S) { i += 10; j += 2;}
                if (dir==W) { i += 1; j += 11;}
                if (dir==E) { i -= 2; j -= 9;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i -= 6; j += 6; dir2=W;}
                if (dir==S) { i += 10; j -= 4; dir2=E;}
                if (dir==W) { i += 6; j += 9; dir2=S;}
                if (dir==E) { i -= 4; j += 6; dir2=N;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i -= 3; j += 10;}
                if (dir==S) { i += 7; j -= 6;}
                if (dir==W) { i += 10; j += 6;}
                if (dir==E) { i -= 7; j -= 3;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 15 :
                box.setW(8); box.setH(8);
                if (dir==N) { i += 13; j -= 9;}
                if (dir==S) { i -= 9; j += 24;}
                if (dir==W) { i -= 12; j -= 3;}
                if (dir==E) { i += 21; j += 18;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i -= 9; j -= 1;}
                if (dir==S) { i += 9; j += 6;}
                if (dir==W) { i -= 3; j += 10;}
                if (dir==E) { i += 2; j -= 8;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i -= 8; j += 3;}
                if (dir==S) i += 11;
                if (dir==W) { i += 3; j += 11;}
                if (dir==E) { i -= 1; j -= 8;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i -= 6; j += 8; dir2=W;}
                if (dir==S) { i += 10; j -= 4; dir2=E;}
                if (dir==W) { i += 7; j += 8; dir2=S;}
                if (dir==E) { i -= 5; j -= 6; dir2=N;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 16 :
                box.setW(8); box.setH(8);
                if (dir==N) { i += 19; j -= 4;}
                if (dir==S) { i -= 14; j += 18;}
                if (dir==W) { i -= 6; j -= 9;}
                if (dir==E) { i += 16; j += 24;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i -= 7; j -= 4;}
                if (dir==S) { i += 7; j += 9;}
                if (dir==W) { i -= 7; j += 9;}
                if (dir==E) { i += 5; j -= 6;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i -= 9; j -= 1;}
                if (dir==S) { i += 10; j += 4;}
                if (dir==W) { i -= 1; j += 10;}
                if (dir==E) { i += 3; j -= 9;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i -= 7; j += 4;}
                if (dir==S) { i += 12; j -= 1;}
                if (dir==W) { i += 4; j += 10;}
                if (dir==E) { i -= 1; j -= 8;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 17 :
                box.setW(8); box.setH(8);
                if (dir==N) { i += 22; j += 3;}
                if (dir==S) { i -= 15; j += 11;}
                if (dir==W) j -= 11;
                if (dir==E) { i += 10; j += 28;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i -= 5; j -= 7;}
                if (dir==S) { i += 3; j += 10;}
                if (dir==W) { i -= 9; j += 6;}
                if (dir==E) { i += 8; j -= 4;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i -= 7; j -= 4;}
                if (dir==S) { i += 9; j += 8;}
                if (dir==W) { i -= 4; j += 8;}
                if (dir==E) { i += 6; j -= 7;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i -= 8; dir2=E;}
                if (dir==S) { i += 11; j += 3; dir2=W;}
                if (dir==W) { j += 11; dir2=N;}
                if (dir==E) { i += 3; j -= 9; dir2=S;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 18 :
                box.setW(8); box.setH(8);
                if (dir==N) { i += 23; j += 11;}
                if (dir==S) { i -= 14; j += 3;}
                if (dir==W) { i += 8; j -= 11;}
                if (dir==E) { i += 2; j += 29;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i -= 2; j -= 9;}
                if (dir==S) { i -= 1; j += 11;}
                if (dir==W) { i -= 10; j += 2;}
                if (dir==E) { i += 10; j -= 1;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i -= 4; j -= 6; dir2=E;}
                if (dir==S) { i += 6; j += 10; dir2=W;}
                if (dir==W) { i -= 7; j += 6; dir2=N;}
                if (dir==E) { i += 8; j -= 4; dir2=S;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i -= 7; j -= 4;}
                if (dir==S) { i += 9; j += 6;}
                if (dir==W) { i -= 3; j += 9;}
                if (dir==E) { i += 7; j -= 8;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 19 :
                box.setW(8); box.setH(8);
                if (dir==N) { i += 21; j += 18;}
                if (dir==S) { i -= 10; j -= 4;}
                if (dir==W) { i += 15; j -= 8;}
                if (dir==E) { i -= 5; j += 27;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 2; j -= 8; dir2=E;}
                if (dir==S) { i -= 5; j += 10; dir2=W;}
                if (dir==W) { i -= 9; j -= 2; dir2=N;}
                if (dir==E) { i += 9; j += 3; dir2=S;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i -= 1; j -= 8;}
                if (dir==S) { i += 2; j += 11;}
                if (dir==W) { i -= 9; j += 3;}
                if (dir==E) { i += 10; j -= 1;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i -= 5; j -= 7;}
                if (dir==S) { i += 7; j += 9;}
                if (dir==W) { i -= 5; j += 6;}
                if (dir==E) { i += 10; j -= 6;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 20 :
                box.setW(8); box.setH(8);
                if (dir==N) { i += 16; j += 24; dir2=E;}
                if (dir==S) { i -= 4; j -= 9; dir2=W;}
                if (dir==W) { i += 21; j -= 2; dir2=N;}
                if (dir==E) { i -= 11; j += 22; dir2=S;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 5; j -= 6;}
                if (dir==S) { i -= 8; j += 8;}
                if (dir==W) { i -= 8; j -= 6;}
                if (dir==E) { i += 7; j += 6;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 3; j -= 9;}
                if (dir==S) { i -= 2; j += 10;}
                if (dir==W) { i -= 8; j -= 1;}
                if (dir==E) { i += 11; j += 3;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i -= 1; j -= 8;}
                if (dir==S) { i += 4; j += 11;}
                if (dir==W) { i -= 7; j += 3;}
                if (dir==E) { i += 11; j -= 2;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 21 :
                box.setW(8); box.setH(8);
                if (dir==N) { i += 10; j += 28; dir2=S;}
                if (dir==S) { i += 2; j -= 12; dir2=N;}
                if (dir==W) { i += 23; j += 4; dir2=E;}
                if (dir==E) { i -= 14; j += 14; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 8; j -= 4; dir2=E;}
                if (dir==S) { i -= 9; j += 4; dir2=W;}
                if (dir==W) { i -= 5; j -= 8; dir2=N;}
                if (dir==E) { i += 5; j += 9; dir2=S;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 6; j -= 7;}
                if (dir==S) { i -= 6; j += 9;}
                if (dir==W) { i -= 6; j -= 4;}
                if (dir==E) { i += 9; j += 6;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 3; j -= 9;}
                if (dir==S) j += 10;
                if (dir==W) { i -= 8; j -= 1;}
                if (dir==E) { i += 12; j += 2;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 22 :
                box.setW(8); box.setH(8);
                if (dir==N) { i += 2; j += 29; dir2=S;}
                if (dir==S) { i += 10; j -= 11; dir2=N;}
                if (dir==W) { i += 23; j += 12; dir2=E;}
                if (dir==E) { i -= 15; j += 6; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 10; j -= 1;}
                if (dir==S) i -= 10;
                if (dir==W) { i -= 1; j -= 9;}
                if (dir==E) { i += 2; j += 11;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 8; j -= 4; dir2=E;}
                if (dir==S) { i -= 8; j += 6; dir2=W;}
                if (dir==W) { i -= 4; j -= 7; dir2=N;}
                if (dir==E) { i += 6; j += 8; dir2=S;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 7; j -= 8;}
                if (dir==S) { i -= 3; j += 8;}
                if (dir==W) { i -= 6; j -= 4;}
                if (dir==E) { i += 11; j += 6;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 23 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 5; j += 27; dir2=S;}
                if (dir==S) { i += 17; j -= 7; dir2=N;}
                if (dir==W) { i += 20; j += 19; dir2=E;}
                if (dir==E) { i -= 13; j -= 1; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 9; j += 3;}
                if (dir==S) { i -= 9; j -= 4;}
                if (dir==W) { i += 3; j -= 8;}
                if (dir==E) { i -= 2; j += 10;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 10; j -= 1;}
                if (dir==S) { i -= 9; j += 2;}
                if (dir==W) { i -= 1; j -= 9;}
                if (dir==E) { i += 3; j += 10;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 10; j -= 6; dir2=E;}
                if (dir==S) { i -= 6; j += 6; dir2=W;}
                if (dir==W) { i -= 3; j -= 6; dir2=N;}
                if (dir==E) { i += 9; j += 9; dir2=S;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 24 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 11; j += 22; dir2=S;}
                if (dir==S) { i += 22; j -= 1; dir2=N;}
                if (dir==W) { i += 14; j += 25; dir2=E;}
                if (dir==E) { i -= 8; j -= 7; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 7; j += 6;}
                if (dir==S) { i -= 7; j -= 7;}
                if (dir==W) { i += 7; j -= 7;}
                if (dir==E) { i -= 5; j += 8;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 11; j += 3;}
                if (dir==S) { i -= 8; j -= 2;}
                if (dir==W) { i += 3; j -= 8;}
                if (dir==E) { i -= 1; j += 11;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 11; j -= 2;}
                if (dir==S) { i -= 8; j += 3;}
                if (dir==W) j -= 8;
                if (dir==E) { i += 5; j += 10;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 25 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 15; j += 15; dir2=W;}
                if (dir==S) { i += 24; j += 6; dir2=E;}
                if (dir==W) { i += 7; j += 28; dir2=S;}
                if (dir==E) { i -= 6; j -= 9; dir2=N;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 5; j += 9; dir2=S;}
                if (dir==S) { i -= 3; j -= 8; dir2=N;}
                if (dir==W) { i += 9; j -= 4; dir2=E;}
                if (dir==E) { i -= 7; j += 9; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 9; j += 6;}
                if (dir==S) { i -= 7; j -= 6;}
                if (dir==W) { i += 6; j -= 6;}
                if (dir==E) { i -= 1; j += 10;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 12; j += 2;}
                if (dir==S) { i -= 7; j -= 1;}
                if (dir==W) { i += 4; j -= 9;}
                if (dir==E) { i += 4; j += 10;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 26 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 16; j += 7; dir2=W;}
                if (dir==S) { i += 23; j += 14; dir2=E;}
                if (dir==W) { i -= 1; j += 28; dir2=S;}
                if (dir==E) { i += 7; j -= 12; dir2=N;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i += 2; j += 11;}
                if (dir==S) { i += 1; j -= 9;}
                if (dir==W) i += 10;
                if (dir==E) { i -= 10; j += 3;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 6; j += 8; dir2=S;}
                if (dir==S) { i -= 4; j -= 8; dir2=N;}
                if (dir==W) { i += 9; j -= 4; dir2=E;}
                if (dir==E) { i -= 6; j += 6; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 11; j += 6;}
                if (dir==S) { i -= 5; j -= 4;}
                if (dir==W) { i += 7; j -= 7;}
                if (dir==E) { i -= 3; j += 10;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 27 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 14; dir2=W;}
                if (dir==S) { i += 19; j += 22; dir2=E;}
                if (dir==W) { i -= 8; j += 25; dir2=S;}
                if (dir==E) { i += 14; j -= 10; dir2=N;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(7); box.setH(7);
                if (dir==N) { i -= 2; j += 10;}
                if (dir==S) { i += 5; j -= 8;}
                if (dir==W) { i += 9; j += 4;}
                if (dir==E) { i -= 9; j -= 1;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(5); box.setH(5);
                if (dir==N) { i += 3; j += 10;}
                if (dir==S) j -= 9;
                if (dir==W) { i += 11; j -= 1;}
                if (dir==E) { i -= 8; j += 3;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);

                box.setW(1); box.setH(1);
                if (dir==N) { i += 9; j += 9; dir2=S;}
                if (dir==S) { i -= 3; j -= 7; dir2=N;}
                if (dir==W) { i += 9; j -= 4; dir2=E;}
                if (dir==E) { i -= 6; j += 8; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 28 :
                box.setW(14); box.setH(14);
                if (dir==N) { i -= 10; j += 22; dir2=S;}
                if (dir==S) { i += 14; j -= 8; dir2=N;}
                if (dir==W) { i += 17; j += 17; dir2=E;}
                if (dir==E) { i -= 14; j -= 1; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 29 :
                box.setW(8); box.setH(8);
                if (dir==N) { i -= 7; j += 25; dir2=S;}
                if (dir==S) { i += 17; j -= 5; dir2=N;}
                if (dir==W) { i += 20; j += 20; dir2=E;}
                if (dir==E) { i -= 11; j += 2; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
            case 30 :
                box.setW(5); box.setH(5);
                if (dir==N) { i -= 6; j += 27; dir2=S;}
                if (dir==S) { i += 18; j -= 3; dir2=N;}
                if (dir==W) { i += 21; j += 22; dir2=E;}
                if (dir==E) { i -= 10; j += 4; dir2=W;}
                box.setX(i + offsetX); box.setY(j + offsetY);
                tmp = tmp || MainController::getInstance()->getGameController()->
                    getSceneController()->getScene()->
                    testDegat(&box, dir2, forceTotal * 2, TA_SWORD, TE_NORMAL);
                break;
        }

        return tmp;

    } else if (charge) {

        bool move = anim == WALK;
        int step2 = step / 2;
        switch (dir) {
            case N : i = 4; j = -2; a = 8; b = 12;
                if (move && step2) j++;
                if (force > 1 || onilink) {i -= 2; j -= 2; b = 15;} break;
            case S : i = 8; j = 19; a = 8; b = 12;
                if (move && step2) j++;
                if (force > 1 || onilink) {i--; b = 14;} break;
            case W : i = -8; a = 12; b = 8; j = 14;
                if (move && step2) j++;
                if (force > 1 || onilink) {i -= 3; a = 15;} break;
            case E : i = 12; a = 12; b = 8; j = 14;
                if (move && step2) j++;
                if (force > 1 || onilink) a = 15; break;
        }

        box.setX(i + offsetX);
        box.setY(j + offsetY);
        box.setW(a);
        box.setH(b);

        MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->getMap()->pickUpItems(&box);

        return MainController::getInstance()->getGameController()->
            getSceneController()->getScene()->
            testDegat(&box, dir, forceTotal, TA_SWORD_HOLD, TE_NORMAL);

    }
    return false;
}

void Sword::draw(int offsetX, int offsetY, Direction dir, Animation anim, int step, int charge) {
    if (anim == HIT_EPEE) {
        drawHit(offsetX, offsetY, dir, step);
    } else if (anim == SPIN_EPEE) {
        drawSpin(offsetX, offsetY, dir, step);
    } else if (charge) {
        drawCharge(offsetX, offsetY, dir, anim == WALK, (step / 2), charge);
    }
}

void Sword::drawHit(int dstX, int dstY, Direction dir, int step) {

    int srcW = (force == 1 && !onilink) ? 14 : 20;
    int srcX = srcW * dir;
    int srcY = srcW * step;

    switch (dir) {
        case N :
            switch(step) {
                case 0 : dstX += 11; dstY += 7; if (force > 1 || onilink) {dstX += 2; dstY--;} break;
                case 1 : dstX += 8; dstY -= 6; if (force > 1 || onilink) {dstX -= 5; dstY -= 7;} break;
                case 2 : dstY -= 8; if (force > 1 || onilink) dstY -= 7; break;
                case 3 : dstX -= 4; dstY -= 6; if (force > 1 || onilink) dstX -= 10; break;
                case 4 : dstX -= 12; dstY ++; if (force > 1 || onilink) {dstX -= 2; dstY++;} break;
            }
            break;
        case S :
            switch(step) {
                case 0 : dstX -= 5; dstY += 12; if (force > 1 || onilink) {dstX -= 2; dstY += 2;} break;
                case 1 : dstX -= 6; dstY += 12; if (force > 1 || onilink) {dstX++; dstY += 7;} break;
                case 2 : dstX += 9; dstY += 21; if (force > 1 || onilink) dstY--; break;
                case 3 : dstX += 10; dstY += 17; if (force > 1 || onilink) dstX += 4; break;
                case 4 : dstX += 16; dstY += 15; break;
            }
            break;
        case W :
            switch(step) {
                case 0 : dstX -= 0; dstY += 1; if (force > 1 || onilink) {dstX--; dstY -= 2;} break;
                case 1 : dstX -= 10; if (force > 1 || onilink) {dstX -= 4; dstY++;} break;
                case 2 : dstX -= 13; dstY += 13; if (force > 1 || onilink) {dstX -= 7; dstY--;} break;
                case 3 : dstX -= 12; dstY += 16; if (force > 1 || onilink) dstX -= 2; break;
                case 4 : dstX -= 7; dstY += 21; break;
            }
            break;
        case E :
            switch(step) {
                default :
                case 0 : dstX += 9; dstY += 1; if (force > 1 || onilink) {dstX += 4; dstY -= 2;} break;
                case 1 : dstX += 14; if (force > 1 || onilink) dstY++; break;
                case 2 : dstX += 15; dstY += 13; if (force > 1 || onilink) dstY--; break;
                case 3 : dstX += 15; dstY += 16; break;
                case 4 : dstX += 8; dstY += 21; break;
            }
            break;
    }
    WindowManager::getInstance()->draw(image, srcX, srcY, srcW, srcW, dstX, dstY);
}

void Sword::drawSpin(int dstX, int dstY, Direction dir, int step) {

    int dstX0 = dstX;
    int dstY0 = dstY;

    int srcX = 0;
    int srcY = 0;
    int srcW = (force == 1 && !onilink) ? 12 : 14;

    switch (dir) {
        case N :
            switch (step) {
                case 0 : case 5 : case 21 : case 22 : case 23 : case 24 : case 25 : case 26 :
                    dstX -= 4; dstY += 17; srcX = 68; srcY = 12;
                    if (force > 1 || onilink) {dstX--; srcX = 94; srcY = 14;}
                    break;
                case 1 : case 2 : case 3 : case 4 :
                    dstX += 3; dstY += 20; srcX = 56; srcY = 12;
                    if (force > 1 || onilink) {srcX = 80; srcY = 14;}
                    break;
                case 6 :
                    dstX -= 12; dstY += 15; srcX = 56; srcY = 24;
                    if (force > 1 || onilink) {dstX -= 2; srcX = 80; srcY = 28;}
                    break;
                case 7 : case 8 :
                    dstX -= 12; dstY += 8; srcX = 56; srcY = 36;
                    if (force > 1 || onilink) {dstX -= 2; dstY--; srcX = 80; srcY = 42;}
                    break;
                case 9 : case 10 :
                    dstX++; dstY -= 4; srcX = 68; srcY = 0;
                    if (force > 1 || onilink) {dstX--; dstY-=2; srcX = 94; srcY = 0;}
                    break;
                case 11 : case 12 :
                    dstX += 10; dstY -= 4; srcX = 56; srcY = 0;
                    if (force > 1 || onilink) {dstY -= 2; srcX = 80; srcY = 0;}
                    break;
                case 13 : case 14 :
                    dstX += 16; dstY += 8; srcX = 68; srcY = 36;
                    if (force > 1 || onilink) {dstY--; srcX = 94; srcY = 42;}
                    break;
                case 15 : case 16 :
                    dstX += 16; dstY += 16; srcX = 68; srcY = 24;
                    if (force > 1 || onilink) {srcX = 94; srcY = 28;}
                    break;
                case 17 : case 18 :
                    dstX += 7; dstY += 21; srcX = 56; srcY = 12;
                    if (force > 1 || onilink) {srcX = 80; srcY = 14;}
                    break;
                case 19 : case 20 :
                    dstX--; dstY += 21; srcX = 68; srcY = 12;
                    if (force > 1 || onilink) {dstX--; srcX = 94; srcY = 14;}
                    break;
            }
            break;
        case S :
            switch (step) {
                case 0 : case 5 : case 21 : case 22 : case 23 : case 24 : case 25 : case 26 :
                    dstX += 13; dstY -= 2; srcX = 56; srcY = 0;
                    if (force > 1 || onilink) {dstY -= 2; srcX = 80; srcY = 0;}
                    break;
                case 1 : case 2 : case 3 : case 4 :
                    dstX += 7; dstY -= 6; srcX = 68; srcY = 0;
                    if (force > 1 || onilink) {dstX--; dstY -= 2; srcX = 94; srcY = 0;}
                    break;
                case 6 :
                    dstX += 16; dstY += 8; srcX = 68; srcY = 36;
                    if (force > 1 || onilink) {dstY--; srcX = 94; srcY = 42;}
                    break;
                case 7 : case 8 :
                    dstX += 16; dstY += 16; srcX = 68; srcY = 24;
                    if (force > 1 || onilink) {srcX = 94; srcY = 28;}
                    break;
                case 9 : case 10 :
                    dstX += 7; dstY += 21; srcX = 56; srcY = 12;
                    if (force > 1 || onilink) {srcX = 80; srcY = 14;}
                    break;
                case 11 : case 12 :
                    dstX--; dstY += 21; srcX = 68; srcY = 12;
                    if (force > 1 || onilink) {dstX--; srcX = 94; srcY = 14;}
                    break;
                case 13 : case 14 :
                    dstX -= 12; dstY += 15; srcX = 56; srcY = 24;
                    if (force > 1 || onilink) {dstX -= 2; srcX = 80; srcY = 28;}
                    break;
                case 15 : case 16 :
                    dstX -= 12; dstY += 8; srcX = 56; srcY = 36;
                    if (force > 1 || onilink) {dstX -= 2; dstY--; srcX = 80; srcY = 42;}
                    break;
                case 17 : case 18 :
                    dstX++; dstY -= 4; srcX = 68; srcY = 0;
                    if (force > 1 || onilink) {dstX--; dstY -= 2; srcX = 94;}
                    break;
                case 19 : case 20 :
                    dstX += 10; dstY -= 4; srcX = 56; srcY = 0;
                    if (force > 1 || onilink) {dstY -= 2; srcX = 80;}
                    break;
            }
            break;
        case W :
            switch (step) {
                case 0 : case 5 : case 21 : case 22 : case 23 : case 24 : case 25 : case 26 :
                    dstX += 11; dstY += 16; srcX = 68; srcY = 24;
                    if (force > 1 || onilink) {srcX = 94; srcY = 28;}
                    break;
                case 1 : case 2 : case 3 : case 4 :
                    dstX += 13; dstY += 9; srcX = 68; srcY = 36;
                    if (force > 1 || onilink) {srcX = 94; srcY = 42;}
                    break;
                case 6 :
                    dstX += 7; dstY += 21; srcX = 56; srcY = 12;
                    if (force > 1 || onilink) {srcX = 80; srcY = 14;}
                    break;
                case 7 : case 8 :
                    dstX--; dstY += 21; srcX = 68; srcY = 12;
                    if (force > 1 || onilink) {dstX--; srcX = 94; srcY = 14;}
                    break;
                case 9 : case 10 :
                    dstX -= 12; dstY += 15; srcX = 56; srcY = 24;
                    if (force > 1 || onilink) {dstX -= 2; srcX = 80; srcY = 28;}
                    break;
                case 11 : case 12 :
                    dstX -= 12; dstY += 8; srcX = 56; srcY = 36;
                    if (force > 1 || onilink) {dstX -= 2; dstY--; srcX = 80; srcY = 42;}
                    break;
                case 13 : case 14 :
                    dstX++; dstY -=4; srcX = 68; srcY = 0;
                    if (force > 1 || onilink) {dstX--; dstY -= 6; srcX = 94;}
                    break;
                case 15 : case 16 :
                    dstX += 10; dstY -= 4;srcX = 56; srcY = 0;
                    if (force > 1 || onilink) {dstY-=2; srcX = 80;}
                    break;
                case 17 : case 18 :
                    dstX += 16; dstY += 8; srcX = 68; srcY = 36;
                    if (force > 1 || onilink) {dstY--; srcX = 94; srcY = 42;}
                    break;
                case 19 : case 20 :
                    dstX += 16; dstY += 16; srcX = 68; srcY = 24;
                    if (force > 1 || onilink) {srcX = 94; srcY = 28;}
                    break;
            }
            break;
        case E :
            switch (step) {
                case 0 : case 5 : case 21 : case 22 : case 23 : case 24 : case 25 : case 26 :
                    dstX -= 8; dstY += 6; srcX = 56; srcY = 36;
                    if (force > 1 || onilink) {dstX -= 2; dstY--; srcX = 80; srcY = 42;}
                    break;
                case 1 : case 2 : case 3 : case 4 :
                    dstX -= 8; dstY += 12; srcX = 56; srcY = 24;
                    if (force > 1 || onilink) {dstX -= 2; srcX = 80; srcY = 28;}
                    break;
                case 6 :
                    dstX++; dstY -= 4; srcX = 68; srcY = 0;
                    if (force > 1 || onilink) {dstX--; dstY -= 2; srcX = 94; srcY = 0;}
                    break;
                case 7 : case 8 :
                    dstX += 10; dstY -= 4; srcX = 56; srcY = 0;
                    if (force > 1 || onilink) {dstY -= 2; srcX = 80; srcY = 0;}
                    break;
                case 9 : case 10 :
                    dstX += 16; dstY += 8;srcX = 68; srcY = 36;
                    if (force > 1 || onilink) {dstY--; srcX = 94; srcY = 42;}
                    break;
                case 11 : case 12 :
                    dstX += 16; dstY += 16;srcX = 68; srcY = 24;
                    if (force > 1 || onilink) {srcX = 94; srcY = 28;}
                    break;
                case 13 : case 14 :
                    dstX += 7; dstY += 21; srcX = 56; srcY = 12;
                    if (force > 1 || onilink) {srcX = 80; srcY = 14;}
                    break;
                case 15 : case 16 :
                    dstX--; dstY += 21;srcX = 68; srcY = 12;
                    if (force > 1 || onilink) {dstX--; srcX = 94; srcY = 14;}
                    break;
                case 17 : case 18 :
                    dstX -= 12; dstY += 15;srcX = 56; srcY = 24;
                    if (force > 1 || onilink) {dstX -= 2; srcX = 80; srcY = 28;}
                    break;
                case 19 : case 20 :
                    dstX -= 12; dstY += 8;srcX = 56; srcY = 36;
                    if (force > 1 || onilink) {dstX -=2 ; dstY--; srcX = 80; srcY = 42;}
                    break;
            }


            break;
    }

    if (step < 27) {
        WindowManager::getInstance()->draw(image, srcX, srcY, srcW, srcW, dstX, dstY);
    }

    // light
    int srcH = 0;
    dstX = dstX0;
    dstY = dstY0;
    switch (step) {
        case 0 :
            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 7; dstY -= 8;}
            if (dir==S) { dstX += 6; dstY += 30;}
            if (dir==W) { dstX -= 12; dstY += 17;}
            if (dir==E) { dstX += 23; dstY += 15;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 1 :
            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 9; dstY -= 6; }
            if (dir==S) { dstX += 8; dstY += 32; }
            if (dir==W) { dstX -= 10; dstY += 19; }
            if (dir==E) { dstX += 25; dstY += 17; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 0; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 3; dstY += 35; }
            if (dir==S) { dstX -= 5; dstY -= 44; }
            if (dir==W) { dstX += 32; dstY -= 13; }
            if (dir==E) { dstX -= 38; dstY -= 1; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 2 :
            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 9; dstY -= 6;}
            if (dir==S) { dstX += 8; dstY += 32;}
            if (dir==W) { dstX -= 10; dstY += 19;}
            if (dir==E) { dstX += 25; dstY += 17;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 8; srcY = 0; srcW = 14; srcH = 14;
            if (dir==N) { dstX -= 6; dstY += 32; }
            if (dir==S) { dstX -= 8; dstY -= 47; }
            if (dir==W) { dstX += 29; dstY -= 16; }
            if (dir==E) { dstX -= 41; dstY -= 4; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 3 :
            srcX = 8; srcY = 0; srcW = 14; srcH = 14;
            if (dir==N) { dstX += 3; dstY += 26; }
            if (dir==S) { dstY -= 15; }
            if (dir==W) { dstX += 19; dstY += 3; }
            if (dir==E) { dstX -= 15; dstY += 13; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 4 :
        case 5 :
            srcX = 22; srcY = 0; srcW = 14; srcH = 14;
            if (dir==N) { dstX += 3; dstY += 26; }
            if (dir==S) { dstY -= 15; }
            if (dir==W) { dstX += 19; dstY += 3; }
            if (dir==E) { dstX -= 15; dstY += 13; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 6 :
            srcX = 36; srcY = 0; srcW = 16; srcH = 16;
            if (dir==N) { dstX += 2; dstY += 25; }
            if (dir==S) { dstX -= 1; dstY -= 15; }
            if (dir==W) { dstX += 19; dstY += 2; }
            if (dir==E) { dstX -= 17; dstY += 12; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 7 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 5; dstY += 27; }
            if (dir==S) { dstX += 17; dstY -= 6; }
            if (dir==W) { dstX += 20; dstY += 19; }
            if (dir==E) { dstX -= 13; dstY -= 1; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 8 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 11; dstY += 22; }
            if (dir==S) { dstX += 22; }
            if (dir==W) { dstX += 14; dstY += 25; }
            if (dir==E) { dstX -= 8; dstY -= 7; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 7; dstY += 6;}
            if (dir==S) { dstX -= 7; dstY -= 7;}
            if (dir==W) { dstX += 7; dstY -= 7;}
            if (dir==E) { dstX -= 5; dstY += 8;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 9 :
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 14; dstY += 15; }
            if (dir==S) { dstX += 23; dstY += 7; }
            if (dir==W) { dstX += 8; dstY += 28; }
            if (dir==E) { dstX -= 2; dstY -= 10; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 5; dstY += 9; }
            if (dir==S) { dstX -= 3; dstY -= 8; }
            if (dir==W) { dstX += 9; dstY -= 4; }
            if (dir==E) { dstX -= 8; dstY += 6; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 9; dstY += 6;}
            if (dir==S) { dstX -= 7; dstY -= 6;}
            if (dir==W) { dstX += 6; dstY -= 6;}
            if (dir==E) { dstX -= 4; dstY += 9;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 10 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 15; dstY += 7; }
            if (dir==S) { dstX += 22; dstY += 15; }
            if (dir==W) { dstY += 28; }
            if (dir==E) { dstX += 6; dstY -= 11; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 11; dstY += 11;}
            if (dir==S) { dstX += 1; dstY -= 9;}
            if (dir==W) dstX += 10;
            if (dir==E) { dstX -= 10; dstY += 3;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 6; dstY += 8; }
            if (dir==S) { dstX -= 4; dstY -= 7; }
            if (dir==W) { dstX += 9; dstY -= 4; }
            if (dir==E) { dstX -= 6; dstY += 6; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 11; dstY += 6;}
            if (dir==S) { dstX -= 5; dstY -= 4;}
            if (dir==W) { dstX += 7; dstY -= 7;}
            if (dir==E) { dstX -= 3; dstY += 10;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 11 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 13; }
            if (dir==S) { dstX += 18; dstY += 22; }
            if (dir==W) { dstX -= 7; dstY += 25; }
            if (dir==E) { dstX += 13; dstY -= 9; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 2; dstY += 10;}
            if (dir==S) { dstX += 5; dstY -= 8;}
            if (dir==W) { dstX += 9; dstY += 4;}
            if (dir==E) { dstX -= 9; dstY -= 1;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 3; dstY += 10;}
            if (dir==S) { dstY -= 9;}
            if (dir==W) { dstX += 11; dstY -= 1;}
            if (dir==E) { dstX -= 8; dstY += 3;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 9; dstY += 9; }
            if (dir==S) { dstX -= 3; dstY -= 7; }
            if (dir==W) { dstX += 9; dstY -= 4; }
            if (dir==E) { dstX -= 6; dstY += 8; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 12 :
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 8; dstY -= 6; }
            if (dir==S) { dstX += 12; dstY += 27; }
            if (dir==W) { dstX -= 13; dstY += 19; }
            if (dir==E) { dstX += 19; dstY -= 4; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 5; dstY += 8;}
            if (dir==S) { dstX += 8; dstY -= 6;}
            if (dir==W) { dstX += 8; dstY += 8;}
            if (dir==E) { dstX -= 7; dstY += 4;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 1; dstY += 11;}
            if (dir==S) { dstX += 4; dstY -= 8;}
            if (dir==W) { dstX += 10; dstY += 3;}
            if (dir==E) { dstX -= 9; dstY -= 1;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 5; dstY += 10;}
            if (dir==S) { dstY -= 9;}
            if (dir==W) { dstX += 11; dstY -= 1;}
            if (dir==E) { dstX -= 7; dstY += 4;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 13 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            dir=dir;
            if (dir==N) { dstX -= 2; dstY -= 10;}
            if (dir==S) { dstX += 6; dstY += 29;}
            if (dir==W) { dstX -= 15; dstY += 12;}
            if (dir==E) { dstX += 22; dstY += 3;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 8; dstY += 6; }
            if (dir==S) { dstX += 9; dstY -= 2; }
            if (dir==W) { dstX += 5; dstY += 10; }
            if (dir==E) { dstX -= 5; dstY -= 7; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 4; dstY += 9;}
            if (dir==S) { dstX += 8; dstY -= 7;}
            if (dir==W) { dstX += 8; dstY += 6;}
            if (dir==E) { dstX -= 7; dstY -= 4;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 1; dstY += 11;}
            if (dir==S) { dstX += 4; dstY -= 8;}
            if (dir==W) { dstX += 12; dstY += 3;}
            if (dir==E) dstX -= 8;
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 14 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            dir=dir;
            if (dir==N) { dstX += 6; dstY -= 11;}
            if (dir==S) { dstX -= 2; dstY += 28;}
            if (dir==W) { dstX -= 15; dstY += 4;}
            if (dir==E) { dstX += 23; dstY += 11;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 10; dstY += 3;}
            if (dir==S) { dstX += 10; dstY += 2;}
            if (dir==W) { dstX += 1; dstY += 11;}
            if (dir==E) { dstX -= 2; dstY -= 9;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 6; dstY += 6; }
            if (dir==S) { dstX += 10; dstY -= 4; }
            if (dir==W) { dstX += 6; dstY += 9; }
            if (dir==E) { dstX -= 4; dstY += 6; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX -= 3; dstY += 10;}
            if (dir==S) { dstX += 7; dstY -= 6;}
            if (dir==W) { dstX += 10; dstY += 6;}
            if (dir==E) { dstX -= 7; dstY -= 3;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 15 :
            dir=dir;
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            if (dir==N) { dstX += 13; dstY -= 9;}
            if (dir==S) { dstX -= 9; dstY += 24;}
            if (dir==W) { dstX -= 12; dstY -= 3;}
            if (dir==E) { dstX += 21; dstY += 18;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 9; dstY -= 1;}
            if (dir==S) { dstX += 9; dstY += 6;}
            if (dir==W) { dstX -= 3; dstY += 10;}
            if (dir==E) { dstX += 2; dstY -= 8;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 8; dstY += 3;}
            if (dir==S) dstX += 11;
            if (dir==W) { dstX += 3; dstY += 11;}
            if (dir==E) { dstX -= 1; dstY -= 8;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX -= 6; dstY += 8; }
            if (dir==S) { dstX += 10; dstY -= 4; }
            if (dir==W) { dstX += 7; dstY += 8; }
            if (dir==E) { dstX -= 5; dstY -= 6; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 18; dstY += 29;}
            if (dir==S) { dstX -= 17; dstY -= 36;}
            if (dir==W) { dstX += 30; dstY -= 19;}
            if (dir==E) { dstX -= 34; dstY += 19;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 16 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            dir=dir;
            if (dir==N) { dstX += 19; dstY -= 4;}
            if (dir==S) { dstX -= 14; dstY += 18;}
            if (dir==W) { dstX -= 6; dstY -= 9;}
            if (dir==E) { dstX += 16; dstY += 24;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 7; dstY -= 4;}
            if (dir==S) { dstX += 7; dstY += 9;}
            if (dir==W) { dstX -= 7; dstY += 9;}
            if (dir==E) { dstX += 5; dstY -= 6;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 9; dstY -= 1;}
            if (dir==S) { dstX += 10; dstY += 4;}
            if (dir==W) { dstX -= 1; dstY += 10;}
            if (dir==E) { dstX += 3; dstY -= 9;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX -= 7; dstY += 4;}
            if (dir==S) { dstX += 12; dstY -= 1;}
            if (dir==W) { dstX += 4; dstY += 10;}
            if (dir==E) { dstX -= 1; dstY -= 8;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 13; dstY += 36;}
            if (dir==S) { dstX -= 10; dstY -= 39;}
            if (dir==W) { dstX += 36; dstY -= 12;}
            if (dir==E) { dstX -= 39; dstY += 14;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 17 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            dir=dir;
            if (dir==N) { dstX += 22; dstY += 3;}
            if (dir==S) { dstX -= 15; dstY += 11;}
            if (dir==W) dstY -= 11;
            if (dir==E) { dstX += 10; dstY += 28;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 5; dstY -= 7;}
            if (dir==S) { dstX += 3; dstY += 10;}
            if (dir==W) { dstX -= 9; dstY += 6;}
            if (dir==E) { dstX += 8; dstY -= 4;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 7; dstY -= 4;}
            if (dir==S) { dstX += 9; dstY += 8;}
            if (dir==W) { dstX -= 4; dstY += 8;}
            if (dir==E) { dstX += 6; dstY -= 7;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX -= 8; }
            if (dir==S) { dstX += 11; dstY += 3; }
            if (dir==W) { dstY += 11; }
            if (dir==E) { dstX += 3; dstY -= 9; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            if (dir==N) { dstX += 8; dstY += 40;}
            if (dir==S) dstY -= 40;
            if (dir==W) { dstX += 40; dstY -= 4;}
            if (dir==E) { dstX -= 40; dstY += 8;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 18 :
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            dir=dir;
            if (dir==N) { dstX += 23; dstY += 11;}
            if (dir==S) { dstX -= 14; dstY += 3;}
            if (dir==W) { dstX += 8; dstY -= 11;}
            if (dir==E) { dstX += 2; dstY += 29;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 2; dstY -= 9;}
            if (dir==S) { dstX -= 1; dstY += 11;}
            if (dir==W) { dstX -= 10; dstY += 2;}
            if (dir==E) { dstX += 10; dstY -= 1;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 4; dstY -= 6; }
            if (dir==S) { dstX += 6; dstY += 10; }
            if (dir==W) { dstX -= 7; dstY += 6; }
            if (dir==E) { dstX += 8; dstY -= 4; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX -= 7; dstY -= 4;}
            if (dir==S) { dstX += 9; dstY += 6;}
            if (dir==W) { dstX -= 3; dstY += 9;}
            if (dir==E) { dstX += 7; dstY -= 8;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 19 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            dir=dir;
            if (dir==N) { dstX += 21; dstY += 18;}
            if (dir==S) { dstX -= 10; dstY -= 4;}
            if (dir==W) { dstX += 15; dstY -= 8;}
            if (dir==E) { dstX -= 5; dstY += 27;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 2; dstY -= 8; }
            if (dir==S) { dstX -= 5; dstY += 10; }
            if (dir==W) { dstX -= 9; dstY -= 2; }
            if (dir==E) { dstX += 9; dstY += 3; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 1; dstY -= 8;}
            if (dir==S) { dstX += 2; dstY += 11;}
            if (dir==W) { dstX -= 9; dstY += 3;}
            if (dir==E) { dstX += 10; dstY -= 1;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX -= 5; dstY -= 7;}
            if (dir==S) { dstX += 7; dstY += 9;}
            if (dir==W) { dstX -= 5; dstY += 6;}
            if (dir==E) { dstX += 10; dstY -= 6;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 27; dstY += 30;}
            if (dir==S) { dstX += 26; dstY -= 29;}
            if (dir==W) { dstX += 26; dstY += 25;}
            if (dir==E) { dstX -= 35; dstY -= 26;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 20 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dir==N) { dstX += 16; dstY += 24; }
            if (dir==S) { dstX -= 4; dstY -= 9; }
            if (dir==W) { dstX += 21; dstY -= 2; }
            if (dir==E) { dstX -= 11; dstY += 22; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 5; dstY -= 6;}
            if (dir==S) { dstX -= 8; dstY += 8;}
            if (dir==W) { dstX -= 8; dstY -= 6;}
            if (dir==E) { dstX += 7; dstY += 6;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 3; dstY -= 9;}
            if (dir==S) { dstX -= 2; dstY += 10;}
            if (dir==W) { dstX -= 8; dstY -= 1;}
            if (dir==E) { dstX += 11; dstY += 3;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX -= 1; dstY -= 8;}
            if (dir==S) { dstX += 4; dstY += 11;}
            if (dir==W) { dstX -= 7; dstY += 3;}
            if (dir==E) { dstX += 11; dstY -= 2;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 32; dstY += 25;}
            if (dir==S) { dstX += 31; dstY -= 22;}
            if (dir==W) { dstX += 21; dstY += 31;}
            if (dir==E) { dstX -= 28; dstY -= 31;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 21 :
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            if (dir==N) { dstX += 10; dstY += 28; }
            if (dir==S) { dstX += 2; dstY -= 12; }
            if (dir==W) { dstX += 23; dstY += 4; }
            if (dir==E) { dstX -= 14; dstY += 14; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 8; dstY -= 4; }
            if (dir==S) { dstX -= 9; dstY += 4; }
            if (dir==W) { dstX -= 5; dstY -= 8; }
            if (dir==E) { dstX += 5; dstY += 9; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 6; dstY -= 7;}
            if (dir==S) { dstX -= 6; dstY += 9;}
            if (dir==W) { dstX -= 6; dstY -= 4;}
            if (dir==E) { dstX += 9; dstY += 6;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 3; dstY -= 9;}
            if (dir==S) dstY += 10;
            if (dir==W) { dstX -= 8; dstY -= 1;}
            if (dir==E) { dstX += 12; dstY += 2;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            if (dir==N) { dstX -= 33; dstY += 19;}
            if (dir==S) { dstX += 35; dstY -= 13;}
            if (dir==W) { dstX += 16; dstY += 34;}
            if (dir==E) { dstX -= 19; dstY -= 33;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 22 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dir==N) { dstX += 2; dstY += 29; }
            if (dir==S) { dstX += 10; dstY -= 11; }
            if (dir==W) { dstX += 23; dstY += 12; }
            if (dir==E) { dstX -= 15; dstY += 6; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 10; dstY -= 1;}
            if (dir==S) dstX -= 10;
            if (dir==W) { dstX -= 1; dstY -= 9;}
            if (dir==E) { dstX += 2; dstY += 11;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 8; dstY -= 4; }
            if (dir==S) { dstX -= 8; dstY += 6; }
            if (dir==W) { dstX -= 4; dstY -= 7; }
            if (dir==E) { dstX += 6; dstY += 8; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 7; dstY -= 8;}
            if (dir==S) { dstX -= 3; dstY += 8;}
            if (dir==W) { dstX -= 6; dstY -= 4;}
            if (dir==E) { dstX += 11; dstY += 6;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 23 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 5; dstY += 27; }
            if (dir==S) { dstX += 17; dstY -= 7; }
            if (dir==W) { dstX += 20; dstY += 19; }
            if (dir==E) { dstX -= 13; dstY -= 1; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 9; dstY += 3;}
            if (dir==S) { dstX -= 9; dstY -= 4;}
            if (dir==W) { dstX += 3; dstY -= 8;}
            if (dir==E) { dstX -= 2; dstY += 10;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 10; dstY -= 1;}
            if (dir==S) { dstX -= 9; dstY += 2;}
            if (dir==W) { dstX -= 1; dstY -= 9;}
            if (dir==E) { dstX += 3; dstY += 10;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 10; dstY -= 6; }
            if (dir==S) { dstX -= 6; dstY += 6; }
            if (dir==W) { dstX -= 3; dstY -= 6; }
            if (dir==E) { dstX += 9; dstY += 9; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 40; dstY -= 13;}
            if (dir==S) { dstX += 31; dstY += 16;}
            if (dir==W) { dstX -= 18; dstY += 33;}
            if (dir==E) { dstX += 8; dstY -= 39;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 24 :
            srcX = 10; srcY = 14; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 11; dstY += 22; }
            if (dir==S) { dstX += 22; dstY -= 1; }
            if (dir==W) { dstX += 14; dstY += 25; }
            if (dir==E) { dstX -= 8; dstY -= 7; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 7; dstY += 6;}
            if (dir==S) { dstX -= 7; dstY -= 7;}
            if (dir==W) { dstX += 7; dstY -= 7;}
            if (dir==E) { dstX -= 5; dstY += 8;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 11; dstY += 3;}
            if (dir==S) { dstX -= 8; dstY -= 2;}
            if (dir==W) { dstX += 3; dstY -= 8;}
            if (dir==E) { dstX -= 1; dstY += 11;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 11; dstY -= 2;}
            if (dir==S) { dstX -= 8; dstY += 3;}
            if (dir==W) dstY -= 8;
            if (dir==E) { dstX += 5; dstY += 10;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 33; dstY -= 18;}
            if (dir==S) { dstX += 26; dstY += 21;}
            if (dir==W) { dstX -= 22; dstY += 28;}
            if (dir==E) { dstX += 15; dstY -= 32;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 25 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 15; dstY += 15; }
            if (dir==S) { dstX += 24; dstY += 6; }
            if (dir==W) { dstX += 7; dstY += 28; }
            if (dir==E) { dstX -= 6; dstY -= 9; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 5; dstY += 9; }
            if (dir==S) { dstX -= 3; dstY -= 8; }
            if (dir==W) { dstX += 9; dstY -= 4; }
            if (dir==E) { dstX -= 7; dstY += 9; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 9; dstY += 6;}
            if (dir==S) { dstX -= 7; dstY -= 6;}
            if (dir==W) { dstX += 6; dstY -= 6;}
            if (dir==E) { dstX -= 1; dstY += 10;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 12; dstY += 2;}
            if (dir==S) { dstX -= 7; dstY -= 1;}
            if (dir==W) { dstX += 4; dstY -= 9;}
            if (dir==E) { dstX += 4; dstY += 10;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            if (dir==N) { dstX -= 24; dstY -= 20;}
            if (dir==S) { dstX += 20; dstY += 24;}
            if (dir==W) { dstX -= 22; dstY += 22;}
            if (dir==E) { dstX += 18; dstY -= 28;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 26 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 16; dstY += 7; }
            if (dir==S) { dstX += 23; dstY += 14; }
            if (dir==W) { dstX -= 1; dstY += 28; }
            if (dir==E) { dstX += 7; dstY -= 12; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 25; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX += 2; dstY += 11;}
            if (dir==S) { dstX += 1; dstY -= 9;}
            if (dir==W) dstX += 10;
            if (dir==E) { dstX -= 10; dstY += 3;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 6; dstY += 8; }
            if (dir==S) { dstX -= 4; dstY -= 8; }
            if (dir==W) { dstX += 9; dstY -= 4; }
            if (dir==E) { dstX -= 6; dstY += 6; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 11; dstY += 6;}
            if (dir==S) { dstX -= 5; dstY -= 4;}
            if (dir==W) { dstX += 7; dstY -= 7;}
            if (dir==E) { dstX -= 3; dstY += 10;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 27 :
            srcX = 10; srcY = 14; srcW = 8; srcH += 8;
            if (dir==N) { dstX -= 14; }
            if (dir==S) { dstX += 19; dstY += 22; }
            if (dir==W) { dstX -= 8; dstY += 25; }
            if (dir==E) { dstX += 14; dstY -= 10; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 2; dstY += 10;}
            if (dir==S) { dstX += 5; dstY -= 8;}
            if (dir==W) { dstX += 9; dstY += 4;}
            if (dir==E) { dstX -= 9; dstY -= 1;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 5; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX += 3; dstY += 10;}
            if (dir==S) dstY -= 9;
            if (dir==W) { dstX += 11; dstY -= 1;}
            if (dir==E) { dstX -= 8; dstY += 3;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 33; srcY = 15; srcW = 1; srcH = 1;
            if (dir==N) { dstX += 9; dstY += 9; }
            if (dir==S) { dstX -= 3; dstY -= 7; }
            if (dir==W) { dstX += 9; dstY -= 4; }
            if (dir==E) { dstX -= 6; dstY += 8; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);

            srcX = 18; srcY = 14; srcW = 7; srcH = 7;
            if (dir==N) { dstX -= 5; dstY -= 34;}
            if (dir==S) { dstX -= 8; dstY += 30;}
            if (dir==W) { dstX -= 35; dstY -= 4;}
            if (dir==E) { dstX += 29; dstY += 4;}
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 28 :
            srcX = 52; srcY = 0; srcW = 14; srcH = 14;
            if (dir==N) { dstX -= 10; dstY += 22; }
            if (dir==S) { dstX += 14; dstY -= 8; }
            if (dir==W) { dstX += 17; dstY += 17; }
            if (dir==E) { dstX -= 14; dstY -= 1; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 29 :
            srcX = 0; srcY = 8; srcW = 8; srcH = 8;
            if (dir==N) { dstX -= 7; dstY += 25; }
            if (dir==S) { dstX += 17; dstY -= 5; }
            if (dir==W) { dstX += 20; dstY += 20; }
            if (dir==E) { dstX -= 11; dstY += 2; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
        case 30 :
            srcX = 0; srcY = 16; srcW = 5; srcH = 5;
            if (dir==N) { dstX -= 6; dstY += 27; }
            if (dir==S) { dstX += 18; dstY -= 3; }
            if (dir==W) { dstX += 21; dstY += 22; }
            if (dir==E) { dstX -= 10; dstY += 4; }
            WindowManager::getInstance()->draw(effects, srcX, srcY + 16, srcW, srcW, dstX, dstY);
            break;
    }

}

void Sword::drawCharge(int dstX, int dstY, Direction dir, bool move, int step, int charge) {

    int srcW = (force == 1 && !onilink) ? 14 : 20;
    int srcH = srcW;
    int srcX = srcW * dir;
    int srcY = srcH * 5;

    switch (dir) {
        case N : dstX += 4; srcW = 8; srcH = 12; dstY -= 2;
            if (move && step) dstY++;
            if (force > 1 || onilink) {dstX -= 2; dstY -= 2; srcH = 15;} break;
        case S : dstX += 8; srcW = 8; srcH = 12; dstY += 19;
            if (move && step) dstY++;
            if (force > 1 || onilink) {dstX--; srcH = 14;} break;
        case W : dstX -= 8; srcW = 12; srcH = 8; dstY += 14;
            if (move && step) dstY++;
            if (force > 1 || onilink) {dstX -= 3; srcW = 15;} break;
        case E : dstX += 12; srcW = 12; srcH = 8; dstY += 14;
            if (move && step) dstY++;
            if (force > 1 || onilink) srcW = 15; break;
    }
    WindowManager::getInstance()->draw(image, srcX, srcY, srcW, srcH, dstX, dstY);


    // Star
    if (charge < 19) {
        srcX = 18; srcY = 30; srcW = 7; srcH = 7; dstX += 2;
        switch (dir) {
            case N : dstX += (charge / 3) - 6 + ((charge + 2) % 3); dstY -= (charge / 2) - 5; break;
            case S : dstX -= (charge / 3) - 2 + ((charge + 2) % 3); dstY += (charge / 2); break;
            case W : dstY += ((charge + 2) % 3) - 1; dstX -= (charge / 2) - 5; break;
            case E : dstY += ((charge + 2) % 3) - 1; dstX += (charge / 2); break;
        }
        WindowManager::getInstance()->draw(effects, srcX, srcY, srcW, srcH, dstX, dstY);
        srcX = 5; srcY = 32; srcW = 5; srcH = 5;
        switch (dir) {
            case N : dstX -= (charge % 3) - 1; dstY += 4; break;
            case S : dstX += (charge % 3) - 1; dstY -= 2; break;
            case W : dstY += (charge % 3) - 0; dstX += 4; break;
            case E : dstY += (charge % 3) - 0; dstX -= 2; break;
        }
        WindowManager::getInstance()->draw(effects, srcX, srcY, srcW, srcH, dstX, dstY);
    } else if (charge < 21) {
        srcX = 0; srcY = 24; srcW = 8; srcH = 8;
        switch (dir) {
            case N : dstX -= 2; dstY -= 5; if (force > 1 || onilink) dstX += 4; break;
            case S : dstX -= 4; dstY += 9; break;
            case W : dstX -= 5; dstY -= 1; break;
            case E : dstX += 12; dstY -= 1; break;
        }
        WindowManager::getInstance()->draw(effects, srcX, srcY, srcW, srcH, dstX, dstY);
    }
}
