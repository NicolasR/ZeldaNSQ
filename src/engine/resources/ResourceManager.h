/*

    Wolf of Light

    Copyright (C) 2010  Vincent Jouillat

*/

#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include <map>

#include "../common/Common.h"
#include "WImage.h"
#include "WMusic.h"
#include "WSound.h"
#include "WFile.h"

class ResourceManager {
    public :
        static ResourceManager* getInstance();
        WImage* loadImage(string filename, bool alpha = false);
        WMusic* loadMusic(string filename);
        WSound* loadSound(string filename);
        WFile* loadFile(string filename);
        void free(WResource* resource);
    private :
        ResourceManager();
        ~ResourceManager();
        static ResourceManager instance;

        map<string, WResource*> resources;
};

#endif  // ResourceManager.h
