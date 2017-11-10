#include "Node.h"

Node::Node() : width(1), height(1), previous(0), length(0), size(1), sonId(-1), maxWidth(1), maxHeight(1) {
    for (int i = 0; i < 4; i++) {
        sons[i] = 0;
    }
    alive = true;
}

Node::~Node() {
    for (int i = 0; i < 4; i++) {
        delete sons[i];
    }
}

void Node::setBox(int x, int y, int w, int h) {
    // only able once and for empty tree
    if (length != 0) {
        return;
    }
    bbox.setX(x);
    bbox.setY(y);
    bbox.setW(w);
    bbox.setH(h);
    computeLength();
    computeMaxSize();
}

void Node::add(Node* obj) {
    if (!bbox.hasPoint(obj->getX(), obj->getY())) {
        if (previous != 0) {
            previous->add(obj);
        } else {
            delete obj;
        }
        return;
    }

    int id = getSonFromPosition(obj->getX(), obj->getY());
    if (sons[id] == 0) {
        sons[id] = obj;
        obj->sonId = id;
        obj->previous = this;
        obj->computeLength();
        obj->computeMaxSize();
        obj->computeBox();
    } else {
        sons[id]->add(obj);
    }
    size++;
}

bool Node::remove(Node* obj) {
    if (!bbox.hasPoint(obj->getX(), obj->getY())) {
        return false;
    }
    int id = getSonFromPosition(obj->getX(), obj->getY());
    if (sons[id] == 0) {
        return false;
    }
    if (sons[id] == obj) {
        removeSon(id);
        computeLength();
        computeMaxSize();
        return true;
    } else {
        return sons[id]->remove(obj);
    }
}

int Node::getSonIdWithMaxLength() {
    int result = -1;
    int max = -1;
    for (int i = 0; i < 4; i++) {
        if (sons[i] != 0) {
            if (sons[i]->length > max) {
                max = sons[i]->length;
                result = i;
            }
        }
    }
    return result;
}

void Node::permuteNodes(Node* old) {
    previous = old->previous;
    for (int i = 0; i < 4; i++) {
        sons[i] = old->sons[i];
        if (sons[i] != 0) {
            sons[i]->previous = this;
        }
    }
    size = old->size;
    sonId = old->sonId;
    old->clearSons();
    computeLength();
    bbox.setX(old->bbox.getX());
    bbox.setY(old->bbox.getY());
    bbox.setW(old->bbox.getW());
    bbox.setH(old->bbox.getH());
}

Node* Node::removeSon(int id) {
    if (sons[id] == 0) {
        return 0;
    }
    Node* son = sons[id];

    int newId = son->getSonIdWithMaxLength();

    if (newId >= 0) {
        Node* newNode = son->removeSon(newId);
        sons[id] = newNode;
        newNode->permuteNodes(son);
    } else {
        sons[id] = 0;
    }

    for (int i = 0; i < 4; i++) {
        son->sons[i] = 0;
    }
    son->previous = 0;
    son->sonId = -1;
    computeSize();

    return son;
}

void Node::clearSons() {
    for (int i = 0; i < 4; i++) {
        sons[i] = 0;
    }
}

int Node::getSize() {
    return size;
}

int Node::getLength() {
    return length;
}

void Node::checkPosition() {
    if (!bbox.hasPoint(getX(), getY())) {
        if (previous == 0) {
            alive = false;
            return;
        }
        Node* tmp = previous;
        previous->removeSon(sonId);
        tmp->add(this);
    }
}

void Node::get(BoundingBox* bb, List* list) {

    if (alive) {
        BoundingBox b(getX(), getY(), width, height);
        if (b.intersect(bb)) {
            list->add(this);
        }
    }

    for (int i = 0; i < 4; i ++) {
        if (sons[i] != 0) {
            Node* son = sons[i];
            BoundingBox b2(son->bbox.getX(), son->bbox.getY(),
                           son->bbox.getW() + son->maxWidth,
                           son->bbox.getH() + son->maxHeight);
            if (b2.intersect(bb)) {
                son->get(bb, list);
            }
        }
    }
}

void Node::computeLength() {
    int old = length;
    length = 1;
    for (int i = 0; i < 4; i++) {
        if (sons[i] != 0) {
            if (sons[i]->length + 1 > length) {
                length = sons[i]->length + 1;
            }
        }
    }
    if (length != old && previous != 0) {
        previous->computeLength();
    }
}

void Node::computeMaxSize() {
    int oldW = maxWidth;
    int oldH = maxHeight;
    maxWidth = width;
    maxHeight = height;
    for (int i = 0; i < 4; i++) {
        if (sons[i] != 0) {
            if (sons[i]->maxWidth > maxWidth) {
                maxWidth = sons[i]->maxWidth;
            }
            if (sons[i]->maxHeight > maxHeight) {
                maxHeight = sons[i]->maxHeight;
            }
        }
    }
    if ((maxWidth != oldW || maxHeight != oldH) && previous != 0) {
        previous->computeMaxSize();
    }
}

void Node::computeSize() {
    int old = size;
    size = 1;
    for (int i = 0; i < 4; i++) {
        if (sons[i] != 0) {
            size += sons[i]->size;
        }
    }
    if (size != old && previous != 0) {
        previous->computeSize();
    }
}

void Node::computeBox() {
    if (previous == 0) {
        return;
    }

    int w = previous->bbox.getW() / 2;
    if (w == 0) w = 1;

    int h = previous->bbox.getH() / 2;
    if (h == 0) h = 1;

    if (sonId % 2 == 0) {
        bbox.setX(previous->bbox.getX());
    } else {
        bbox.setX(previous->bbox.getX() + w);
        w = previous->bbox.getW() - w;
    }
    if (sonId / 2 == 0) {
        bbox.setY(previous->bbox.getY());
    } else {
        bbox.setY(previous->bbox.getY() + h);
        h = previous->bbox.getH() - h;
    }
    bbox.setW(w);
    bbox.setH(h);
}

int Node::getSonFromPosition(int x, int y) {
    int result = 0;
    if (bbox.getW() > 1 && x >= bbox.getX() + (bbox.getW() / 2)) {
        result ++;
    }
    if (bbox.getH() > 1 && y >= bbox.getY() + (bbox.getH() / 2)) {
        result += 2;
    }
    return result;
}

bool Node::isAlive() {
    return alive;
}

void Node::killNode() {
    alive = false;
}

void Node::removeDeadNodes() {
    for (int i = 0; i < 4; i++) {
        Node* node = sons[i];
        if (node != 0) {
            node->removeDeadNodes();
            if (!node->alive && !node->isResetable()) {
                delete removeSon(i);
            }
        }
    }
}

bool Node::isResetable() {
    return false;
}

void Node::reset() {
}

void Node::resetNodes(BoundingBox* bb) {

    for (int i = 0; i < 4; i ++) {
        if (sons[i] != 0) {
            Node* son = sons[i];
            BoundingBox b2(son->bbox.getX(), son->bbox.getY(),
                           son->bbox.getW() + son->maxWidth,
                           son->bbox.getH() + son->maxHeight);
            if (b2.intersect(bb)) {
                son->resetNodes(bb);
            }
        }
    }

    if (isResetable()) {
        BoundingBox b(getX(), getY(), width, height);
        if (b.intersect(bb)) {
            reset();
        }
    }
}
