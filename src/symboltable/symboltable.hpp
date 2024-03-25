#ifndef symboltable_hpp
#define symboltable_hpp
#include <iostream>
#include "../object/object.hpp"
using std::string;

class SymbolTable {
    private:
        struct node {
            string key;
            Object* value;
            node* next;
            node(string k, Object* v, node* n) {
                key = k; value = v; next = n;
            }
        };
        typedef node* link;
        link head;
        int n;
        Object* nilObject;
        link addToTable(link h, link m);
    public:
        SymbolTable();
        SymbolTable(const SymbolTable& st);
        ~SymbolTable();
        void insert(string key, Object* value);
        Object*& lookup(string key);
        Object* nil();
        SymbolTable& operator=(const SymbolTable& st);
};

#endif