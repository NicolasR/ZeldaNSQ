/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __NORMALRECORDSCONTROLLER_H__
#define __NORMALRECORDSCONTROLLER_H__

#include "../../../../engine/common/Common.h"
#include "../../../../engine/window/Event.h"

#include "NormalRecords.h"

class NormalRecordsController {
    public :
        NormalRecordsController();
        ~NormalRecordsController();

        void launch();

        void handleEvents(Event* event);
        void draw();

    private :

        NormalRecords normal;
};

#endif  // NormalRecordsController.h
