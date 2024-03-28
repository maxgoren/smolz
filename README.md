# Owlscript

With Lexical scoping, dynamic typing, first class functions, recusrion, and built in lists,
you'd be forgiven for assuming Smolz is yet another lisp, But with a syntax 
similar to C and the overall 'feel' of python, Owlscript has something for everyone.

Owlscript has a terse grammar, where variables are declared by use - no 'let' or 'var' keywords. 
'if' and 'loop' statements allow for easy control flow of Owlscript programs.

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

    
Lists are a built in data type in owlscript, with wrtie access at both ends, it can be used as a stack
or queue through the use of the push/pop/append statements, and indexed access is also supported.
