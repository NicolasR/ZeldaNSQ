#include "TextConfiguration.h"

TextConfiguration::TextConfiguration() : 
    wSpace(0), 
    wSize(0), 
    hSize(0), 
#ifdef __vita__
    psButtonsSpace(0),
#endif 
    image(0), 
    textfile("") {
}

TextConfiguration::~TextConfiguration() {
}

int TextConfiguration::getWSpace() {
    return wSpace;
}

int TextConfiguration::getWSize() {
    return wSize;
}

int TextConfiguration::getHSize() {
    return hSize;
}

 string TextConfiguration::getTextfile() {
     return textfile;
 }

WImage* TextConfiguration::getImage() {
    return image;
}

void TextConfiguration::setWSpace(int i) {
    wSpace = i;
}

void TextConfiguration::setWSize(int i) {
    wSize = i;
}

void TextConfiguration::setHSize(int i) {
    hSize = i;
}

void TextConfiguration::setTextfile(string s) {
#ifdef __vita__
    textfile = "app0:" + s;
#else
    textfile = s;
#endif
}

void TextConfiguration::setImage(WImage* im) {
    image = im;
}

#ifdef __vita__
int TextConfiguration::getPSButtonsSpace() {
    return psButtonsSpace;
}

WImage* TextConfiguration::getImagePSButtons() {
    return imagePSButton;
}

void TextConfiguration::setPSButtonsSpace(int i) {
    psButtonsSpace = i;
}

void TextConfiguration::setImagePSButtons(WImage* im) {
    imagePSButton = im;
}

#endif
