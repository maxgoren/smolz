#ifndef interpreter_hpp
#define interpreter_hpp
#include <iostream>
#include <map>
#include "../object/object.hpp"
#include "../parser/parser.hpp"
#include "../memstore/memstore.hpp"
#include "../symboltable/symboltable.hpp"
using namespace std;


struct Procedure {
    string name;
    ASTNode* paramList;
    ASTNode* functionBody;
};

struct ActivationRecord {
    Procedure* function;
    map<string, int> env;
    Object* returnValue;
    ActivationRecord();
};


class CallStack {
    private:
        ActivationRecord* stack[255];
        int p;
    public:
        CallStack() {
            p = 0;
        }
        bool empty() {
            return p == 0;
        }
        void push(ActivationRecord* ar) {
            stack[p++] = ar;
        }
        void pop() {
            ActivationRecord* t = top();
            p--;
            delete t;
        }
        ActivationRecord* top() {
            return stack[p-1];
        }
};

class Interpreter {
    private:
        bool loud;
        bool stopProcedure;
        int recDepth;
        void enter(string s);
        void leave(string s);
        void leave();
        void say(string s);
        map<string, int> st;
        map<string, Procedure*> procedures;
        CallStack callStack;
        MemStore memStore;
        ActivationRecord* prepareActivationRecord(ASTNode* node);
        Object* resolveVariableName(ASTNode* node);
        Object* procedureCall(ASTNode* node);
        Object* eval(ASTNode* node);
        Object* expression(ASTNode* node);
        Object* listExpr(ASTNode* node);
        Object* getListItem(ASTNode* node, Object* arr);
        void popList(ASTNode* node);
        void pushList(ASTNode* node);
        void returnStmt(ASTNode* node);
        void printStmt(ASTNode* node);
        void ifStmt(ASTNode* node);
        void loopStmt(ASTNode* node);
        void assignStmt(ASTNode* node);
        void statement(ASTNode* node);
        void defineFunction(ASTNode* node);
    public:
        Interpreter();
        void run(ASTNode* node);
        void setLoud(bool isloud);
};

#endif