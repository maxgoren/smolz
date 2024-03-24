# Smolz

Smolz is a small (get it?), simple, toy programming language
meant to show the basics of lexing/parsing, AST construction,
and to a lesser extent, interpretation.

The grammar is purposefully small and easily parsed. 
Smolz has variables that are declared by use, with the only
supported type being reals (single precision floating point numbers).
Smolz has both conditional branching and iteration through if statements
and the 'loop' keyword which works just like a while loop in C.

    prev := 0;
    curr := 1;
    next := 1;
    i := 0;
    loop (i < 13) {
        next := (prev + curr);
        prev := curr;
        curr := next;
        print next;
        i := i + 1;
    }