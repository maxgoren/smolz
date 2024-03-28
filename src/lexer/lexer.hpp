#ifndef lexer_hpp
#define lexer_hpp
#include "../buffer/stringbuffer.hpp"
#include <vector>
#include <iostream>
using namespace std;

enum TOKENS {
    ERROR, WHITESPACE, EOFTOKEN,
    DEF, NUMBER, STRING, ID,
    PLUS, MINUS, MULTIPLY, DIVIDE, ASSIGN,
    SEMI, COLON, PERIOD, COMA, QUOTE, NOT,
    LSQ, RSQ, LPAREN, RPAREN, LCURLY, RCURLY,
    LESS, GREATER, EQUAL, NOTEQUAL,
    IF, ELSE, PRINT, LOOP, RETURN, LAMBDA,
    PUSH, POP, APPEND, LENGTH, SORT
};

inline string tokenString[] = {
    "ERROR", "WHITESPACE", "EOFTOKEN",
    "DEF", "NUMBER", "STRING", "ID",
    "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "ASSIGN",
    "SEMI", "COLON", "PERIOD", "COMA", "QUOTE", "NOT",
    "LSQ", "RSQ", "LPAREN", "RPAREN", "LCURLY", "RCURLY",
    "LESS", "GREATER", "EQUAL", "NOTEQUAL", 
    "IF", "ELSE", "PRINT", "LOOP", "RETURN", "LAMBDA", 
    "PUSH", "POP", "APPEND", "LENGTH", "SORT"
};

struct Lexeme {
    TOKENS tokenVal;
    string stringVal;
    int lineNumber;
    Lexeme(TOKENS t = ID, string sv = "(empty)", int ln = 0) {
        tokenVal = t;
        stringVal = sv;
        lineNumber = ln;
    }
};

class Lexer {
    private:
        StringBuffer sb;
        vector<Lexeme> lexemes;
        Lexeme extractWord();
        Lexeme extractNumber();
        Lexeme checkSpecials();
        Lexeme extractStringLiteral();
        vector<Lexeme>& start();
    public:
        Lexer();
        vector<Lexeme>& lex(string filename);
        vector<Lexeme>& lexString(string str);
};


#endif