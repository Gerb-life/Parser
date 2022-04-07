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
int expr(){
 int total;
 int subtotal = term();
 if (subtotal == ERROR){
    total = ERROR;
 }else{
    total = ttail(subtotal);
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
int ttail(int subtotal)
{
  int term_value;
 
  if (!strncmp(line, "+", 1))
  {
     //Gets the "+" token
     get_token(*line);
     term_value = term();
 
     // if term returned an error, give up otherwise call ttail
     if (term_value == ERROR)
        return term_value;
     else
        //token = get_token(*line);
        return ttail((subtotal + term_value));
  }
  else if(!strncmp(line, "-", 1))
  {
     //Gets the "-" token
     get_token(*line);
     term_value = term();
 
     // if term returned an error, give up otherwise call ttail
     if (term_value == ERROR)
        return term_value;
     else
        return ttail((subtotal - term_value));
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
int ftail(int subtotal){
      int term_value;
      if(!strncmp(line , "<",1)){
         //Gets the "<" token
         get_token(*line);
         term_value = factor();
         //if term returned an error , give up otherwise call ftail.
         if(term_value == ERROR){
            return term_value;
         }
         else{
            return ftail((subtotal < term_value));
         }
      }
      else if(!strncmp(line , ">" , 1)){
         //Gets the ">" token
         get_token(*line);
         term_value = factor();
         if(term_value == ERROR){
            return term_value;
         }
         //if term returned an error , give up otherwise call ftail
         else{
            return ftail((subtotal > term_value));
         }
      }
      else if(!strncmp(line , "<=" , 1)){
         //Gets the "<" token
         get_token(*line);
         term_value = factor();
         if(term_value == ERROR){
            return term_value;
         }
         //if term returned an error , give up otherwise call ftail
         else{
            //token = move_cursor(FORWARDS, TRUE);
            return ftail((subtotal <= term_value));
         }
      }
      else if(!strncmp(line , ">=" , 1)){
         //Gets the ">=" token
         get_token(*line);
         term_value = factor(line);
 
         if(term_value == ERROR){
            return term_value;
         }
         else{
            return ftail((subtotal >= term_value));
         }
      }
      else if(!strncmp(line , "==" , 1)){
         //Gets the "==" token
         get_token(*line);
         term_value = factor();
         //if term returned an error , give up otherwise call ftail
         if(term_value == ERROR){
            return term_value;
         }
         else{
            return ftail((subtotal == term_value));
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
int stail(int subtotal){
 int term_value;
 if (!strncmp(line, "/", 1))
 {
   //Gets the "/" token
   get_token(*line);
   term_value = term();
   // if term returned an error, give up otherwise call ttail
   if (term_value == ERROR)
      return term_value;
   else
      return stail((subtotal / term_value));
 }
 else if(!strncmp(line, "*", 1))
 {
   //Gets the "*" token
   get_token(*line);
   term_value = term(line);
   // if term returned an error, give up otherwise call ttail
   if (term_value == ERROR)
      return term_value;
   else
      return stail((subtotal * term_value));
 }
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
int term(){
 int subtotal = stmt();
 if (subtotal == ERROR)
    return subtotal;
 else
    return stail(subtotal);
}
 
/**
* <expp> ::=  ( <expr> ) | <num>
* @brief
*
* @return int
*/
int expp(){
int subtotal;
if (!strncmp(line, "(", 1)){
    get_token(*line);
    subtotal = expr();
    char* token = get_token(*line);
    if(strcmp(")", token) != 0){
       error_message = "===> ')' expected\nSyntax error\n";
       subtotal = ERROR;
    }
}else{
   subtotal = num();
}
return subtotal;
}
 
 
/**
* <stmt> ::=  <factor> <ftail>
* @brief
*
* @return int
*/
int stmt(){
int subtotal = factor(line);
  if(subtotal == ERROR){
     return subtotal;
  }else{
     //token = move_cursor(FORWARDS, TRUE);
     return ftail(subtotal);
  }
}
 
/**
* <factor> ::=  <expp> ^ <factor> | <expp>
* @brief
*
* @param token
* @return int
*/
int factor(){
 int subtotal = expp();
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
* <num> ::=  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
* @brief
*
* @param number
* @return int
*/
int num(){
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
   //error_message = malloc(TSIZE);
   error_message = message;
   free(message);
}
return num;
}
