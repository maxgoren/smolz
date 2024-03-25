# Smolz

Smolz is a small (get it?), simple programming language
meant to show the basics of lexing/parsing, AST construction,
and to a lesser extent, interpretation. It can be used as a
an instructional tool, or serve as a starting point for a 
more serious effort.

The grammar is purposefully small and easily parsed. 
Smolz has variables that are declared by use, and are dynamically typed.
Smolz also features conditional branching and iteration through the use of
if statements and the 'loop' keyword which is analagous to 'while' loops. 
functions are defined using the 'def' keyword, and can call themselves recursively.

    def fib(m) {
        if (m < 2) {
            return 1;
        }
        return fib(m-1) + fib(m-2);
    }
    i := 1;
    loop (i < 14) {
        print fib(i);
        i := i + 1;
    }

    
