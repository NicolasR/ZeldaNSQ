#include "PiegeFeu.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../MainController.h"

#include "../helper/ProjectileHelper.h"

PiegeFeu::PiegeFeu(int i, int j) : anim(0), animMax(16), vanim(120) {
    x = i;
    y = j;

    image = ResourceManager::getInstance()->loadImage("data/images/objects/piegeFeu.png", true);

    // for quadtree operations:
    width = 16;
    height = 16;

    box.setX(x);
    box.setY(y);
    box.setW(width);
    box.setH(height);

    chrono.reset();
}

PiegeFeu::~PiegeFeu() {
    ResourceManager::getInstance()->free(image);
}

void PiegeFeu::loop() {
    if (chrono.getElapsedTime() >= vanim) {
        anim++;
        if (anim > animMax) {
            anim = 0;
            snipe();
        }
        chrono.reset();
    }
}

void PiegeFeu::draw(int offsetX, int offsetY) {
    WindowManager::getInstance()->draw(image, 0, 0, 16, 16, x - offsetX, y - offsetY);
}

void PiegeFeu::snipe() {

    // throw proj and play sound
    Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();

    int dstX = link->getX() + 8;
    int dstY = link->getY() + 24;

    double anglx = 0;
    double angly = 0;
    int origx = x + 8;
    int origy = y + 8;
    int destx = dstX;
    int desty = dstY - 8;

    double coef1 = 0;
    double coef2 = 0;

    if ((destx-origx) == 0) {anglx=0; angly=12;}
    else if ((desty-origy) == 0) {anglx=12; angly=0;}
    else {
        coef1=((double)(desty-origy))/((double)(destx-origx));
        coef2=((double)(destx-origx))/((double)(desty-origy));
        anglx=(sqrt(12/(1+(coef1*coef1))));
        angly=(sqrt(12/(1+(coef2*coef2))));
    }
    if (destx - origx < 0) anglx = -anglx;
    if (desty - origy < 0) angly = -angly;

    if (anglx>4) anglx=4;
    if (angly>4) angly=4;
    if (anglx<-4) anglx=-4;
    if (angly<-4) angly=-4;

    ProjectileHelper::getInstance()->addProjectile(TP_BOULE_FEU, origx, origy, anglx, angly, getBoundingBox());
    AudioManager::getInstance()->playSound(TS_THROW);
}
