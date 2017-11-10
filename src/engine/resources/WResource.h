/*

    Wolf of Light

    Copyright (C) 2010  Vincent Jouillat

*/

#ifndef __WRESOURCE_H__
#define __WRESOURCE_H__

#include "../common/Common.h"

class WResource {
    public :
        WResource();
        virtual ~WResource() = 0;
        string getName();
        void load();
        int unload();
    private :
        int count;
    protected :
        string name;
};

#endif  // WResource.h
