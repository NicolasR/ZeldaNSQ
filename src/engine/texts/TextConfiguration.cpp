#include "TextConfiguration.h"

TextConfiguration::TextConfiguration() : wSpace(0), wSize(0), hSize(0), image(0), textfile("") {
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
    textfile = s;
}

void TextConfiguration::setImage(WImage* im) {
    image = im;
}
