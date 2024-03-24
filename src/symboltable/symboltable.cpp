#include "symboltable.hpp"

typename SymbolTable::link SymbolTable::addToTable(link h, link m) {
    if (h == nullptr) {
        n++;
        return m;
    }
    if (h->key == m->key) {
        h->value = m->value;
        delete m;
        return h;
    }
    h->next = addToTable(h->next, m);
    return h;
}

SymbolTable::SymbolTable() {
    head = nullptr;
    n = 0;
}

SymbolTable::SymbolTable(const SymbolTable& st) {
    head = nullptr;
    for (link it = st.head; it != nullptr; it = it->next)
        insert(it->key, it->value);
}

SymbolTable::~SymbolTable() {
    while (head) {
        link x = head;
        head = head->next;
        x->next = nullptr;
        delete x;
    }
}

void SymbolTable::insert(string key, Object* value) {
    head = addToTable(head, new node(key, value, nullptr));
}

Object* SymbolTable::lookup(string key) {
    for (link it = head; it != nullptr; it = it->next) {
        if (key == it->key)
            return it->value;
    }
    return makeRealObject(-1.0f);
}

SymbolTable& SymbolTable::operator=(const SymbolTable& st) {
    if (this->head != st.head) {
        head = nullptr;
        for (link it = st.head; it != nullptr; it = it->next)
            insert(it->key, it->value);
    }
    return *this;
}