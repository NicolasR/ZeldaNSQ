#include "List.h"

List::List() : head(0), current(0), length(0) {
}

List::~List() {
    delete head;
}

void List::add(Listable* obj) {
    if (obj == 0) {
        return;
    }

    ListCell* cell = new ListCell(obj);
    cell->cleanNext();

    if (head == 0) {
        head = cell;
    } else if (head->getContent()->compareTo(obj) < 0) {
        ListCell* tmp = head;
        head = cell;
        cell->add(tmp);
    } else {
        head->add(cell);
    }
    length++;
}

bool List::remove(Listable* obj) {
    if (obj == 0) {
        return false;
    }
    if (head->getContent() == obj) {
        ListCell* tmp = head;
        head = tmp->getNext();
        tmp->cleanNext();
        delete tmp;
        length--;
        return true;
    }
    if (head->remove(obj)) {
        length--;
        return true;
    }
    return false;
}

Listable* List::remove(int i) {
    if (i < 0 || i >= length) {
        return 0;
    }
    ListCell* tmp = head;
    Listable* result = 0;

    if (i == 0) {
        result = head->getContent();
        tmp = head->getNext();
        head->cleanNext();
        delete head;
        head = tmp;
        return result;
    }
    ListCell* previous = 0;
    for (int j = 0; j < i; j++) {
        previous = tmp;
        tmp = tmp->getNext();
    }
    previous->setNext(tmp->getNext());

    result = tmp->getContent();
    tmp->cleanNext();
    delete tmp;
    return result;
}

int List::size() {
    return length;
}

void List::merge(List* other) {
    if (other == 0) {
        return;
    }
    if (head == 0) {
        head = other->head;
    } else {
        head->merge(other->head);
    }
    length += other->length;
}

Listable* List::get(int i) {
    if (i < 0 || i >= length) {
        return 0;
    }
    ListCell* tmp = head;
    for (int j = 0; j < i; j++) {
        tmp = tmp->getNext();
    }
    if (tmp == 0) return 0;
    return tmp->getContent();
}

void List::iterateOnFirst() {
    current = head;
}

bool List::hasNext() {
    return (current != 0);
}

Listable* List::getNext() {
    ListCell* l = current;
    current = current->getNext();
    return l->getContent();
}
