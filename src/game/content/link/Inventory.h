/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "../../../engine/common/Common.h"
#include "../../../engine/resources/WImage.h"

#include "../../Save.h"

class Inventory {
    public :
        Inventory(Save* save);
        ~Inventory();

        void saveData(Save* save);

        void draw(Equipment e, int x, int y);

        void drawTroc(TypeTroc t, int x, int y);

        void setObject(Equipment e, int value = 1);

        bool hasObject(Equipment e);

        bool hasObject();

        Equipment getCurrent();

        void setCurrent(Equipment e);

        bool hasQuartCoeur(int i);

        void setQuartCoeur(int i, int value = 1);

        int nbQuartCoeur();

        int totalQuartCoeur();

        bool hasCoeur(int i);

        void setCoeur(int i, int value = 1);

        void setGemmeForce(int i, int value = 1);

        bool hasGemmeForce(int i);

        int nbGemmeForce();

        int totalGemmeForce();

        void removeGemme(int nb);

        void setCoquillages(int i, int value = 1);

        bool hasCoquillages(int i);

        int nbCoquillages();

        int totalCoquillages();

        void removeCoquillages(int nb);

        void setMedaillons(int i, int value = 1);

        bool hasMedaillons(int i);

        int nbMedaillons();

        int getFlacon(int i);

        void setFlacon(int i, int value);

        int getCurrentFlacon();

        void setCurrentFlacon(int value);

        int getTroc(int i);

        void setTroc(int i, int value);

        void addTroc(int i);

        void removeTroc(int i, int val);

    private :

        void loadFromSave(Save* save);

        void drawFlacon(int x, int y, int value);

        int equipment[NB_EQPMT];

        Equipment current;

        WImage* inventaire;

        WImage* imageObjects;

        int quartCoeur[NB_QUART_COEUR];

        int coeur[NB_COEUR];

        int gemmeForce[NB_GEMME_FORCE];

        int coquillages[NB_COQUILLAGES];

        int medaillons[NB_MEDAILLONS];

        int flacons[NB_FLACON];

        int troc[NB_TROC];
};

#endif  // Inventory.h
