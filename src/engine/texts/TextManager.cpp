#include "TextManager.h"

#include "../window/WindowManager.h"

TextManager TextManager::instance=TextManager();

TextManager::TextManager() : config(0) {
}

TextManager::~TextManager() {
}

TextManager* TextManager::getInstance() {
    return &instance;
}

void TextManager::setConfiguration(TextConfiguration* conf) {
    config = conf;
    reloadTexts();
}

void TextManager::reloadTexts() {
    loadTextsFromFile(config->getTextfile());
}

int TextManager::getWSpace() {
    return config->getWSpace();
}

int TextManager::getWSize() {
    return config->getWSize();
}

int TextManager::getHSize() {
    return config->getHSize();
}

WImage* TextManager::getImage() {
    return config->getImage();
}

#ifdef __vita__
int TextManager::getPSButtonsSpace() {
    return config->getPSButtonsSpace();
}

WImage* TextManager::getImagePSButtons() {
    return config->getImagePSButtons();
}
#endif

Text* TextManager::getText(int id) {
    string text = texts[id];

    int begin = -1;
    int end = -1;
    bool in = false;
    for (unsigned int i = 0; i < text.length(); i++) {

        if (in) {
            if (text[i] == ']') {
                char type = text[begin + 1];
                end = i;

                // only color styles ( like [c=?] ) are ignored
                if (text[begin + 2] != '=' || type != 'c') {
                    if (text[begin + 2] != '=') {
                        string newValue = config->getCommonValue(text.substr(begin + 1, end - begin - 1));
                        text.replace(begin, end - begin + 1, newValue);
                        i += (newValue.length() - (end - begin + 1));
                    } else {
                        string value = text.substr(begin + 3, end - begin - 1);
                        int number;
                        istringstream ss(value);
                        ss >> number;
                        string newValue = config->getVariableValue(id, number);
                        text.replace(begin, end - begin + 1, newValue);
                        i += (newValue.length() - (end - begin + 1));
                    }
                }

                in = false;
            }
        } else {
            if (text[i] == '[') {
                in = true;
                begin = i;
            }
        }



    }

    return new Text(text);
}

void TextManager::loadTextsFromFile(string filename) {
    texts.clear();

    ifstream file(filename.c_str());

    if (file) {
        string line;
        while (getline(file, line)) {
            texts.push_back(line);
        }
    }
}

#ifdef __vita__
int TextManager::affichePSBouton(int index, int i, int j, Surface* surface, int style) {
    int x = 0;
    int w = getPSButtonsSpace();
    int h = 16;
    int y = 0;
    
    // Playstation button
    // 1 => Triangle
    // 2 => Rond
    // 3 => Croix
    // 4 => Carrï¿½
    // 5 => Pad Droite
    // 6 => Pad Haut
    // 7 => Pad Gauche
    // 8 => Pad Bas
    // 9 => Stick analogique
	switch(index){
        case 1: 
            x=2;
            y=2;
            h=16;
            w=16;
            break;
        case 2: 
            x=22;
            y=2;
            h=16;
            w=16;
            break;
        case 3: 
            x=42;
            y=2;
            h=16;
            w=16;
            break;
        case 4: 
            x=62;
            y=2;
            h=16;
            w=16;
            break;
        case 5:
            x=2;
            y=24;
            h=12;
            w=16;
            j+=2;
            break;
        case 6:
            x=24;
            y=22;
            h=16;
            w=12;
            break;
        case 7:
            x=42;
            y=24;
            h=12;
            w=16;
            j+=2;
            break;
        case 8:
            x=64;
            y=22;
            h=16;
            w=12;
            break;
        case 9:
            x=2;
            y=42;
            h=16;
            w=16;
            break;
	}

    //int y = style * h;    
    
    if (surface == 0) {
        WindowManager::getInstance()->draw(getImagePSButtons(), x, y, w, h, i, j);
    } else {
        surface->put(getImagePSButtons(), i, j, x, y, w, h);
    }

    return w;
}
#endif

void TextManager::drawLetter(char c, int i, int j, int style) {
    drawLetterInternal(c, i, j, style, 0);
}

void TextManager::drawLetterOnSurface(char c, int i, int j, Surface* surface, int style) {
    if (surface == 0) {
        return;
    }
    drawLetterInternal(c, i, j, style, surface);
}

void TextManager::drawLetterInternal(char c, int i, int j, int style, Surface* surface) {

    int x = 0;
    int y = style * getHSize();
    int w = getWSize();
    int h = getHSize();

    int val = (int)c;

    if (val == 32) return;

    //minuscules a-z
    if (val >= 97 && val <= 122) {
        x = w * (26 + (val - 97));
    }

    //majuscules A-Z
    if (val >= 65 && val <= 90) {
        x = w * (val - 65);
    }

    //chiffres
    if (val >= 48 && val <= 57) {
        x = w * (52 + (val - 48));
    }

    if (c == 'à') {x = w * 62;}
    if (c == 'â') {x = w * 63;}
    if (c == 'ä') {x = w * 64;}
    if (c == 'ç') {x = w * 65;}
    if (c == 'è') {x = w * 66;}
    if (c == 'é') {x = w * 67;}
    if (c == 'ë') {x = w * 68;}
    if (c == 'ê') {x = w * 69;}
    if (c == 'ï') {x = w * 70;}
    if (c == 'î') {x = w * 71;}
    if (c == 'ô') {x = w * 72;}
    if (c == 'ö') {x = w * 73;}
    if (c == 'ù') {x = w * 74;}
    if (c == 'û') {x = w * 75;}
    if (c == 'ü') {x = w * 76;}
    if (c == '-') {x = w * 77;}
    if (c == '.') {x = w * 78;}
    if (c == ',') {x = w * 79;}
    if (c == '\'') {x = w * 80;}
    if (c == '!') {x = w * 81;}
    if (c == '?') {x = w * 82;}
    if (c == ':') {x = w * 83;}
    if (c == '%') {x = w * 84;}
    if (c == '(') {x = w * 85;}
    if (c == ')') {x = w * 86;}
    if (c == '/') {x = w * 87;}

    //sprite.SetSubRect(sf::IntRect(x, y, x + w, y + h));
    //sprite.SetPosition(i, j);
    //WindowManager::getInstance()->draw(sprite);
    if (surface == 0) {
        WindowManager::getInstance()->draw(getImage(), x, y, w, h, i, j);
    } else {
        surface->put(getImage(), i, j, x, y, w, h);
    }

}
