/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ANIMATIONINGAME_H__
#define __ANIMATIONINGAME_H__

#include "../link/Link.h"
#include "../link/Navi.h"
#include "../scene/Map.h"
#include "../scene/Camera.h"

#include "../../../engine/renderer/SimpleTransition.h"
#include "../../../engine/renderer/Surface.h"

class AnimationInGame {
    public :
        AnimationInGame();
        ~AnimationInGame();

        void testAnim(); // startAnim if needed
        void startAnim(int id);
        void loop();
        void draw();
        void drawAfterHud();

        bool isRunning();

        int getSpecialMusicId();

        bool loopMap();
        bool loopCamera();
        bool loopLink();
        bool loopNavi();

    private :

        int id;
        int anim;

        bool running;

        bool lmap;
        bool lcamera;
        bool llink;
        bool lnavi;

        Link* link;
        Navi* navi;
        Map* map;
        Camera* camera;

        WImage* image;
        Surface* surface;
        SimpleTransition transition;

};

#endif  // AnimationInGame.h

