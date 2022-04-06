#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "tokenizer.h"
#include <stdlib.h>

extern char* line;
extern char* error_message;

int main(int argc , char* argv[]){
    char input_line [100];
    char *skipable = "\n\t\r";
    int running = TRUE;
    FILE *in_file = fopen(argv[1], "r");
    if (in_file == NULL) {
        fprintf(stderr, "ERROR: could not open %s for reading\n", argv[1]);
        exit(1);
    }
    FILE *out_file = fopen(argv[2] , "w");
    if(out_file == NULL){
        fprintf(stderr, "ERROR: could not open %s for reading\n", argv[2]);
        exit(1);
    }

     while (fgets(input_line, 100, in_file) != NULL){
            line = input_line;
            if(strlen(line) != 1){

            int num = expr(line);
        
            fprintf(out_file , "%s" , input_line);
            if(num == ERROR){
                //fprintf(out_file , "Lexical Error not a lexeme:\n\n ");
                fprintf(out_file, "%s\n", error_message);
                error_message = "";
                /**
                if(strstr(error_message, "Lexical") != NULL){
                    free(error_message);
                }
                */
            }
            else{
                fprintf(out_file , "Syntax is ok\n");
                fprintf(out_file , "Value is: %d\n\n" , num);
             }

            }

     }
}

//gcc -c interpreter.c -g 
//gcc parser.c interpreter.o -g 
//./a.out unix_input.txt 101.txt