#include "closure.hpp"

Closure* makeClosure(ASTNode* funcBody, ASTNode* paramList, Environment env) {
    Closure* clos = new Closure;
    clos->paramList = paramList;
    clos->functionBody = funcBody;
    clos->env = env;
    return clos;
}