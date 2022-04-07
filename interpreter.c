#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "tokenizer.h"
#include <stdlib.h>
/**
 *interpreter.c - driver for parser.c and tokenizer.c 
 *Authors Josiah Cherbony  and Gabriel Rodriguez
 *Date 4/7/2022
 */
 
extern char* line; // global pointer to external line
extern char* error_message; // global pointer to external error message
 /**
  * @brief 
  *  This is the driver for parser.c and tokenizer.c it handles printing to the 
  *  designated the statement , result and errors (if any) to the designated outputfile
  * @param argc - counter for command line arguments
  * @param argv - array of command line arguments in this case files to read from and write to 
  * @return - returns 0 just to show that everything completed
  */
int main(int argc , char* argv[]){
   char input_line [100];  //buffer to hold characters from input file
   
   //if input file is null print error message to terminal and exit
   FILE *in_file = fopen(argv[1], "r");
   if (in_file == NULL) {
       fprintf(stderr, "ERROR: could not open %s for reading\n", argv[1]);
       exit(1);
   }
   //if output file is null print error message to terminal and exit
   FILE *out_file = fopen(argv[2] , "w");
   if(out_file == NULL){
       fprintf(stderr, "ERROR: could not open %s for reading\n", argv[2]);
       exit(1);
   }
    
    // while input line is not null 
    while (fgets(input_line, 100, in_file) != NULL){
           line = input_line; // single line of input
           error_message = malloc(MALLSIZE); // create space on the stack for the error message
           // checking to see if line has more than single \n character 
           if(strlen(line) != 1){
 
           int num = expr(line); // result from expr funciton in parser.c
      
           fprintf(out_file , "%s" , input_line); // printing expression   
           // if num is equal to error print error message
           if(num == ERROR){
              
               fprintf(out_file, "%s\n\n", error_message);// printing error message 
               error_message = ""; // resetting error message
              
           }
           // if num is legal and syntax is legal print to output
           else{
               fprintf(out_file , "Syntax is ok\n"); // print if syntax is legal
               fprintf(out_file , "Value is: %d\n\n" , num);// print value if value is legal
            }
 
           }
    
    }
    free(error_message);// free space on stack for error message
    return 0;
}
 
//gcc -c interpreter.c -g
//gcc parser.c interpreter.o -g -lm
//./a.out unix_input.txt 101.txt