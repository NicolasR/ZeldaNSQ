#include "WImage.h"

WImage::WImage(string filename, bool alpha) {
    name = filename;
    image = IMG_Load(name.c_str());
    if (alpha) {
        SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 0, 0, 255));
    }
}

WImage::WImage(int w, int h, bool alpha) {
    if (alpha) {
        image = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0, 0, 0, 0);
        SDL_FillRect(image, NULL, SDL_MapRGB(image->format, 0, 0, 255));
        SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 0, 0, 255));
    } else {
        image = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0, 0, 0, 0);
    }
}

WImage::~WImage() {
    SDL_FreeSurface(image);
}

SDL_Surface* WImage::getImage() {
    return image;
}

void WImage::setAlpha(int alpha) {
    SDL_SetAlpha(image, SDL_SRCALPHA, alpha);
}
