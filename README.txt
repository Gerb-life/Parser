=== CS 352 Project 1 ===
Author: Josiah Cherbony and Gabe Rodriguez
Date of Submission: 4/8/2022

=== HOW TO COMPILE PROGRAM ===

To compile the this program, the user must enter “gcc -c interpreter.c tokenizer.c -g”
into the terminal, then "gcc parser.c interpreter.o tokenizer.o -g -lm " 
afterward. The executable file will be named “a.out”.

=== HOW TO RUN PROGRAM ===

To run “a.out” the user must enter “./a.out argv[1] argv[2]”, with argv[1]
representing the name of the text file that contains the tokens to be parsed
(which must be formatted as “<file_name>.txt”), and argv[2] representing the 
name of the outputted text file that will contain the statement identifying
our tokens (which must be formatted as “<file_name>.txt”).

=== ERRORS/BUGS ===

As of the time of writing this, there are no known bugs or errors in the
tokenizer.c program.

=== OTHER NOTES ===

The only other thing to note is that by the end of the project, we only used the
global string "line" in our functions, so we got rid of any instance of any 
"token" paramters in the functions.