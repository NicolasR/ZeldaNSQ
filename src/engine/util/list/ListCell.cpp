#include "ListCell.h"

ListCell::ListCell(Listable* obj) : content(obj), next(0) {
}

ListCell::~ListCell() {
    // we don't delete content when we delete the cell
    delete next;
}

int ListCell::compareTo(ListCell* other) {
    if (content == 0 || other->content == 0) return 0;
    return content->compareTo(other->content);
}

void ListCell::add(ListCell* obj) {
    if (obj == 0) {
        return;
    }
    if (next == 0) {
        next = obj;
    } else if (next->compareTo(obj) < 0) {
        ListCell* tmp = obj->next;
        obj->next = next;
        next = obj;
        next->add(tmp);
    } else {
        next->add(obj);
    }
}

bool ListCell::remove(Listable* obj) {
    if (next == 0) {
        return false;
    }
    if (next->content == obj) {
        ListCell* tmp = next->next;
        next->cleanNext();
        delete next;
        next = tmp;
        return true;
    }
    return next->remove(obj);
}

void ListCell::merge(ListCell* obj) {
    if (obj == 0) {
        return;
    }
    if (next == 0) {
        next = obj;
    } else if (next->compareTo(obj) < 0) {
        ListCell* tmp = obj->next;
        obj->next = next;
        next = obj;
        next->merge(tmp);
    } else {
        next->merge(obj);
    }
}

Listable* ListCell::getContent() {
    return content;
}

ListCell* ListCell::getNext() {
    return next;
}

void ListCell::setNext(ListCell* obj) {
    next = obj;
}

void ListCell::cleanNext() {
    next = 0;
}
