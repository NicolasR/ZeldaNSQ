#include "Quadtree.h"

Quadtree::Quadtree() : root(0) {
}

Quadtree::~Quadtree() {
    delete root;
}

void Quadtree::setBox(int x, int y, int w, int h) {
    box.setX(x - 16);
    box.setY(y - 16);
    box.setW(w + 32);
    box.setH(h + 32);
}

void Quadtree::add(Node* obj) {
    if (root == 0) {
        root = obj;
        root->setBox(box.getX(), box.getY(), box.getW(), box.getH());
    } else {
        root->add(obj);
    }
}

bool Quadtree::remove(Node* obj) {
    if (obj == root) {
        int sonId = root->getSonIdWithMaxLength();
        if (sonId >= 0) {
            Node* son = root->removeSon(sonId);
            son->permuteNodes(root);
            root = son;
        } else {
            root = 0;
        }
    } else {
        return root->remove(obj);
    }
    return true;
}

int Quadtree::size() {
    if (root == 0) {
        return 0;
    }
    return root->getSize();
}

int Quadtree::length() {
    if (root == 0) {
        return 0;
    }
    return root->getLength();
}

void Quadtree::get(BoundingBox* bb, List* list) {
    if (root != 0) {
        root->get(bb, list);
    }
}

void Quadtree::resetNodes(BoundingBox* bb) {
    if (root != 0) {
        root->resetNodes(bb);
    }
}

void Quadtree::getAll(List* list) {
    get(&box, list);
}

void Quadtree::removeDeadNodes() {
    if (root == 0) {
        return;
    }
    while (root != 0 && !root->isAlive() && !root->isResetable()) {
        Node* tmp = root;
        remove(tmp);
        tmp->clearSons();
        delete tmp;
    }
    if (root != 0) {
        root->removeDeadNodes();
    }
}

