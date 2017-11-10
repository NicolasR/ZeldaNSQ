#include "Shield.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"

Shield::Shield(int i, bool ol) : level(i), onilink(ol) {
    if (onilink) {
        image = ResourceManager::getInstance()->loadImage("data/images/link/bouclierOni.png", true);
    } else {
        ostringstream os;
        os << i;
        string filename = "data/images/link/bouclier" + os.str() + ".png";
        image = ResourceManager::getInstance()->loadImage(filename, true);
    }
}

Shield::~Shield() {
    ResourceManager::getInstance()->free(image);
}

int Shield::getLevel() {
    return level;
}

void Shield::draw(int offsetX, int offsetY, Direction dir, Animation anim, int step, int charge) {

    int dstX = offsetX;
    int dstY = offsetY;


    //IDLE, WALK, PUSH, TOUCHE, FLOTTE, NAGE, COULE, HIT_EPEE, NOYE, MORT
    switch (anim) {
        case IDLE :
        case WALK :
            if (charge) {
                switch (dir) {
                    case N : dstX += 15; dstY += 9; dir = E; break;
                    case S : dstX -= 3; dstY += 10; dir = W; break;
                    default : return;
                }
                break;
            }
            switch (dir) {
                case N :
                    dstX += 9; dstY += 9;
                    if (anim == WALK) {
                        if (step == 0 || step == 4) dstY--;
                        if (step == 1 || step == 6) dstY -= 2;
                    }
                    break;
                case S :
                    dstY += 14;
                    if (anim == WALK) {
                        if (step == 1 || step == 5) dstY--;
                        if (step == 2 || step == 6) dstY -= 3;
                        if (step == 3 || step == 7) dstY -= 2;
                    }
                    break;
                case W :
                    dstX -= 3; dstY += 8;
                    if (anim == WALK) {
                        if (step == 0 || step == 2 || step == 4 || step == 6) dstY--;
                        if (step == 1 || step == 5) dstY -= 2;
                    }
                    break;
                case E :
                    dstX += 15; dstY += 8;
                    if (anim == WALK) {
                        if (step == 0 || step == 2 || step == 4 || step == 6) dstY--;
                        if (step == 1 || step == 5) dstY -= 2;
                    }
                    break;
            }
            break;
        case HIT_EPEE :
            switch (dir) {
                case N :
                    dstX += 16; dstY += 12;
                    if (step == 1 || step == 4) dstY -= 2;
                    if (step == 2) dstY -= 4;
                    if (step == 3) dstY -= 3;
                    dir = E;
                    break;
                case S :
                    dstX -= 3; dstY += 4;
                    if (step == 1) {
                        dstY+=3;
                    }
                    else if (step > 1) {
                        dstY+=5; dstX--;
                    }
                    dir = W;
                    break;
                default : return;
            }
            break;
        case TIR_GRAPPIN :
        case TIR_LANTERNE :
            switch (dir) {
                case N : dstX += 16; dstY += 9; dir = E; break;
                case S : dstX -= 4; dstY += 10; dir = W; break;
                default : return;
            }
            break;
        case TIR_BOOMERANG :
            switch (dir) {
                case N :
                    if (step == 0) {
                        dstX += 9; dstY += 10;
                    } else {
                        dstX += 16; dstY += 9; dir = E;
                    }
                    break;
                case S :
                    if (step == 0) {
                        dstY += 14;
                    } else {
                        dstX -= 4; dstY += 10; dir = W;
                    }
                    break;
                case W :
                    if (step == 0) {
                        dstX -= 2; dstY += 7;
                    } else {
                        return;
                    }
                    break;
                case E :
                    if (step == 0) {
                        dstX += 14; dstY += 7;
                    } else {
                        return;
                    }
                    break;
                default : return;
            }
            break;
        case TIR_BAGUETTE_FEU :
        case TIR_BAGUETTE_GLACE :
        case TIR_MARTEAU :
            switch (dir) {
                case N :
                    dstX += 15; dstY += 10; dir = E;
                    if (step != 0) dstY--;
                    break;
                case S : dstX -= 4; dstY += 10; dir = W; break;
                default : return;
            }
            break;
        default : return;
    }

    draw(dstX, dstY, dir);
}

void Shield::draw(int dstX, int dstY, Direction dir) {

    int srcX;
    switch (dir) {
        case N : srcX = 0; break;
        case S :
            if (onilink) {srcX = 10; break;}
            switch (level) {
                case 1 : srcX = 8; break;
                case 2 : srcX = 11; break;
                case 3 : srcX = 13; break;
                case 4 : srcX = 13; break;
                default : return;
            }
            break;
        case W :
            if (onilink) {srcX = 20; break;}
            switch (level) {
                case 1 : srcX = 16; break;
                case 2 : srcX = 22; break;
                case 3 : srcX = 26; break;
                case 4 : srcX = 26; break;
                default : return;
            }
            break;
        case E :
            if (onilink) {srcX = 23; break;}
            switch (level) {
                case 1 : srcX = 20; break;
                case 2 : srcX = 26; break;
                case 3 : srcX = 30; break;
                case 4 : srcX = 30; break;
                default : return;
            }
            break;
    }

    int srcY = 0;

    int srcW;
    switch (dir) {
        case N :
        case S :
            if (onilink) {srcW = 10; break;}
            switch (level) {
                case 1 : srcW = 8; break;
                case 2 : srcW = 11; break;
                case 3 : srcW = 13; break;
                case 4 : srcW = 13; break;
                default : return;
            }
            break;
        case W :
        case E :
            if (onilink) {srcW = 3; break;}
            srcW = 4; break;
    }

    int srcH;
    if (onilink) {
        srcH = 11;
    } else {
        switch (level) {
            case 1 : srcH = 10; break;
            case 2 : srcH = 12; break;
            case 3 : srcH = 15; break;
            case 4 : srcH = 15; break;
            default : return;
        }
    }

    switch (dir) {
        case N :
            if (onilink) {dstX -= 1; dstY -= 1; break;}
            switch (level) {
                case 2 : dstX -= 2; dstY -= 2; break;
                case 3 : dstX -= 2; dstY -= 4; break;
                case 4 : dstX -= 2; dstY -= 4; break;
            }
            break;
        case S :
            if (onilink) {dstX -= 1; dstY -= 1; break;}
            switch (level) {
                case 2 : dstX -= 2; dstY -= 2; break;
                case 3 : dstX -= 3; dstY -= 4; break;
                case 4 : dstX -= 3; dstY -= 4; break;
            }
            break;
        case W :
            if (onilink) {dstX += 1; dstY -= 1; break;}
        case E :
            if (onilink) {dstY -= 1; break;}
            switch (level) {
                case 3 : dstY --; break;
                case 4 : dstY --; break;
            }
            break;
    }

    WindowManager::getInstance()->draw(image, srcX, srcY, srcW, srcH, dstX, dstY);
}
