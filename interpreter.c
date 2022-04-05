#include <stdio.h>
#include "parser.h"
#include "tokenizer.h"
#include <stdlib.h>

extern char* line;
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

        int num = expr(line);
        fprintf(out_file , "%d\n" , num);


     }


}