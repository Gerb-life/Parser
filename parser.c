/**
* parser.c - recursive descent parser for a simple expression language.
* Most of the functions in this file model a non-terminal in the
* grammar listed below
* @author William Kreahling, Mark Holliday, Josiah Cherbony, 
* and Gabriel Rodriguez
* @version Modified 9-29-08 and 3-25-15 and 14 april 2020 and 4/8/2022
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "tokenizer.h"
#include "parser.h"

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

char extern *line;
char* error_message = "";

/**
* <expr> -> <term> <ttail>
* The function for the non-terminal <expr> that views
* the expression as a series of terms and addition and
* subtraction operators.
* @return: the number of the evaluated expression or an error
*/
int expr(){
   int total;
   int subtotal = term();
   if (subtotal == ERROR){
      total = ERROR;
   }else{
      total = ttail(subtotal);
      if(strlen(line) == 1 && strcmp(error_message, "") == EQUAL){
         if(';' != *line){
            error_message = "===> ';' expected\n";
            total = ERROR;
         }
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
* @param subtotal: the number we have evaluated up to this
*                  point
* @return: the number of the evaluated expression or an error
*/
int ttail(int subtotal){
   int term_value;
   if (!strncmp(line, "+", 1)){
      //Gets the "+" token
      get_token(*line);
      term_value = term();
      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR)
         return term_value;
      else
         return ttail((subtotal + term_value));
   } 
   else if(!strncmp(line, "-", 1)){
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
* @brief Checks to see is the next token in the input is a comparison token. If
* so, we get the next term and compare is to the subtotal. If not, we return the
* subtotal unmodified. 
* @param subtotal the number we have evaluated up to this point 
* @return subtotal. Or ERROR is the term value we're compating subtotal to is an
* ERROR value. 
*/
int ftail(int subtotal){
   int term_value;
   char* compare_tok = "<><=>===";

   //If the line we're reading is contained in the compare_tok string above.
   if(strchr(compare_tok, *line) != FALSE){
      //Gets the next comparison token 
      char* token = get_token(*line);
      term_value = factor();
      //If term returned an error , give up otherwise call ftail.
      if(term_value == ERROR){
         return term_value;
      }else if(!strncmp(token , "<",1)){

         return ftail((subtotal < term_value));

      } else if(!strncmp(token , ">" , 1)){

         return ftail((subtotal > term_value));

      }else if(!strncmp(token , ">" , 1)){

         return ftail((subtotal > term_value));

      }else if(!strncmp(token , "<=" , 1)){

         return ftail((subtotal <= term_value));

      }else if(!strncmp(token , ">=" , 1)){

         return ftail((subtotal >= term_value));
      }
      else if(!strncmp(token , "==" , 1)){

         return ftail((subtotal == term_value));

      }else{

         return ERROR;
      }
   }else{
      return subtotal;
   }
}

/**
* <stail> ::=  <mult_div_tok> <stmt> <stail> | e
* @brief Checks to see is the next token in the input is a multiply or divide 
* token. If so, we get take the subtotal and multiply or divide it by the next 
* term. If not, we return the subtotal unmodified. 
* @param subtotal the number we have evaluated up to this point 
* @return subtotal. Or ERROR is the term value we're compating subtotal to is an
* ERROR value. 
*/
int stail(int subtotal){
   int term_value;
   if (!strncmp(line, "/", 1)){
      //Gets the "/" token
      get_token(*line);
      term_value = term();
      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR)
         return term_value;
      else
         return stail((subtotal / term_value));
   }
   else if(!strncmp(line, "*", 1)){
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
* @brief Sends the character the cursor's point to to stmt(). If the return
* value is not an error, we send that subtotal to stail().
* @return subtotal. Either an error value or the subtotal modified by ftail.
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
* @brief Checks to see if the cursor is pointing to a "(" character. If it is,
* we get the value generated by expr(). If the next character is not ")", there
* we return an ERROR value. If the next character isn't "(", we call num();
* @return subtotal. the number we have evaluated up to this point.
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
* @brief Sends the character the cursor's point to to factor. If the return
* value is not an error, we send that subtotal to ftail.
* @return subtotal. Either an error value or the subtotal modified by ftail.
*/
int stmt(){
  int subtotal = factor();
  if(subtotal == ERROR){
     return subtotal;
  }else{
     return ftail(subtotal);
  }
}

/**
* <factor> ::=  <expp> ^ <factor> | <expp>
* @brief Checks to see if the cursor is pointing to a "^" character. If it is,
* we raise the current subtotal to the next factor and return that as a int. If
* not, we just return the subtotal unmodified.
* @return subtotal or num. Either the subtotal unmodified, or "num", which is 
* subtotal raised to a power. 
*/
int factor(){
  int subtotal = expp();
   if (!strncmp(line, "^", 1)){
     get_token(*line);
     int pwr = factor(line);
     int num = pow(subtotal, pwr);
     return num;
   }else{
     return subtotal;
   }
}
 
/**
* <num> ::=  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
* @brief Confirms that the cursor is currently pointing to a number. If it
* is a number, the character is converted to an int and returned. If the
* character is not a number, we return an error value, and give error_message
* a message showing that the current token is not a lexeme.
* @return num. The int representation of the number in 'line'.
*/
int num(){
  int num = 0;
  if(isdigit(*line) != FALSE){
     char* number = get_token(*line);
     num = atoi(number);
  }else{
     num = ERROR;
     char* token = get_token(*line);
     strcat(error_message, "===> ");
     strcat(error_message, token);
     strcat(error_message, "\nLexical Error: not a lexeme\n");
  }
  return num;
}




