#include "ResourceManager.h"

ResourceManager ResourceManager::instance=ResourceManager();

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
}

ResourceManager* ResourceManager::getInstance() {
    return &instance;
}

WImage* ResourceManager::loadImage(string filename, bool alpha) {

    if (resources.find(filename) == resources.end()) {
        resources[filename] = new WImage(filename, alpha);
    }

    WImage* image = dynamic_cast<WImage*>(resources[filename]);

    if (image != 0) {
        image->load();
    }

    return image;
}

WMusic* ResourceManager::loadMusic(string filename) {

    if (resources.find(filename) == resources.end()) {
        resources[filename] = new WMusic(filename);
    }

    WMusic* music = dynamic_cast<WMusic*>(resources[filename]);

    if (music != 0) {
        music->load();
    }

    return music;
}

WSound* ResourceManager::loadSound(string filename) {

    if (resources.find(filename) == resources.end()) {
        resources[filename] = new WSound(filename);
    }

    WSound* sound = dynamic_cast<WSound*>(resources[filename]);

    if (sound != 0) {
        sound->load();
    }

    return sound;
}

WFile* ResourceManager::loadFile(string filename) {

    if (resources.find(filename) == resources.end()) {
        resources[filename] = new WFile(filename);
    }

    WFile* file = dynamic_cast<WFile*>(resources[filename]);

    if (file != 0) {
        file->load();
    }

    return file;
}

void ResourceManager::free(WResource* resource) {
    if (resource == 0) {
        return;
    }
    if (resource->unload() == 0) {
        resources.erase(resource->getName());
        delete resource;
    }
}
