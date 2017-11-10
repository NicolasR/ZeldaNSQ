/*

    Wolf of Light

    Copyright (C) 2010  Vincent Jouillat

*/

#ifndef __WFILE_H__
#define __WFILE_H__

#include "../common/Common.h"
#include "WResource.h"

class WFile : public WResource {
    public :
        WFile(string filename);
        ~WFile();
        char* getBuffer();
        long getSize();
    private :
        char* buffer;
        long size;
};

#endif  // WFile.h
