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
    LSQ, RSQ, LPAREN, RPAREN, LCURLY, RCURLY,
    LESS, EQUAL, IF, ELSE, PRINT, LOOP, RETURN, PUSH, POP,
    SEMI, COLON, PERIOD, COMA, QUOTE
};

inline string tokenString[] = {
    "ERROR", "WHITESPACE", "EOFTOKEN",
    "DEF", "NUMBER", "STRING", "ID",
    "PLUS", "MINUS", "MULTIPLY", "DIVIDE",
    "ASSIGN", "LSQ", "RSQ", "LPAREN", "RPAREN", "LCURLY", "RCURLY",
    "LESS", "EQUAL", "IF", "ELSE", "PRINT", "LOOP", "RETURN", "PUSH", "POP",
    "SEMI", "COLON", "PERIOD", "COMA", "QUOTE", 
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