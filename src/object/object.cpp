#include "object.hpp"

Object::Object() {
    type = AS_REAL;
    prim.realVal = 0.0f;
}

Object::Object(const Object& obj) {
    switch (obj.type) {
        case AS_BOOL:   prim.boolVal == obj.prim.boolVal;
        case AS_INT:    prim.intVal == obj.prim.intVal;
        case AS_REAL:   prim.realVal == obj.prim.realVal;
        case AS_STRING: prim.stringVal == obj.prim.stringVal;
        case AS_LIST:   prim.list == obj.prim.list;
    }
    isnull = obj.isnull;
    type = obj.type;
}

bool Object::operator==(const Object& obj) const noexcept {
    if (type == obj.type) {
        switch (obj.type) {
            case AS_BOOL: return prim.boolVal == obj.prim.boolVal;
            case AS_INT:  return prim.intVal == obj.prim.intVal;
            case AS_REAL: return prim.realVal == obj.prim.realVal;
            case AS_STRING: return prim.stringVal == obj.prim.stringVal;
            case AS_LIST: return  prim.list == obj.prim.list;
        }
    }
    return false;
}

bool Object::operator!=(const Object& obj) const noexcept {
    return !(*this == obj);
}

Object& Object::operator=(const Object& obj) {
    if (obj != *this) {
        switch (obj.type) {
            case AS_BOOL:   prim.boolVal == obj.prim.boolVal;
            case AS_INT:    prim.intVal == obj.prim.intVal;
            case AS_REAL:   prim.realVal == obj.prim.realVal;
            case AS_STRING: prim.stringVal == obj.prim.stringVal;
            case AS_LIST:   prim.list == obj.prim.list;
        }
        isnull = obj.isnull;
        type = obj.type;
    }
    return *this;
}

Object* makeObject(StoreAs type) {
    Object* obj = new Object;
    obj->type = type;
    obj->isnull = false;
    return obj;
}

Object* makeIntObject(int value) {
    Object* obj = makeObject(AS_INT);
    obj->prim.intVal = value;
    return obj;
}

Object* makeRealObject(float value) {
    Object* obj = makeObject(AS_REAL);
    obj->prim.realVal = value;
    return obj;
}

Object* makeBoolObject(bool value) {
    Object* obj = makeObject(AS_BOOL);
    obj->prim.boolVal = value;
    return obj;
}

Object* makeStringObject(string* value) {
    Object* obj = makeObject(AS_STRING);
    obj->prim.stringVal = value;
    return obj;
}

Object* makeListObject(ListHeader* list) {
    Object* obj = makeObject(AS_LIST);
    obj->prim.list = list;
    return obj;
}

string toString(Object* obj) {
    if (obj == nullptr)
        return "[err]";
    string str;
    Object* data;
    switch (obj->type) {
        case AS_REAL:   str = to_string(obj->prim.realVal); break;
        case AS_INT:    str = to_string(obj->prim.intVal);  break;
        case AS_BOOL:   str = obj->prim.boolVal ? "true":"false"; break;
        case AS_STRING: str = *obj->prim.stringVal; break;
        case AS_LIST: 
            str.push_back('[');
            if (obj->prim.list != nullptr) {
                for (ListNode* m = obj->prim.list->head; m != nullptr; m = m->next) {
                    data = m->data;
                    if (data == nullptr) {
                        str += "[err] ";
                    } else str += toString(m->data) + " ";
                }
            } else {
                str += "(empty)";
            }
            str.push_back(']');
            break;
        default: 
            str = "(empty)"; break;
    }
    return str;
}