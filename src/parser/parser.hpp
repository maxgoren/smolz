#ifndef parser_hpp
#define parser_hpp
#include <iostream>
#include "../lexer/lexer.hpp"
#include "../ast/ast.hpp"
using std::string;



class Parser {
        vector<Lexeme> lexemes;
        int lexPos;
        Lexeme current;
        TOKENS lookahead();
        void nexttoken();
        bool match(TOKENS token);
    public:
        Parser();
        ASTNode* parse(vector<Lexeme>& tokens);
        ASTNode* program();
        ASTNode* statement();
        ASTNode* paramList();
        ASTNode* argsList();
        ASTNode* simpleExpr();
        ASTNode* expression();
        ASTNode* term();
        ASTNode* var();
        ASTNode* arrayExpr();
};

#endif