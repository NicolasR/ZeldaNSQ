/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __SLIDEMANAGER_H__
#define __SLIDEMANAGER_H__

/**
 * this class is to use to handle slides.
 * SetMoves is to set current moves to compute if we have to slide.
 * GetSlideX and getSlideY return slide.
 * StopX and stopY stop the slide.
 * raz reinitializes the manager.
 */
class SlideManager {
    public :
        SlideManager();
        ~SlideManager();

        void setMoves(int dx, int dy, int vitesse);

        int getSlideX();
        int getSlideY();

        void stopX();
        void stopY();

        void raz();

    private :

        void fillHisto(int dir, int vitesse);
        void computeDistance(int dir);

        int histo[4][16];
        int speedToSlide[4]; // speed to begin to slide
        int distanceToSlide[4];
        int elan[4];
};

#endif  // SlideManager.h


