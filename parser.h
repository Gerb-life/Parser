#ifndef PARSER_H
   #define PARSER_H
   #endif
/*
 * Author:  William Kreahling and Mark Holliday
 * Purpose: Function Prototypes for parser.c
 * Date:    Modified 9-26-08, 3-25-15
 */
int bexpr(char *);	// bexpr is short for boolean_expression
int expr();     // expr is short for expression
int term();
int ttail(int);       // ttail is short for term_tail
int stmt();
int stail(int);      // stail is short for statement_tail
int factor();
int ftail(int);	// ftail is short for factor_tail
int expp();     // expp is short for exponentiation
int num();


#define ERROR -99999
#define FORWARDS 1
#define BACKWARDS 0
#define TRUE 1
#define FALSE 0
#define EQUAL 0