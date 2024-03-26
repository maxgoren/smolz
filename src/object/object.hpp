#ifndef object_hpp
#define object_hpp
#include <iostream>
#include <vector>
using namespace std;
enum StoreAs {
    AS_INT, AS_REAL, AS_BOOL, AS_STRING, AS_LIST
};

struct ListNode;

struct Object {
    ListNode* listhead;
    bool isnull;
    StoreAs type;
    union {
        int intVal;
        float realVal;
        bool boolVal;
        string* stringVal;
    } prim;
    Object();
    Object(float real);
    Object(int num);
    Object(bool boolean);
    Object(string* str);
    Object(ListNode* list);
    //Object(const Object& obj);
    //Object& operator=(const Object& obj);
    bool operator==(const Object& obj) const noexcept;
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
Object* makeListObject(ListNode* arrayObj);
string toString(Object* object);
int compareObjects(Object* lhs, Object* rhs);
#endif