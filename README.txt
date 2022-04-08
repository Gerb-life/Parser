Authors: Josiah Cherbony and Gabriel Rodriguez

Date 4/7/2022

This program will perform lexical and syntax analysis for a given language and grammer. Characters that are not apart of this language will be caught durring 
the initial lexical analysis phase of runtime while syntax errors will be caught durring the parsing phase of runtime. If a statement passes both the lexical and syntax
processes it will then be evaluated this includes comparison operation , addition/ subtraction operations , and multiplication/division operations. The whole process of printing the 
statement , catching errors , and evaluating the result will be shown in the output file after runtime.


How to run.

In the command line the user will type:
    "gcc -c interpreter.c -g "
    "gcc parser.c interpreter.o -g -lm"
    "a.out  unix_input.txt  output.txt"

Where a.out is the name of the executable , unix_input.txt is the name of the input file , and output.txt is the name of the output file.

Known Issues:

In parser.c we are using tokenizer.c in our #includes 
