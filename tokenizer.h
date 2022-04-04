#include <stdio.h>

/**
 * @author Josiah Cherbony 
 * @version 3/19/2022
 * @file tokenizer.h
 * This header file is intended for the program 'tokenizer.c' and 
 * includes it's named constants and function prototypes.
 */


///////////////////////NAMED CONSTANTS//////////////////////////////////////////
#define LINE 100
#define OUTPUT 1000
#define TSIZE 20
#define STATSIZE 30
#define MALLSIZE 40
#define TRUE 1
#define FALSE 0

///////////////////////FUNCTION PROTOTYPES//////////////////////////////////////

void build_statement(char*);  
char* get_token(char);
char* get_token_identifier(char* token);
char* statement_maker(int count, char* lexeme);
char* make_new_statement(int count);
char* error_maker(char* token);
void parse_file(FILE* in_file, char input_line[]);
void two_char_lexeme_builder(char*, int); 
