#ifndef object_hpp
#define object_hpp
#include <iostream>
using namespace std;
enum StoreAs {
    AS_INT, AS_REAL, AS_BOOL, AS_STRING
};

struct Object {
    StoreAs type;
    union {
        int intVal;
        float realVal;
        bool boolVal;
        string* stringVal;
    };
};

Object* makeObject(StoreAs type);
Object* makeIntObject(int value);
Object* makeRealObject(float value);
Object* makeBoolObject(bool value);
Object* makeStringObject(string* object);
string toString(Object* object);
#endif