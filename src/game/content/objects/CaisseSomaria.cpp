#include "CaisseSomaria.h"

#include "../../../engine/resources/ResourceManager.h"
#include "../../../engine/window/WindowManager.h"
#include "../../../engine/audio/AudioManager.h"

#include "../../game/scene/Scene.h"

#include "../../MainController.h"

#include "../effects/FumeeBlanche.h"

#include "../helper/ProjectileHelper.h"

CaisseSomaria::CaisseSomaria(int x, int y) : Caisse(x, y) {
    moving = true;
}

CaisseSomaria::~CaisseSomaria() {
    MainController::getInstance()->getGameController()->getSceneController()->getScene()->getLink()->notifySomariaEnd();
}

void CaisseSomaria::draw(int offsetX, int offsetY) {

    int i = x - offsetX;
    int j = y - offsetY;

    if (moving) {
        if (dx < 0) {
            i += width - 16;
        } else if (dy < 0) {
            j += height - 16;
        }
    }

    WindowManager::getInstance()->draw(image, 0, 32, 16, 16, i, j);
}

void CaisseSomaria::explose() {
    MainController::getInstance()->getGameController()->getSceneController()->getScene()->
        getMap()->addEffect(new FumeeBlanche(x + 8, y + 8, 4));

    ProjectileHelper::getInstance()->addProjectile(TP_FEU_LONG_LINK, x + 5, y - 8, N);
    ProjectileHelper::getInstance()->addProjectile(TP_FEU_LONG_LINK, x + 5, y + 8, S);
    ProjectileHelper::getInstance()->addProjectile(TP_FEU_LONG_LINK, x - 8, y + 5, W);
    ProjectileHelper::getInstance()->addProjectile(TP_FEU_LONG_LINK, x + 8, y + 5, E);

    alive = false;
}

bool CaisseSomaria::isOnGap(BoundingBox* b) {
    bool result = Poussable::isOnGap(b);
    if (result) {
    }
    return result;
}

bool CaisseSomaria::isResetable() {
    return false;
}
