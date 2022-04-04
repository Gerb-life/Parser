/**
 * tokenizer.c - A simple token recognizer.
 *
 * NOTE: The terms 'token' and 'lexeme' are used interchangeably in this
 *       program.
 * 
 * @author Josiah Cherbony 
 * @version 2/19/2022
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tokenizer.h"
#include <ctype.h>

// global variables

/**Global pointer to line of input*/
extern char *line;

/** The file that will hold our output*/
FILE  *out_file;

/** All possible tokens that can make up a lexeme with two characters*/
char *two_char_lexeme = "<>=!";

/** All possible tokens that can be skipped over*/
char *skipable = "\n\t\r";

/**
 * @brief The main function that creates a file object from the text file 
 * specified in argument 1. The tokens in the text file are then identified and
 * categorized, with the output being sent to a file specifed in argument 2.
 * @param argc The number of arguments given.
 * @param argv An array containing the arguments inputed. 
 * @return int. 0 if no errors occur.
 */
/**
int main(int argc, char* argv[]) {
   char  input_line[LINE];
   FILE  *in_file = NULL; 
   out_file = NULL;
   int line_count, Number of lines read             
      start, is this the start of a new statement? 
      count; count of tokens
    

  in_file = fopen(argv[1], "r");//Directs the first argument to fopen

  if(in_file == NULL){//If the input is null, print an error message and exit
    fprintf(stderr, "ERROR: couldn't open %s for reading\n", argv[1]);
    exit(1);
  }

  if (argc != 3) {
    printf("Usage: tokenizer inputFile outputFile\n");
    exit(1);
  }

  in_file = fopen(argv[1], "r");
  if (in_file == NULL) {
    fprintf(stderr, "ERROR: could not open %s for reading\n", argv[1]);
    exit(1);
  }

  out_file = fopen(argv[2], "w");

  if (out_file == NULL) {
    fprintf(stderr, "ERROR: could not open %s for writing\n", argv[2]);
    exit(1);
  }
  
   parse_file(in_file, input_line);

   fclose(in_file);
   fclose(out_file);
   return 0;
   
}
*/

/**
 * @brief Takes one line of input from the file, sends it to the build_token 
 * function, which builds the individual tokens that are sent to the get_token
 * function.
 * @param in_file The file the be read from.
 * @param input_line An empty char array that holds one line from the input file
 */
void parse_file(FILE* in_file, char input_line[]){
  while (fgets(input_line, LINE, in_file) != NULL){
      line = input_line;  // Sets a global pointer to the memory location
                          // where the input line resides

      while(*line != '\0'){//While we haven't reached the end of the linex

        /** If the charcter (*line) is in the skipable catagory or whitespace,
         * we skip over it*/
        if(strchr(skipable, *line) == NULL && *line != ' '){
          char* lexeme = malloc(TSIZE);
          lexeme = get_token(*line);//Builds a lexeme
          build_statement(lexeme);//Lexeme is sent to get_token
          free(lexeme);
        }
        line++;//Go to the next character in the line 
      }    
   }
}

/**
 * @brief Builds a token based on the input given. If the input given is a 
 * number or a character that could potentially be a two character lexeme ('==',
 * '!=', '<=', etc.) we look at the next character in the global variable 'line'
 * to see if the next token is either a nunber (making it a multidigit number) 
 * or a token that would make the lexeme a two character lexeme.
 * @param token. A character that makes up a token 
 * @return char_holder. A holder for out token 
 */
char* get_token(char token){
  char *skipable = "\n\t\r ";
  char* char_holder = malloc(TSIZE);
  int char_counter = 0;

  char_holder[char_counter] = token;
  line++;//UPDATED
  char_counter++;

  //If the character is a digit, it could be a multidigit number, so we check to
  //see if the next lexeme is also a number.
  if(isdigit(*line) && isdigit(*char_holder)){
    while(isdigit(*line) == TRUE){
      char_holder[char_counter] = *line;
      line++;
    }
  }
  //If the character is a lexeme that COULD be a lexeme made up of 2 characters,
  //we check to see if adding the next character in the line produces that type
  //of lexeme. 
  else if(strchr(two_char_lexeme, *char_holder) != NULL && strchr(two_char_lexeme, *line) != NULL){
    char_holder[char_counter] = *line;
    line++;
  }
  while(strchr(skipable, *line) != NULL){
    line++;
  }
  
  return char_holder;
}

/**
* This function accepts a token and builds a statement (or adds to an existing 
* statement) that identifies if the token is a legal, and the catagory of the 
* token. This statement is added to the global variable 'out_file'.
*/
void build_statement(char *token_ptr){
  static int new_statement = TRUE; 
  static int lexeme_count = 0; 
  char* statement = malloc(OUTPUT);
  char* line = malloc(LINE);
  char* token_identifier = malloc(STATSIZE);
  char* new_statement_line = malloc(LINE);
  static int statement_count = 1;
  /**Gets a statement identifying the token*/
  token_identifier = get_token_identifier(token_ptr);

  /**If we're beginning a new statement*/
  if(new_statement == TRUE){
    new_statement = FALSE;
    new_statement_line = make_new_statement(statement_count); 
    strcat(statement, new_statement_line);
    statement_count++;
  }
  /**If the token doesn't have the identity "UNIDENTIFIABLE"*/
  if(strcmp(token_identifier, "UNIDENTIFIABLE") != FALSE){
    line = statement_maker(lexeme_count, token_ptr);
    strcat(statement, line);
    strcat(statement, token_identifier);
    strcat(statement, "\n");
    lexeme_count++;

    /**If we've reached the end of a statement*/
    if(*token_ptr == ';'){
      lexeme_count = 0;
      new_statement = 1;//The time get_token is called, we begin a new statement 
      char* end = "---------------------------------------------------------\n";
      strcat(statement, end);
    }
  /**If the token is UNIDENTIFIABLE*/
  }else{
    line = error_maker(token_ptr);
    strcat(statement, line);
  }
  fprintf(out_file, "%s", statement);
  free(statement);
  free(token_identifier);
  free(line);
}

/**
 * @brief Builds a statement that includes the category of the inputted token.
 * @param token The token to be identified.
 * @return statement
 */
char* get_token_identifier(char* token){
  char* statement = malloc(STATSIZE);
  strcat(statement, " and is a ");

  if(strcmp(token, "+") == FALSE){
    strcat(statement, "ADD_OP");
  }
  else if(strcmp(token, "-") == FALSE){
    strcat(statement, "SUB_OP");
  }
  else if(strcmp(token, "*") == FALSE){
    strcat(statement, "MULT_OP");
  }
  else if(strcmp(token, "/") == FALSE){
    strcat(statement, "DIV_OP");
  }
  else if(strcmp(token, "(") == FALSE){
    strcat(statement, "LEFT_PAREN");
  }
  else if(strcmp(token, ")") == FALSE){
    strcat(statement, "RIGHT_PAREN");
  }
  else if(strcmp(token, "^") == FALSE){
    strcat(statement, "EXPON_OP");
  }
  else if(strcmp(token, "=") == FALSE){
    strcat(statement, "ASSIGN_OP");
  }
  else if(strcmp(token, "<") == FALSE){
    strcat(statement, "LESS_THAN_OP");
  }
  else if(strcmp(token, "<=") == FALSE){
    strcat(statement, "LESS_THAN_OR_EQUAL_OP");
  }
  else if(strcmp(token, ">") == FALSE){
    strcat(statement, "GREATER_THAN_OP");
  }
  else if(strcmp(token, ">=") == FALSE){
    strcat(statement, "GREATER_THAN_OR_EQUAL_OP");
  }
  else if(strcmp(token, "==") == FALSE){
    strcat(statement, "EQUALS_OP");
  }
  else if(strcmp(token, "!") == FALSE){
    strcat(statement, "NOT_OP");
  }
  else if(strcmp(token, "!=") == FALSE){
    strcat(statement, "NOT_EQUALS_OP");
  }
  else if(strcmp(token, ";") == FALSE){
    strcat(statement, "SEMI_COLON");
  }
  else if(isdigit(*token) != FALSE){
    strcat(statement, "INT_LITERAL");
  }
  else{
    strcpy(statement, "UNIDENTIFIABLE");
  }
  return statement;
}

/**
 * @brief Used when making a new statement (the beginning of a new line of 
 * input).
 * @param statement_count The number statements made.
 * @return statement
 */
char* make_new_statement(int statement_count){
  char* statement = malloc(MALLSIZE);
  char stat_count_str[4];
  strcat(statement, "Statement #");
  sprintf(stat_count_str, "%d", statement_count); 
  strcat(statement, stat_count_str);
  strcat(statement, "\n");
  return statement;
}

/**
 * @brief Used to make statments if the token is of the catagory UNIDENTIFIABLE.
 * @param token The token that is UNIDENTIFIABLE. 
 * @return statement 
 */
char* error_maker(char* token){
  char* statement = malloc(MALLSIZE);
  strcat(statement, "===> ");
  strcat(statement, token);
  strcat(statement, "\n");
  strcat(statement, "Lexical error: not a lexeme\n");
  return statement;
}

/**
 * @brief Makes a statement that gives the token and the token's identity
 * @param token_count The token's number in the statement 
 * @param lexeme 
 * @return statement
 */
char* statement_maker(int token_count, char* lexeme){
  char* statement = malloc(MALLSIZE);
  char count_str[4];
  strcat(statement, "Lexeme ");
  sprintf(count_str, "%d", token_count); 
  strcat(statement, count_str);
  strcat(statement, " is ");
  strcat(statement, lexeme);

  return statement;
}

