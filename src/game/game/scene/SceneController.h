/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __SCENECONTROLLER_H__
#define __SCENECONTROLLER_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/window/Event.h"

#include "Scene.h"

class SceneController {
    public :
        SceneController();
        ~SceneController();

        void launch(Save* sv);

        void saveData();

        void handleActions(Action* action);
        void loop();
        void draw();

        Scene* getScene();

        void setStep(SceneStep s);

    private :
        Scene scene;
};

#endif  // SceneController.h
