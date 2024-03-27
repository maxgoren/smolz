#include "interpreter.hpp"

ActivationRecord::ActivationRecord() {

}

Object* Interpreter::eval(ASTNode* node) {
    enter("eval");
    Object* lhs = expression(node->left);
    Object* rhs = expression(node->right);
    if (lhs->type != AS_LIST && rhs->type != AS_LIST && lhs->type != AS_STRING && rhs->type != AS_STRING ) {
        double left = lhs->realVal;
        double right = rhs->realVal;
        switch (node->data.tokenVal) {
            case PLUS:     return makeRealObject(left+right);
            case MINUS:    return makeRealObject(left-right);
            case DIVIDE:
                if (right == 0) {
                    cout<<"Error: attempted divide by zero"<<endl;
                    return makeRealObject(0.0f);
                }
                return makeRealObject(left/right);
            case MULTIPLY: return makeRealObject(left*right);
            case EQUAL:    return makeBoolObject(left == right);
            case LESS:     return makeBoolObject(left < right);
            case GREATER:  return makeBoolObject(left > right);
            case NOTEQUAL: return makeBoolObject(left != right);
            default:
                cout<<"Unknown Operator: "<<node->data.stringVal<<endl;
        }
    } else if ((lhs->type == AS_STRING || rhs->type == AS_STRING) && node->data.tokenVal == PLUS) {
        
        return makeStringObject(new string(toString(lhs) + toString(rhs)));
    } else {
        cout<<"Error: Unsupported operation for those types."<<endl;
    }
    leave();
    return makeRealObject(-1.0f);
}

ActivationRecord* Interpreter::prepareActivationRecord(ASTNode* node) {
    ActivationRecord* ar = new ActivationRecord();
    Procedure* func = procedures[node->data.stringVal];
    ar->function = func;
    for (auto it = func->paramList; it != nullptr; it = it->left) {
        ar->env.insert(make_pair(it->data.stringVal, memStore.storeAtNextFree(expression(node->left))));
        say(it->data.stringVal + " added to AR.");
    }
    ar->returnValue = makeRealObject(0.0);
    return ar;
}

Object* Interpreter::procedureCall(ASTNode* node) {
    Object* retVal;
    enter("[procedureCall]");
    if (procedures.find(node->data.stringVal) != procedures.end()) {
        callStack.push(prepareActivationRecord(node));
        say("Calling: " + node->data.stringVal);
        auto body = callStack.top()->function->functionBody;
        stopProcedure = false;
        run(body);
        retVal = callStack.top()->returnValue;
        say("Returned: " + to_string(retVal->realVal) + " from " + node->data.stringVal);
        for (auto toFree : callStack.top()->env) {
            memStore.free(toFree.second);
        }
        callStack.pop();
    } else {
        say("No such function: " + node->data.stringVal);
    }
    leave();
    return retVal;
}

Object* Interpreter::listExpr(ASTNode* node) {
    enter("[list_expr]");
    ListHeader* list = new ListHeader;
    list->size = 0;
    ASTNode* t = node->left;
    ListNode d;
    ListNode* x = &d;
    if (t == nullptr) {
        x = new ListNode;
        list->size = 0;
        list->head = nullptr;
        return makeListObject(list);
    }
    while (t != nullptr) {
        Object* obj = expression(t);
        say("push: " + toString(obj));
        x->next = new ListNode;
        x = x->next;
        x->data = obj;
        x->next = nullptr;
        t = t->left;
        list->size++;
    }
    list->head = d.next;
    return makeListObject(list);
}

int Interpreter::getAddress(string name) {
    int addr = 0;
    if (!callStack.empty() && callStack.top()->env.find(name) != callStack.top()->env.end())
        addr = callStack.top()->env[name];
    
    if (st.find(name) != st.end())
        addr = st[name];
   
    if (addr == 0)
        cout<<"Error: No List named "<<name<<" found."<<endl;
    
    return addr;
}

int Interpreter::listSize(ASTNode* node) {
    enter("[list size]");
    bool is_local = false;
    Object* obj;
    int addr, size;
    auto name = node->left->data.stringVal;
    addr = getAddress(name);
    if (addr > 0) {
        obj = memStore.get(addr);
        size = obj->list->size;
    }
    return size;
}

Object* Interpreter::getListItem(ASTNode* node, Object* listObj) {
    enter("listEntry");
    Object* tmp = expression(node->left);
    int arrIndex = tmp->realVal;
    say("Index: " + to_string(arrIndex));
    ListNode* x = listObj->list->head;
    if (arrIndex == 0)
        return x->data;
    int i = 0;
    while (i < arrIndex) {
        if (x->next != nullptr)
            x = x->next;
        else break;
        i++;
    }
    leave();
    return x->data;
}

void Interpreter::pushList(ASTNode* node) {
    enter("[push list]");
    Object* listObj;
    int addr;
    auto name = node->left->data.stringVal;
    say("list: " + name);
    Object* value = expression(node->right);
    say("push: " + toString(value));
    ListNode *toAdd = new ListNode;
    toAdd->data = value;
    toAdd->next = nullptr;
    addr = getAddress(name);
    if (addr > 0) {
        addr = st[name];
        listObj = memStore.get(addr);
        toAdd->next = listObj->list->head;
        listObj->list->head = toAdd;
        listObj->list->size += 1;
        memStore.store(addr, listObj);
    }
    leave();
}

void Interpreter::popList(ASTNode* node) {
    enter("[pop List]");
    bool is_local = false;
    Object* obj;
    int addr;
    auto name = node->left->data.stringVal;
    addr = getAddress(name);
    if (addr == 0) {
        cout<<"Error: No List named "<<name<<" found."<<endl;
        return;
    }
    obj = memStore.get(addr);
    ListNode* x = obj->list->head;
    if (x != nullptr) {
        obj->list->head = obj->list->head->next;
        delete x;
        obj->list->size -= 1;
        memStore.store(addr, obj);
    }
    leave();
}

Object* Interpreter::getVariableValue(ASTNode* node) {
    int addr = 0;
    Object* result;
    string name = node->data.stringVal;
    addr = getAddress(name);
    result = memStore.get(addr);
    if (result->type == AS_LIST && node->left != nullptr) {
         result = getListItem(node, result);
    }
    return result;
}

Object* Interpreter::expression(ASTNode* node) {
    Object* result, *tmp;
    int addr, arrIndex = 0;
    switch (node->type.expr) {
        case OP_EXPR:
            enter("[op expression]" + node->data.stringVal); leave();
            return eval(node);
        case ID_EXPR:
            enter("[id expression]");
            result = getVariableValue(node);
            if (result != nullptr) {
                leave();
                return result;
            }
            say("no variable named: " + node->data.stringVal);
            leave();
            return makeStringObject(new string("(err)"));
        case CONST_EXPR:
            enter("[const expression] " + node->data.stringVal); leave();
            return makeRealObject(stof(node->data.stringVal.c_str()));
        case FUNC_EXPR:
            enter("[func_expr] " + node->data.stringVal); leave();
            return procedureCall(node);
        case STRINGLIT_EXPR:
            enter("[string literal expression]"); leave();
            return makeStringObject(&(node->data.stringVal));
        case LIST_EXPR:
            enter("[list expression]"); leave();
            return listExpr(node);
        case LISTLEN_EXPR:
            enter("[listlen expr]"); leave();
            return makeIntObject(listSize(node));
        default:
            break;
    }
    return makeRealObject(0.0f);
}

void Interpreter::printStmt(ASTNode* node) {
    enter("[print]" + node->data.stringVal);
    Object* obj = expression(node->left);
    if (node->data.stringVal == "println") {
        cout<<toString(obj)<<endl;
    } else {
        cout<<toString(obj)<<flush;
    }
    leave();
}

void Interpreter::ifStmt(ASTNode* node) {
    enter("[if statement]");
    ASTNode* t = nullptr;
    say("testing condition");
    auto result = expression(node->left)->boolVal;
    if (result) {
        say("executing matching result");
        t = node->mid;
        while (t != nullptr) {
            statement(t);
            t = t->next;
        }
    } else {
        say("executing else clause");
        t = node->right;
        while (t != nullptr) {
            statement(t);
            t = t->next;
        }
    }
    leave();
}

void Interpreter::loopStmt(ASTNode* node) {
    enter("[loop]");
    while (expression(node->left)->boolVal) {
        ASTNode* t = node->right;
        while (t != nullptr) {
            statement(t);
            t = t->next;
        }
    }
}

void Interpreter::assignStmt(ASTNode* node) {
    enter("[assign]");
    string name = node->left->data.stringVal;
    Object* value = expression(node->right);
    int saveAddr = memStore.storeAtNextFree(value);
    if (!callStack.empty()) {
        callStack.top()->env[name] = saveAddr;
    } else {
        st[name] = saveAddr;
    }
    leave();
}

void Interpreter::defineFunction(ASTNode* node) {
    Procedure* np = new Procedure;
    np->name = node->data.stringVal;
    np->paramList = node->left;
    np->functionBody = node->right;
    procedures[np->name] = np;
    cout<<np->name<<" defined."<<endl;
}

void Interpreter::returnStmt(ASTNode* node) {
    enter("[return]");
    if (!callStack.empty()) {
        callStack.top()->returnValue = expression(node->left);
        say("Returning: " + toString(callStack.top()->returnValue));
        stopProcedure = true;
    }
    leave();
}

void Interpreter::statement(ASTNode* node) {
    enter("[statement]");
    switch (node->type.stmt) {
        case PRINT_STMT:
            printStmt(node);
            break;
        case ASSIGN_STMT:
            assignStmt(node);
            break;
        case PUSH_STMT:
            pushList(node);
            break;
        case POP_STMT:
            popList(node);
            break;
        case DEF_STMT:
            defineFunction(node);
            break;
        case IF_STMT:
            ifStmt(node);
            break;
        case LOOP_STMT:
            loopStmt(node);
            break;
        case RETURN_STMT:
            returnStmt(node);
            break;
        default: 
            cout<<"Invalid Statement: "<<node->data.stringVal<<endl;
        break;
    }
    leave();
}

Interpreter::Interpreter() {
    recDepth = 0;
    stopProcedure = false;
}

void Interpreter::run(ASTNode* node) {
    enter("[run]");
    if (node == nullptr)
        return;
    switch(node->kind) {
        case STMTNODE:
            say("-statement");
            statement(node);
            break;
        case EXPRNODE:
            say("-expression");
            expression(node);
            break;
    }
    if (stopProcedure) {
        stopProcedure = false;
        return;
    }
    say("sibling"); leave(); leave();
    run(node->next);
}

void Interpreter::enter(string s) {
    recDepth++;
    say(s);
}

void Interpreter::setLoud(bool l) {
    loud = l;
}

void Interpreter::say(string s) {
    if (loud) {
        for (int i = 0; i < recDepth; i++)
            cout<<"  ";
        cout<<"("<<recDepth<<") "<<s<<endl;
    }
}

void Interpreter::leave(string s) {
    say(s);
    recDepth--;
}

void Interpreter::leave() {
    --recDepth;
}