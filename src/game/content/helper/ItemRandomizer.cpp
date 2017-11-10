#include "ItemRandomizer.h"

#include "../../MainController.h"

ItemRandomizer::ItemRandomizer() {
}

ItemRandomizer::~ItemRandomizer() {
}

void ItemRandomizer::addType(TypeItem type, int proba) {
    p_node p;
    p.type = type;
    p.proba = proba;

    nodes.push_back(p);
}

TypeItem ItemRandomizer::selectType() {
    int val = rand() % 100;
    int cumul = 0;
    for(vector<p_node>::iterator i = nodes.begin(); i != nodes.end(); ++i){
        p_node p = *i;
        cumul += p.proba;
        if (val < cumul) return checkWithLink(p.type);
    }
    return TI_NO_ITEM;
}

TypeItem ItemRandomizer::checkWithLink(TypeItem type) {
    Link* link = MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink();

    switch (type) {
        case TI_RUBIS_VERT :
        case TI_RUBIS_BLEU :
        case TI_RUBIS_ROUGE :
            if (link->getStatus()->getRupees() >= link->getStatus()->getMaxRupees()) {
                return TI_NO_ITEM;
            }
            break;
        case TI_PETIT_COEUR :
            if (link->getStatus()->getLife() >= link->getStatus()->getMaxLife()) {
                return TI_NO_ITEM;
            }
            break;
        case TI_MAGIE_PEU :
        case TI_MAGIE_BEAUCOUP :
            if (!link->getStatus()->getMaxMagic()
                || link->getStatus()->getMagic() >= link->getStatus()->getMaxMagic()) {
                return TI_NO_ITEM;
            }
            break;
        case TI_BOMBE :
            if (!link->getInventory()->hasObject(BOMBES)
                            || link->getStatus()->getBombs() >= link->getStatus()->getMaxBombs()) {
                return TI_NO_ITEM;
            }
            break;
        case TI_FLECHE :
            if (!link->getInventory()->hasObject(ARC)
                            || link->getStatus()->getArrows() >= link->getStatus()->getMaxArrows()) {
                return TI_NO_ITEM;
            }
            break;
        default : return type;
    }
    return type;
}
