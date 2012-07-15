/* Parser for smpl */

%{

#pragma once
// Includes
#include <string.h>

#include "lex.h"     // the lexer
#include "treenode.h" // the syntax tree

extern SyntTree tree;

// Yacc (bison) defines
#define YYDEBUG 1       // Generate debug code; needed for YYERROR_VERBOSE
#define YYERROR_VERBOSE // Give more specific parse error messages

// Error-reporting function must be defined by the caller
void error(char *format, ...);
// Forward references
void yyerror(const char *msg);
char *make_name();

%}
%glr-parser
/* -----------------------------------
   Yacc declarations
   -----------------------------------*/

%union {
  char     *str;     // a char string
  int       num;     // a number
  rd_tree_node *tnode;   // node in the syntax tree
}

/* Token definitions */
%token ERROR_TOKEN IF ELSE PRINT INPUT ASSIGN EQUAL
%token CONCAT END_STMT OPEN_PAR CLOSE_PAR
%token BEGIN_CS END_CS DEF THEN END COMMA RETURN
%token <str> ID STRING
%token <num> NUMBER

/* Rule type definitions */
/*%type <tnode>  program statement_list statement function_definition
%type <tnode>  if_statement optional_else_statement compound_statement
%type <tnode>  expression equal_expression assign_expression function_call
%type <tnode>  concat_expression simple_expression args
%type <tnode>  identifier string*/

%type <tnode>  program statement_list statement
%type <tnode>  assign_expression
%type <tnode>  getlocal putstring

%expect 1 /* shift/reduce conflict: dangling ELSE */
          /* declaration */
%expect-rr 0

%%

/* -----------------------------------
   Yacc grammer rules
   -----------------------------------*/

program
      : statement_list              {tree = $1;}
      ;

statement_list
      : statement_list statement    {$$ = new rd_tree_node(STMT_LIST, $1, $2);}
      | /* Empty */                 {$$ = new rd_tree_node(EMPTY_STMT);}
      ;

statement
      : END_STMT                    {$$ = new rd_tree_node(EMPTY_STMT);}
      | assign_expression END_STMT  {$$ = $1;}
      | getlocal END_STMT           {$$ = $1;}
      | putstring END_STMT          {$$ = $1;}
      ;

assign_expression
      : ID ASSIGN assign_expression
        {
          rd_tree_node *setlocal = new rd_tree_node(SET_LOCAL);
          setlocal->cont = $1;
          $$ = new rd_tree_node(ASSIGN_EXPR, $3, setlocal);
        }
      | putstring                         {$$ = $1;}
      /*| putobject                       {$$ = $1;}*/
      ;
putstring
      : STRING      {$$ = new rd_tree_node(PUT_STR); $$->cont = $1;}
      ;
getlocal
      : ID          {$$ = new rd_tree_node(GET_LOCAL); $$->cont = $1;}
      ;
%%


char *make_name() {
  char *name = new char[10], num[4];
  static int n = 0;
  sprintf(num, "%d", ++n);
  strcpy(name, "strconst"); strcat(name, num);
  return name;
}
