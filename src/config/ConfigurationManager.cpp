#include "ConfigurationManager.h"

#include "../engine/audio/AudioManager.h"
#include "../engine/texts/TextManager.h"
#include "KeyBinder.h"

ConfigurationManager ConfigurationManager::instance=ConfigurationManager();

ConfigurationManager::ConfigurationManager() : haveToSave(false), file(""),
    volume(128), volson(128), lang(0), skin(1), audio(0), text(0) {
}

ConfigurationManager::~ConfigurationManager() {
}

ConfigurationManager* ConfigurationManager::getInstance() {
    return &instance;
}

void ConfigurationManager::init(string filename, string keys) {
    file = filename;
    haveToSave = false;

    ifstream f(file.c_str(), ios::in | ios::binary);
    if(f.is_open()) {
        f.read((char *)&volume,sizeof(int));
        f.read((char *)&volson,sizeof(int));
        f.read((char *)&lang,sizeof(int));
        f.read((char *)&skin,sizeof(int));
        f.close();
    }

    audio = new AudioHelper();

    AudioManager::getInstance()->init();
    AudioManager::getInstance()->setConfiguration(audio);
    AudioManager::getInstance()->setVolumeMusic(volume);
    AudioManager::getInstance()->setVolumeSound(volson);

    text = new TextHelper();
    text->setLanguage(lang);

    TextManager::getInstance()->setConfiguration(text);

    KeyBinder::getInstance()->load(keys);
}

void ConfigurationManager::close() {
    delete audio;
    delete text;
}

void ConfigurationManager::save() {
    if (!haveToSave) {
        return;
    }
    ofstream f(file.c_str(), ios::out | ios::binary);
    f.write((char *)&volume,sizeof(int));
    f.write((char *)&volson,sizeof(int));
    f.write((char *)&lang,sizeof(int));
    f.write((char *)&skin,sizeof(int));
    f.close();
    haveToSave = false;
}

bool ConfigurationManager::isFrench() {
    return lang == 0;
}

string ConfigurationManager::getSkinName() {
    switch (skin) {
        case 0 : return "Original";
        case 1 : return "Blond";
        case 2 : return "Glass";
        default : return "Blond";
    }
}

int ConfigurationManager::getVolume() {return volume;}
int ConfigurationManager::getVolson() {return volson;}
int ConfigurationManager::getLang() {return lang;}
int ConfigurationManager::getSkin() {return skin;}

void ConfigurationManager::setVolume(int v) {
    volume = v;
    haveToSave = true;
    AudioManager::getInstance()->setVolumeMusic(volume);
}

void ConfigurationManager::setVolson(int v) {
    volson = v;
    haveToSave = true;
    AudioManager::getInstance()->setVolumeSound(volson);
}

void ConfigurationManager::setLang(int l) {
    lang = l;
    text->setLanguage(lang);
    TextManager::getInstance()->reloadTexts();
    haveToSave = true;
}

void ConfigurationManager::setSkin(int s) {
    skin = s;
    haveToSave = true;
}
