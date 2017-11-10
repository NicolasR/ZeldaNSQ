/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GLOBALRECORDSCONTROLLER_H__
#define __GLOBALRECORDSCONTROLLER_H__

#include "../../../../engine/common/Common.h"
#include "../../../../engine/window/Event.h"

#include "GlobalRecords.h"

class GlobalRecordsController {
    public :
        GlobalRecordsController();
        ~GlobalRecordsController();

        void launch();

        void handleEvents(Event* event);
        void draw();

    private :

        GlobalRecords global;
};

#endif  // GlobalRecordsController.h
