SKInny
======

An interpreter for the SKInny language.

Compiling
---------
While in the `source` directory, type `make` to compile the code.

Running
-------
The interpreter should be run as
	./interpreter [-s] [filename]

If the `-s` flag is used, the interpreter will simply print out all
evaluate statements with all macros expanded. Otherwise, the normal
behavior of the interpreter is to actually evaluate all evaluate
statements. If a filename is provided, the program is read from that file;
otherwise stdin is used.

SKInny
------
The SKInny language is a variation on the Unlambda language. Just like
Unlambda, SKInny is built on top of the SKI calculus. There are three major
differences between SKInny and Unlambda:
1. Function application is written in the more traditional `f(x)` form, rather
   than ```fx``. Chained application becomes `f(x)(y)`, rather than `\`\`fxy`. 
2. SKInny allows for macros to be defined for use in later expressions.
3. SKInny uses call-by-need semantics, as opposed to Unlambda's call-by-value
   semantics.

A SKInny program consists of semicolon-separated statements. There are two
types of statements in SKInny:
1. Macro definitions, of the form `name expr;`. A macro definition allows
   "name" to be used in place of "expr" in later statements.
2. Evaluate statements, of the form `expr;`. The interpreter simply evaluates
   "expr".
