#include "interpreter.hpp"

Object* Interpreter::eval(ASTNode* node) {
    enter("eval");
    Object* lhs = expression(node->left);
    Object* rhs = expression(node->right);
    if (lhs->type == rhs->type && (lhs->type == AS_REAL || lhs->type == AS_INT)) {
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
            case EQUAL:    return makeRealObject(left == right);
            case LESS:     return makeRealObject(left < right);
            default:
                cout<<"Unknown Operator: "<<node->data.stringVal<<endl;
        }
    } else if (lhs->type == AS_STRING && rhs->type == AS_STRING && node->data.tokenVal == PLUS) {
        return makeStringObject(new string(*(lhs->stringVal) + *(rhs->stringVal)));
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
        ar->env.insert(it->data.stringVal, expression(node->left));
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
        callStack.pop();
    } else {
        say("No such function: " + node->data.stringVal);
    }
    return retVal;
}

Object* Interpreter::expression(ASTNode* node) {
    Object* result;
    switch (node->type.expr) {
        case OP_EXPR:
            enter("[op expression]" + node->data.stringVal); leave();
            return eval(node);
        case ID_EXPR:
            enter("[id expression]");
            if (!callStack.empty() && callStack.top()->env.lookup(node->data.stringVal)->realVal != -1.0) {
                result = callStack.top()->env.lookup(node->data.stringVal);
                leave("local variable: " + to_string(result->realVal));
                return result;
            }
            result = st.lookup(node->data.stringVal);
            leave("global variable: " + to_string(result->realVal));
            return result;
        case CONST_EXPR:
            enter("const expression: " + node->data.stringVal); leave();
            return makeRealObject(stof(node->data.stringVal.c_str()));
        case FUNC_EXPR:
            enter("procedure call: " + node->data.stringVal); leave();
            return procedureCall(node);
        case STRINGLIT_EXPR:
            enter("[string literal expression]");
            return makeStringObject(&(node->data.stringVal));
        default:
            leave("invalid expression.");
    }
    return makeRealObject(0.0f);
}

void Interpreter::printStmt(ASTNode* node) {
    enter("[print]");
    Object* obj = expression(node->left);
    switch (obj->type) {
        case AS_REAL: 
            cout<<obj->realVal<<endl;
            break;
        case AS_INT:
            cout<<obj->intVal<<endl;
            break;
        case AS_BOOL:
            if (obj->boolVal) cout<<"true"<<endl;
            else              cout<<"false"<<endl;
            break;
            if (obj->boolVal) cout<<"true"<<endl;
            else              cout<<"false"<<endl;
            break;
        case AS_STRING:
            cout<<*obj->stringVal<<endl;
            break;
        default:
            break;
    }
    leave();
}

void Interpreter::ifStmt(ASTNode* node) {
    enter("[if statement]");
    if (expression(node->left)->realVal) {
        ASTNode* t = node->right;
        while (t != nullptr) {
            statement(t);
            t = t->next;
        }
    }
    leave();
}

void Interpreter::loopStmt(ASTNode* node) {
    enter("[loop]");
    while (expression(node->left)->realVal) {
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
    st.insert(name, value);
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
        say("Returning: " + to_string(callStack.top()->returnValue->realVal));
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
    say("sibling");
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