/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __AUDIOHELPER_H__
#define __AUDIOHELPER_H__

#include "../engine/common/Common.h"
#include "../engine/audio/AudioConfiguration.h"
#include "../engine/resources/WSound.h"

enum TypeSound{
    TS_MENU1, TS_MENU2, TS_MENU3,

    TS_STEP_GRASS1, TS_STEP_GRASS2,
    TS_STEP_TALLGRASS1, TS_STEP_TALLGRASS2,
    TS_STEP_DIRT1, TS_STEP_DIRT2,
    TS_STEP_ICE1, TS_STEP_ICE2,
    TS_STEP_WATER1, TS_STEP_WATER2,
    TS_STEP_SAND1, TS_STEP_SAND2,
    TS_STEP_STONE1, TS_STEP_STONE2,
    TS_STEP_WOOD1, TS_STEP_WOOD2,
    TS_STEP_CARPET1, TS_STEP_CARPET2,

    TS_JUMP, TS_PLOUF, TS_NAGE, TS_FALL, TS_SOL,
    TS_ATTACK1, TS_ATTACK2, TS_ATTACK3, TS_ATTACK4,
    TS_SWORD, TS_SWORDCHARGING, TS_SPIN,
    TS_HURT, TS_DANGER, TS_BOOM, TS_TOMBE, TS_SHOCK,
    TS_SHOOT, TS_THROW, TS_PORTE, TS_GRAPPIN,
    TS_MAGIC, TS_BURN, TS_MARTEAU, TS_PLOT,
    TS_CAPE_ON, TS_CAPE_OFF, TS_CANE, TS_BYRNA,
    TS_BUISSON, TS_DOOR, TS_SURPRISE,
    TS_ESCALIERS_DOWN, TS_ESCALIERS_UP,
    TS_TELEPORT, TS_BOOMERANG, TS_BAGUETTE_FEU,
    TS_BAGUETTE_GLACE,

    TS_NAVY_HEY, TS_NAVY_LISTEN, TS_NAVY_HELLO, TS_NAVY_LOOK,

    TS_SONG_1, TS_SONG_2, TS_SONG_3,

    TS_HEART, TS_RUPEE, TS_ITEM, TS_HAPPY, TS_MAGICCHARGE,

    TS_PUSH, TS_BREAK, TS_COFFRE, TS_POULE, TS_ERROR, TS_PICS,

    TS_HITENNEMY, TS_KILLENNEMY,

    TS_TEXT, TS_TEXTNEXT, TS_TEXTEND,

    TS_NB_MAX};

class AudioHelper : public AudioConfiguration {
    public :
        AudioHelper();
        ~AudioHelper();

        string getMusicName(int id);
        WSound* getSound(int id);

        void loadSounds();

        int getNbSounds();

    private :
        void deleteSounds();

        WSound** sounds;
};

#endif  // AudioHelper.h
