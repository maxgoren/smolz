#include <iostream>
#include "interpreter/interpreter.hpp"
#include "interpreter/repl.hpp"
using namespace std;

void interpretFile(string filename, bool loud) {
    Interpreter interpreter;
    ASTBuilder astBuilder;
    auto ast = astBuilder.fromFile(filename);
    ASTTracer tracer;
    tracer.traverse(ast);
    interpreter.setLoud(loud);
    interpreter.run(ast);
}

void repl() {
    REPL pler;
    pler.repl();
}

int main(int argc, char* argv[]) {
     if (argc < 2) {
        cout<<"usage: "<<endl;
        cout<<"smolz <filename>"<<endl;
        cout<<"smolz -r (repl)"<<endl;
        return 0;
    }
    if (argv[1][0] == '-' && argv[1][1] == 'r')
         repl();
    else if (argv[1][0] == '-' && argv[1][1] == 'v') {
        interpretFile(argv[2], true);
    } else {
        interpretFile(argv[1], false);
    }
    return 0;
}