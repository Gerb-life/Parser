/*
 * parser.c - recursive descent parser for a simple expression language.
 * Most of the functions in this file model a non-terminal in the
 * grammar listed below
 * Author: William Kreahling and Mark Holliday
 * Date:   Modified 9-29-08 and 3-25-15 and 14 april 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
//#include "tokenizer.h"
#include "parser.h"

//////TAKE THIS OUT LATER
#include "tokenizer.c"

extern char *line;
int running = TRUE;


/*
 * <bexpr> ::= <expr> ;
 * <expr> ::=  <term> <ttail>
 * <ttail> ::=  <add_sub_tok> <term> <ttail> | e
 * <term> ::=  <stmt> <stail>
 * <stail> ::=  <mult_div_tok> <stmt> <stail> | e
 * <stmt> ::=  <factor> <ftail>
 * <ftail> ::=  <compare_tok> <factor> <ftail> | e
 * <factor> ::=  <expp> ^ <factor> | <expp>
 * <expp> ::=  ( <expr> ) | <num>
 * <add_sub_tok> ::=  + | -
 * <mul_div_tok> ::=  * | /
 * <compare_tok> ::=  < | > | <= | >= | != | ==
 * <num> ::=  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
 */

/**
 * <expr> -> <term> <ttail>
 * The function for the non-terminal <expr> that views
 * the expression as a series of terms and addition and
 * subtraction operators.
 * @param token: the line being read
 * @return: the number of the evaluated expression or an error
 */
int expr(char *token){
   int subtotal = term(token);
   if (subtotal == ERROR)
      return subtotal;
   else
      //Move the cursor up
      token = move_cursor(FORWARDS);
      return ttail(token, subtotal);
}

/**
 * <ttail> -> <add_sub_tok> <term> <ttail> | e
 * The function for the non-terminal <ttail> that is the
 * the rest of an arithmetic expression after the initial
 * term. So it expects an addition or subtraction operator
 * first or the empty string.
 * @param token: the line being read
 * @param subtotal: the number we have evaluated up to this
 *                  point
 * @return: the number of the evaluated expression or an error
 */
int ttail(char *token, int subtotal){
   int term_value;

   if (!strncmp(token, "+", 1))
   {
      add_sub_tok(token);
      //Move the cursor up
      token = move_cursor(FORWARDS);
      term_value = term(token);

      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR)
         return term_value;
      else
         //Move the cursor up
         token = move_cursor(FORWARDS);
         return ttail(token, (subtotal + term_value));
   }
   else if(!strncmp(token, "-", 1))
   {
      add_sub_tok(token);
      //Move the cursor up
      token = move_cursor(FORWARDS);
      term_value = term(token);

      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR)
         return term_value;
      else
         //Move the cursor up
         token = move_cursor(FORWARDS);
         return ttail(token, (subtotal - term_value));
   }
    else if(running == TRUE){
      running = FALSE;
      //Move the cursor up
      token = move_cursor(FORWARDS);
      subtotal = ttail(token, subtotal);
      running = TRUE;
      token = move_cursor(BACKWARDS);
      return subtotal;
   }
   /* empty string */
   else
      return subtotal;
}

/**
 * <term> ::=  <stmt> <stail>
 *
 * @brief
 * @param token
 * @return int
 */
int term(char* token){
   int subtotal = stmt(token);
   if (subtotal == ERROR)
      return subtotal;
   else
      return stail(token, subtotal);
}

/**
 * <stail> ::=  <mult_div_tok> <stmt> <stail> | e
 * @brief
 *
 * @return int
 */
int stail(char* token, int subtotal){
   int term_value;

   if (!strncmp(token, "/", 1))
   {
      mul_div_tok(token);
      //Move the cursor up
      token = move_cursor(FORWARDS);
      term_value = term(token);

      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR)
         return term_value;
      else
         //Move the cursor up
         token = move_cursor(FORWARDS);
         return stail(token, (subtotal / term_value));
   }
   else if(!strncmp(token, "*", 1))
   {
      mul_div_tok(token);
      //Move the cursor up
      token = move_cursor(FORWARDS);
      term_value = term(token);

      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR)
         return term_value;
      else
         //Move the cursor up
         token = move_cursor(FORWARDS);
         return stail(token, (subtotal * term_value));
   }
   /* empty string */
   //Checking the next token to see if it matches with the non-terminal
   else if(running == TRUE){
      running = FALSE;
      //Move the cursor up
      token = move_cursor(FORWARDS);
      subtotal = stail(token, subtotal);
      running = TRUE;
      token = move_cursor(BACKWARDS);
      return subtotal;
   }
   else
      return subtotal;
}

/**
 * <stmt> ::=  <factor> <ftail>
 * @brief
 *
 * @return int
 */
int stmt(char* token){
 int subtotal = factor(token);
 return subtotal;
}

/**
 * <ftail> ::=  <compare_tok> <factor> <ftail> | e
 * @brief
 *
 * @return int
 */
int ftail(char* token, int subtotal){
        int term_value;

        if(!strncmp(token , "<",1)){
                compare_tok(token);
                //move cursor up
                token = move_cursor(FORWARDS);
                term_value = term(token);

                //if term returned an error , give up otherwise call ftail.
                if(term_value == ERROR){
                        return term_value;

                }
                else{
                        token = move_cursor(FORWARDS);
                        return ftail(token , (subtotal < term_value));
                }
        }

        else if(!strncmp(token , ">" , 1)){
                compare_tok(token);
                // move cursor up

                token = move_cursor(FORWARDS);
                term_value = term(token);

                if(term_value == ERROR){
                        return term_value;
                }
                //if term returned an error , give up otherwise call ftail
                else{
                        token = move_cursor(FORWARDS);
                        return ftail(token, (subtotal > term_value));
                }
        }

        else if(!strncmp(token , "<=" , 1)){
                compare_tok(token);
                // move cursor up

                token = move_cursor(FORWARDS);
                term_value = term(token);

                if(term_value == ERROR){
                        return term_value;
                }
                //if term returned an error , give up otherwise call ftail
                else{
                        token = move_cursor(FORWARDS);
                        return ftail(token , (subtotal <= term_value));

                }

        }
        else if(!strncmp(token , ">=" , 1)){
                compare_tok(token);
                // move cursor up

                token = move_cursor(FORWARDS);
                term_value = term(token);
                //if term returned an error , give up otherwise call ftail
                if(term_value == ERROR){
                        return term_value;
                }

                else{
                        token = move_cursor(FORWARDS);
                        return ftail(token , (subtotal >= term_value));
                }
        }
        else if (running == TRUE){
                running = FALSE;
                // move cursor up

                token = move_cursor(FORWARDS);
                subtotal = ftail(token , subtotal);
                running = TRUE;
                token = move_cursor(BACKWARDS);
                return subtotal;
        }

        else{
                return subtotal;
        }
}

/**
 * <factor> ::=  <expp> ^ <factor> | <expp>
 * @brief
 *
 * @param token
 * @return int
 */
int factor(char* token){
   if (!strncmp(token, "^", 1)){
      //do something
   }
   int subtotal = expp(token);
   return subtotal;
}

/**
 * <expp> ::=  ( <expr> ) | <num>
 * @brief
 *
 * @return int
 */
int expp(char* token){

   if (!strncmp(token, "(", 1)){
      //do something
   }
   int subtotal = num(token);
   return subtotal;
}

/**
 * <add_sub_tok> ::=  + | -
 * @brief
 *
 */
void add_sub_tok(char* token){
   char* toks = "+-";
   if(strchr(toks, token) != NULL){
      //do something
   }
}

/**
 * <mul_div_tok> ::=  * | /
 * @brief
 *
 * @param token
 */
void mul_div_tok(char* token){
        char* toks = "*/";
        if(strchr(toks , token) != NULL){
                //do something
        }
}

/**
 * <compare_tok> ::=  < | > | <= | >= | != | ==
 * @brief
 *
 *
 * @param token
 */
void compare_tok(char* token){
        char* toks = "<>!=";
        if(strchr(toks , token) != NULL){
                //do something
        }
}

/**
 * <num> ::=  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
 * @brief
 *
 * @param number
 * @return int
 */
int num(char* number){
   int num = 0;
   if(isdigit(*number) == TRUE){
      num = atoi(number);
   }else{
      num = ERROR;
   }
   return num;
}

///FOR TESTING PURPOSES///////////////////////////
int main(int argc, char* argv[]){
   char input_line[100];
   char *skipable = "\n\t\r";
   int running = TRUE;

   FILE *in_file = fopen("unix_input.txt", "r");//Directs the first argument to fopen

   while (fgets(input_line, 100, in_file) != NULL){
      line = input_line;  // Sets a global pointer to the memory location
                          // where the input line resides
         char* token = get_token(*line);
         int num = expr(token);
         printf("%d\n", num);
      }
}

char* move_cursor(int movement){
   char *skipable = "\n\t\r ";
   if(movement == FORWARDS){
      line++;
      }
   else{
      line--;
   }
   while(strchr(skipable, *line) != NULL){
      if(movement == FORWARDS){
         line++;
      }
      else{
         line--;
      }

   }
   return get_token(*line);
}