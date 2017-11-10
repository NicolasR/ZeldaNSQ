#include "Object.h"

#include "../../MainController.h"

Object::Object() {
    attackable = false;
}

Object::~Object() {
}

void Object::loop() {
}

int Object::getX() {return x;}
int Object::getY() {return y;}
BoundingBox* Object::getBoundingBox() {return &box;}
bool Object::isAttackable() {return attackable;}
void Object::underAttack(Direction dir, int force, TypeAttack type, TypeEffect effect) {}
bool Object::action(Direction dir) {return false;}
BoundingBox* Object::getSecondBoundingBox() {return 0;}
