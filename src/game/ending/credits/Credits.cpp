#include "Credits.h"

#include "../../MainController.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/texts/TextManager.h"
#include "../../../engine/audio/AudioManager.h"

Credits::Credits() : over(false), step(0), anim(0), surface(0), surface2(0), image(0) {
}

Credits::~Credits() {
    delete surface;
    delete surface2;
    //ResourceManager::getInstance()->free(image);
}

void Credits::init() {
    delete surface;
    surface = new Surface(320, 240);

    image = ResourceManager::getInstance()->loadImage("data/images/ending/credits/fond.png");
    surface->put(image, 0, 0, 0, 0, 320, 240);
    ResourceManager::getInstance()->free(image);

    image = ResourceManager::getInstance()->loadImage("data/images/ending/credits/photo_1.png", true);
    image->setAlpha(0);

    over = false;
    step = 0;
    anim = 0;
}

void Credits::handleEvents(Event* event) {
}

void Credits::loop() {
    switch (step) {
        case 0 :
            anim++;
            image->setAlpha(anim);
            if (anim == 255) {
                surface->put(image, 0, 0, 0, 0, 320, 240);
                ResourceManager::getInstance()->free(image);
                image = 0;
                anim = 0;
                step = 1;
            }
            break;
        case 1 :
            anim++;
            if (anim == 1) {
                delete surface2;
                surface2 = new Surface(320, 240, true);
                Text* text = TextManager::getInstance()->getText(674);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 10, surface2);
                delete text;
            } else if (anim == 128) {
                Text* text = TextManager::getInstance()->getText(675);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 90, surface2);
                delete text;
            } else if (anim == 255) {
                delete surface2;
                surface2 = 0;
                image = ResourceManager::getInstance()->loadImage("data/images/ending/credits/photo_2.png", true);
                image->setAlpha(0);
                anim = 0;
                step = 2;
            }
            break;
        case 2 :
            anim++;
            image->setAlpha(anim);
            if (anim == 255) {
                surface->put(image, 0, 0, 0, 0, 320, 240);
                ResourceManager::getInstance()->free(image);
                image = 0;
                anim = 0;
                step = 3;
            }
            break;
        case 3 :
            anim++;
            if (anim == 1) {
                delete surface2;
                surface2 = new Surface(320, 240, true);
                Text* text = TextManager::getInstance()->getText(676);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 10, surface2);
                delete text;
            } else if (anim == 128) {
                Text* text = TextManager::getInstance()->getText(677);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 60, surface2);
                delete text;
            } else if (anim == 255) {
                Text* text = TextManager::getInstance()->getText(678);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 130, surface2);
                delete text;
            } else if (anim == 384) {
                Text* text = TextManager::getInstance()->getText(679);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 180, surface2);
                delete text;
            } else if (anim == 512) {
                delete surface2;
                surface2 = 0;
                image = ResourceManager::getInstance()->loadImage("data/images/ending/credits/photo_3.png", true);
                image->setAlpha(0);
                anim = 0;
                step = 4;
            }
            break;
        case 4 :
            anim++;
            image->setAlpha(anim);
            if (anim == 255) {
                surface->put(image, 0, 0, 0, 0, 320, 240);
                ResourceManager::getInstance()->free(image);
                image = 0;
                anim = 0;
                step = 5;
            }
            break;
        case 5 :
            anim++;
            if (anim == 1) {
                delete surface2;
                surface2 = new Surface(320, 240, true);
                Text* text = TextManager::getInstance()->getText(680);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 10, surface2);
                delete text;
            } else if (anim == 128) {
                Text* text = TextManager::getInstance()->getText(681);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 40, surface2);
                delete text;
            } else if (anim == 255) {
                Text* text = TextManager::getInstance()->getText(764);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 90, surface2);
                delete text;
            } else if (anim == 384) {
                Text* text = TextManager::getInstance()->getText(765);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 120, surface2);
                delete text;
            } else if (anim == 512) {
                Text* text = TextManager::getInstance()->getText(767);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 170, surface2);
                delete text;
            } else if (anim == 640) {
                Text* text = TextManager::getInstance()->getText(768);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 200, surface2);
                delete text;
            } else if (anim == 768) {
                delete surface2;
                surface2 = 0;
                image = ResourceManager::getInstance()->loadImage("data/images/ending/credits/photo_4.png", true);
                image->setAlpha(0);
                anim = 0;
                step = 6;
            }
            break;
        case 6 :
            anim++;
            image->setAlpha(anim);
            if (anim == 255) {
                surface->put(image, 0, 0, 0, 0, 320, 240);
                ResourceManager::getInstance()->free(image);
                image = 0;
                anim = 0;
                step = 7;
            }
            break;
        case 7 :
            anim++;
            if (anim == 1) {
                delete surface2;
                surface2 = new Surface(320, 240, true);
                Text* text = TextManager::getInstance()->getText(682);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 10, surface2);
                delete text;
            } else if (anim == 128) {
                Text* text = TextManager::getInstance()->getText(683);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 60, surface2);
                delete text;
            } else if (anim == 255) {
                Text* text = TextManager::getInstance()->getText(684);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 110, surface2);
                delete text;
            } else if (anim == 384) {
                delete surface2;
                surface2 = 0;
                image = ResourceManager::getInstance()->loadImage("data/images/ending/credits/photo_5.png", true);
                image->setAlpha(0);
                anim = 0;
                step = 8;
            }
            break;
        case 8 :
            anim++;
            image->setAlpha(anim);
            if (anim == 255) {
                surface->put(image, 0, 0, 0, 0, 320, 240);
                ResourceManager::getInstance()->free(image);
                image = 0;
                anim = 0;
                step = 9;
            }
            break;
        case 9 :
            anim++;
            if (anim == 1) {
                delete surface2;
                surface2 = new Surface(320, 240, true);
                Text* text = TextManager::getInstance()->getText(685);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 10, surface2);
                delete text;
            } else if (anim == 128) {
                Text* text = TextManager::getInstance()->getText(686);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 60, surface2);
                delete text;
            } else if (anim == 255) {
                Text* text = TextManager::getInstance()->getText(687);
                int size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 110, surface2);
                delete text;
                text = TextManager::getInstance()->getText(688);
                size = text->getLength() * TextManager::getInstance()->getWSpace();
                text->displayOnSurface(160 - (size / 2), 130, surface2);
                delete text;
            } else if (anim == 384*2) {
                delete surface2;
                surface2 = 0;
                anim = 0;
                step = 10;
                over = true;
            }
            break;
        case 10 :
            anim++;
            if (anim == 255) {
                MainController::getInstance()->getEndingController()->setStep(END_END);
            }
        default : break;
    }
}

void Credits::draw() {
    WindowManager::getInstance()->draw(surface->toImage(), 0, 0, 320, 240, 0, 0);

    if (image != 0) WindowManager::getInstance()->draw(image, 0, 0, 320, 240, 0, 0);

    if (surface2 != 0) WindowManager::getInstance()->draw(surface2->toImage(), 0, 0, 320, 240, 0, 0);
}
