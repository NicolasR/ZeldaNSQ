#include "MapHelper.h"

MapHelper MapHelper::instance=MapHelper();

MapHelper::MapHelper() {
    herbeHaute.addType(TI_RUBIS_VERT, 7);
    herbeHaute.addType(TI_PETIT_COEUR, 7);
    herbeHaute.addType(TI_FLECHE, 7);
    herbeHaute.addType(TI_BOMBE, 7);
    herbeHaute.addType(TI_MAGIE_PEU, 7);

    buissonVert.addType(TI_RUBIS_VERT, 7);
    buissonVert.addType(TI_PETIT_COEUR, 7);
    buissonVert.addType(TI_FLECHE, 7);
    buissonVert.addType(TI_BOMBE, 7);
    buissonVert.addType(TI_MAGIE_PEU, 7);

    buissonRose.addType(TI_RUBIS_VERT, 5);
    buissonRose.addType(TI_PETIT_COEUR, 5);
    buissonRose.addType(TI_FLECHE, 5);
    buissonRose.addType(TI_BOMBE, 5);
    buissonRose.addType(TI_MAGIE_PEU, 5);

    buissonOcre.addType(TI_RUBIS_VERT, 3);
    buissonOcre.addType(TI_PETIT_COEUR, 3);
    buissonOcre.addType(TI_FLECHE, 3);
    buissonOcre.addType(TI_BOMBE, 3);
    buissonOcre.addType(TI_MAGIE_PEU, 3);

    buissonNeige.addType(TI_RUBIS_VERT, 2);
    buissonNeige.addType(TI_PETIT_COEUR, 2);
    buissonNeige.addType(TI_FLECHE, 2);
    buissonNeige.addType(TI_BOMBE, 2);
    buissonNeige.addType(TI_MAGIE_PEU, 2);

}

MapHelper::~MapHelper() {
}

MapHelper* MapHelper::getInstance() {
    return &instance;
}

ItemRandomizer* MapHelper::getItemRandomizer(TypeDons type) {
    switch (type) {
        case TD_HERBE_HAUTE : return &herbeHaute;
        case TD_BUISSON_VERT : return &buissonVert;
        case TD_BUISSON_ROSE : return &buissonRose;
        case TD_BUISSON_OCRE : return &buissonOcre;
        case TD_BUISSON_NEIGE : return &buissonNeige;
        default : return &herbeHaute;
    }
}

