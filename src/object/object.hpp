#ifndef object_hpp
#define object_hpp
#include <iostream>
#include <vector>
using namespace std;
enum StoreAs {
    AS_INT, AS_REAL, AS_BOOL, AS_STRING, AS_LIST
};

struct ListHeader;

struct Object {
    bool isnull;
    StoreAs type;
    union {
        int intVal;
        float realVal;
        bool boolVal;
        string* stringVal;
        ListHeader* list;
    } prim;
    Object();
    Object(const Object& obj);
    Object& operator=(const Object& obj);
    bool operator==(const Object& obj) const noexcept;
    bool operator!=(const Object& obj) const noexcept;
};

struct ListNode {
    Object* data;
    ListNode* next;
};

struct ListHeader {
    int size;
    ListNode* head;
};

Object* makeObject(StoreAs type);
Object* makeIntObject(int value);
Object* makeRealObject(float value);
Object* makeBoolObject(bool value);
Object* makeStringObject(string* object);
Object* makeListObject(ListHeader* listObj);
string toString(Object* object);
int compareObjects(Object* lhs, Object* rhs);
#endif