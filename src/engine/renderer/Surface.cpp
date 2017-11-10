#include "Surface.h"

Surface::Surface(int w, int h, bool alpha) {
    image = new WImage(w, h, alpha);
}

Surface::~Surface() {
    delete image;
}

void Surface::put(WImage* im, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH) {
    SDL_Surface* tmp = im->getImage();

    SDL_Rect src;
    src.x = srcX;
    src.y = srcY;
    src.w = srcW;
    src.h = srcH;

    SDL_Rect dst;
    dst.x = dstX;
    dst.y = dstY;

    SDL_BlitSurface(tmp, &src, image->getImage(), &dst);
}

WImage* Surface::toImage() {
    return image;
}
