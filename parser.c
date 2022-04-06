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

//char *line; 
char* error_message = "";

/**
 * <expr> -> <term> <ttail>
 * The function for the non-terminal <expr> that views
 * the expression as a series of terms and addition and
 * subtraction operators.
 * @param token: the line being read
 * @return: the number of the evaluated expression or an error
 */
int expr(char *token){
  int total;
  int subtotal = term(line);
  if (subtotal == ERROR){
     total = ERROR;
  }else{
     total = ttail(line, subtotal);
     if(strlen(line) == 1 && strcmp(error_message, "") == 0){
        error_message = "===> ';' expected\n";
        total = ERROR;
     }
  }
 
  return total;
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
int ttail(char *token, int subtotal)
{
   int term_value;

   if (!strncmp(line, "+", 1))
   {
      add_sub_tok(line);
      term_value = term(line);

      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR)
         return term_value;
      else
         //token = get_token(*line);
         return ttail(line, (subtotal + term_value));
   }
   else if(!strncmp(line, "-", 1))
   {
      add_sub_tok(line);
      term_value = term(line);

      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR)
         return term_value;
      else
         //token = get_token(*line);
         return ttail(line, (subtotal - term_value));
   }
   /* empty string */
   else
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
 
       if(!strncmp(line , "<",1)){
         //Reads in the next comparison token 
         compare_tok(line);
         term_value = factor(line);
 
         //if term returned an error , give up otherwise call ftail.
         if(term_value == ERROR){
            return term_value;
         }
         else{
             //token = move_cursor(FORWARDS, TRUE);
            return ftail(line , (subtotal < term_value));
         }
       }
 
      else if(!strncmp(line , ">" , 1)){
         compare_tok(line);
         term_value = factor(line);
 
         if(term_value == ERROR){
            return term_value;
         }
         //if term returned an error , give up otherwise call ftail
          else{
            //token = move_cursor(FORWARDS, TRUE);
            return ftail(line, (subtotal > term_value));
         }
       }
 
       else if(!strncmp(line , "<=" , 1)){
         compare_tok(line);
         term_value = factor(line);
 
         if(term_value == ERROR){
            return term_value;
         }
         //if term returned an error , give up otherwise call ftail
         else{
            //token = move_cursor(FORWARDS, TRUE);
            return ftail(line , (subtotal <= term_value));
         }
 
       }
       else if(!strncmp(line , ">=" , 1)){
         compare_tok(line);
         term_value = factor(line);

         if(term_value == ERROR){
            return term_value;
         }
         else{
            //token = move_cursor(FORWARDS, TRUE);
            return ftail(line , (subtotal >= term_value));
         }
       }
       else if(!strncmp(line , "==" , 1)){
         compare_tok(line);
         term_value = factor(line);//Maybe change back to term
 
         //if term returned an error , give up otherwise call ftail
         if(term_value == ERROR){
            return term_value;
         }
         else{
            //token = move_cursor(FORWARDS, TRUE);
            return ftail(line , (subtotal == term_value));
         }
       }
       else{
               return subtotal;
       }
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
      mul_div_tok(line);
      term_value = term(line);
 
     // if term returned an error, give up otherwise call ttail
     if (term_value == ERROR)
        return term_value;
     else
        //Move the cursor up
        //token = move_cursor(FORWARDS, TRUE);
        return stail(line, (subtotal / term_value));
  }
  else if(!strncmp(token, "*", 1))
  {
      mul_div_tok(line);
      term_value = term(line);
 
     // if term returned an error, give up otherwise call ttail
     if (term_value == ERROR)
        return term_value;
     else
        //Move the cursor up
        //token = move_cursor(FORWARDS, TRUE);
        return stail(line, (subtotal * term_value));
  }
  else
     return subtotal;
}
 

/**
* <add_sub_tok> ::=  + | -
* @brief
*
*/
void add_sub_tok(char* token){
  char* toks = "+-";
  token = get_token(*line);
  if(strchr(toks, token) != NULL){
     //Don't do anything 
  }
}
 
/**
* <term> ::=  <stmt> <stail>
*
* @brief
* @param token
* @return int
*/
int term(char* token){
  int subtotal = stmt(line);
  if (subtotal == ERROR)
     return subtotal;
  else
     return stail(line, subtotal);
}

/**
* <expp> ::=  ( <expr> ) | <num>
* @brief
*
* @return int
*/
int expp(char* token){
 int subtotal;
 if (!strncmp(line, "(", 1)){
     get_token(*line);
     subtotal = expr(line);
     token = get_token(*line);
     if(strcmp(")", token) != 0){
        error_message = "===> ')' expected\nSyntax error\n";
        subtotal = ERROR;
     }
 }else{
    subtotal = num(line);
 }
 return subtotal;
}


/**
* <stmt> ::=  <factor> <ftail>
* @brief
*
* @return int
*/
int stmt(char* token){
int subtotal = factor(line);
   if(subtotal == ERROR){
      return subtotal;
   }else{
      //token = move_cursor(FORWARDS, TRUE);
      return ftail(line, subtotal);
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
  int subtotal = expp(line);
  //token = move_cursor(FORWARDS, TRUE);
  if (!strncmp(line, "^", 1)){
     get_token(*line);
     //token = move_cursor(FORWARDS, TRUE);
     int pwr = factor(line);
     int num = pow(subtotal, pwr);
     return num;
  }else{
     //move_cursor(BACKWARDS, FALSE);
     return subtotal;
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
       token = get_token(*line);
       if(strchr(toks, token) != NULL){
       //Don't do anything 
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
      token = get_token(*line);
      if(strchr(toks, token) != NULL){
     //Don't do anything 
      }
}
 
 /**
* <num> ::=  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
* @brief
*
* @param number
* @return int
*/
int num(char* token){
 int num = 0;
 if(isdigit(*line) == TRUE){
    char* number = get_token(*line);
    num = atoi(number);
 }else{
    num = ERROR;
    char* token = get_token(*line);
    char* message = malloc(TSIZE);
    strcat(message, "===> ");
    strcat(message, token);
    strcat(message, "\nLexical Error: not a lexeme\n");
    error_message = malloc(TSIZE);
    error_message = message;
    free(message);
 }
 return num;
}

 