#include "object.hpp"

Object* makeObject(StoreAs type) {
    Object* obj = new Object;
    obj->type = type;
    return obj;
}

Object* makeIntObject(int value) {
    Object* obj = makeObject(AS_INT);
    obj->intVal = value;
    return obj;
}

Object* makeRealObject(float value) {
    Object* obj = makeObject(AS_REAL);
    obj->realVal = value;
    return obj;
}

Object* makeBoolObject(bool value) {
    Object* obj = makeObject(AS_BOOL);
    obj->boolVal = value;
    return obj;
}

Object* makeStringObject(string* value) {
    Object* obj = makeObject(AS_STRING);
    obj->stringVal = value;
    return obj;
}