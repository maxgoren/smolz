#ifndef object_hpp
#define object_hpp
#include <iostream>
#include <vector>
using namespace std;
enum StoreAs {
    AS_INT, AS_REAL, AS_BOOL, AS_STRING, AS_ARRAY
};

struct ListNode;

struct Object {
    StoreAs type;
    union {
        int intVal;
        float realVal;
        bool boolVal;
        string* stringVal;
        ListNode* listhead;
    };
    bool isnull;
};

struct ListNode {
    Object* data;
    ListNode* next;
};

Object* makeObject(StoreAs type);
Object* makeIntObject(int value);
Object* makeRealObject(float value);
Object* makeBoolObject(bool value);
Object* makeStringObject(string* object);
Object* makeArrayObject(ListNode* arrayObj);
string toString(Object* object);
#endif