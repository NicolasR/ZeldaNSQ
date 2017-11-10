#include "TeleportController.h"

TeleportController::TeleportController() {
}

TeleportController::~TeleportController() {
}

void TeleportController::launch() {
    teleport.init();
}

void TeleportController::loop() {
    teleport.loop();
}

void TeleportController::draw() {
    teleport.draw();
}

void TeleportController::setTeleport(int map, int x, int y, Direction dir, bool instant, bool reload, bool etage) {
    teleport.setTeleport(map, x, y, dir, instant, reload, etage);
}
