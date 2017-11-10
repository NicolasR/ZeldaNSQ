/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __EXPERTRECORDSCONTROLLER_H__
#define __EXPERTRECORDSCONTROLLER_H__

#include "../../../../engine/common/Common.h"
#include "../../../../engine/window/Event.h"

#include "ExpertRecords.h"

class ExpertRecordsController {
    public :
        ExpertRecordsController();
        ~ExpertRecordsController();

        void launch();

        void handleEvents(Event* event);
        void draw();

    private :

        ExpertRecords expert;
};

#endif  // ExpertRecordsController.h
