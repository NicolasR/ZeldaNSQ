/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __RECORDSCONTROLLER_H__
#define __RECORDSCONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "../../../engine/renderer/SimpleTransition.h"

#include "global/GlobalRecordsController.h"
#include "normal/NormalRecordsController.h"
#include "expert/ExpertRecordsController.h"

enum RecordsStep {RECORDS_MAIN, RECORDS_NORMAL, RECORDS_EXPERT};

class RecordsController {
    public :
        RecordsController();
        ~RecordsController();

        void launch();

        void handleEvents(Event* event);
        void loop();
        void draw();

        void setStep(RecordsStep newStep);

        void setInternalStep(RecordsStep newStep); // set step without transition

    private :

        RecordsStep step;
        RecordsStep nextStep;

        SimpleTransition transition;

        GlobalRecordsController global;
        NormalRecordsController normal;
        ExpertRecordsController expert;
};

#endif  // RecordsController.h
