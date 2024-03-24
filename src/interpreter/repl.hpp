#ifndef repl_hpp
#define repl_hpp
#include <iostream>
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
#include "interpreter.hpp"
using std::string;
using std::cout;
using std::endl;


class ASTBuilder {
    private:
    
    public:
        ASTBuilder() {
            
        }
        ASTNode* build(string text) {
            Lexer lexer;
            Parser parser;
            return parser.parse(lexer.lexString(text));
        }
        ASTNode* fromFile(string filename) {
            Lexer lexer;
            Parser parser;
            return parser.parse(lexer.lex(filename));
        }
};


class REPL {
    private:
        ASTBuilder astBuilder;
        Interpreter interpreter;
    public:
        REPL() {

        }
        void repl() {
            string input;
            interpreter.setLoud(false);
            while (input != ".exit" && input != "quit") {
                cout<<"Smolz> ";
                getline(cin, input);
                cout<<"   '-> "<<input<<endl;
                if (input == ".trace") {
                    interpreter.setLoud(true);
                    continue;
                }
                if (input != ".exit" && input != "quit") {
                    auto ast = astBuilder.build(input);
                    ASTTracer trace;
                    trace.traverse(ast);
                    interpreter.run(ast);
                }
            }
        }
};

#endif