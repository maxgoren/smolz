#ifndef CLOSURE_HPP
#define CLOSURE_HPP
#include <iostream>
#include <map>
#include "../ast/ast.hpp"
using namespace std;

typedef map<string, int> Environment;

struct Closure {
    ASTNode* paramList;
    ASTNode* functionBody;
    Environment env;
};

Closure* makeClosure(ASTNode* funcBody, ASTNode* paramList, Environment env);

#endif