/* Parser for smpl */

%{

#pragma once
// Includes
#include <string.h>

#include "lex.h"     // the lexer
#include "treenode.h" // the syntax tree
#include "types.h"

extern SyntTree tree;
extern int _argc;

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
  char         *str;     // a char string
  int          num;     // a number
  rd_tree_node *tnode;   // node in the syntax tree
}

/* Token definitions */
%token ERROR_TOKEN IF ELSE INPUT ASSIGN EQUAL CLASS
%token CONCAT END_STMT OPEN_PAR CLOSE_PAR
%token BEGIN_CS END_CS DEF THEN END COMMA RETURN
%token WHILE
%token DOT NEW
%token <str> ID STRING METHOD OBJECT
%token <num> NUMBER

%type <tnode>  program statement_list statement expression compound_statement
%type <tnode>  equal_expression assign_expression simple_expression
%type <tnode>  if_statement block_start head_expr optional_else_statement
%type <tnode>  getlocal putobject getconstant send argumentlist arguments
%type <tnode>  class_def method_call method_def argument_def_list argument_def

%expect 5
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
      | class_def                   {$$ = $1;}
      | method_def                  {$$ = $1;}
      | head_expr if_statement      {$$ = new rd_tree_node(BLOCK_STMT, $2, $1);}
      | head_expr END_STMT          {$$ = $1;}
      | compound_statement          {$$ = $1;}
      | method_call                 {$$ = $1;}
      ;

head_expr
      : expression                  {$$ = $1;}
      ;

block_start
      : if_statement                {$$ = $1;}
      ;

if_statement
      : IF expression
        {
          $$ = new rd_tree_node(IFTHEN_STMT, $2);
        }
      ;

compound_statement
      : IF expression statement_list optional_else_statement END_CS
        {
          if($4 != NULL)
            $$ = new rd_tree_node(IFTHENELSE_STMT, $2, $3, $4);
          else
            $$ = new rd_tree_node(IFTHEN_STMT, $2, $3);
        }
      | WHILE expression statement_list END_CS      {$$ = new rd_tree_node(WHILE_STMT, $2, $3);}
      | BEGIN_CS statement_list END_CS {$$ = $2;}
      ;

optional_else_statement
      : ELSE statement_list    {$$ = $2;}
      | /* empty */            {$$ = NULL;}
      ;
/*compound_statement
      : block_start statement_list END_CS {$$ = new rd_tree_node(BLOCK_STMT, $1, $2);}
      | BEGIN_CS statement_list END_CS {$$ = $2;}
      ;
*/

expression
      : equal_expression            {$$ = $1;}
      ;

equal_expression
      : expression EQUAL expression {$$ = new rd_tree_node(EQUAL_EXPR, $1, $3);}
      | assign_expression           {$$ = $1;}
      ;

assign_expression
      : ID ASSIGN assign_expression
        {
          rd_tree_node *setlocal = new rd_tree_node(SET_LOCAL);
          setlocal->constant = $1;
          $$ = new rd_tree_node(ASSIGN_EXPR, $3, setlocal);
        }
      | simple_expression           {$$ = $1;}
      ;

simple_expression
      : getlocal    {$$ = $1;}
      | putobject   {$$ = $1;}
      | getconstant {$$ = $1;}
      /*
      | simple_expression send argumentlist {$$ = new rd_tree_node(COMP_STMT, $3, $1, $2);}
      | simple_expression send OPEN_PAR CLOSE_PAR {$$ = new rd_tree_node(COMP_STMT, NULL, $1, $2);}
      */
      | simple_expression send OPEN_PAR CLOSE_PAR {$$ = new rd_tree_node(COMP_STMT, $1, $2, NULL);}
      | simple_expression send argumentlist {$$ = new rd_tree_node(COMP_STMT, $1, $2, $3);}
      ;

argumentlist
      : arguments                    {$$ = new rd_tree_node(ARGUMENT_LIST, $1);}
      | OPEN_PAR arguments CLOSE_PAR {$$ = new rd_tree_node(ARGUMENT_LIST, $2);}
      ;
arguments
      : expression    {$$ = $1;}
      | arguments COMMA expression  {$$ = new rd_tree_node(ARGUMENT, $3, $1); _argc++;}
      ;

method_call
      : ID OPEN_PAR CLOSE_PAR   {$$ = new rd_tree_node(SEND_STMT, NULL); $$->constant = $1;}
      | ID argumentlist         {$$ = new rd_tree_node(SEND_STMT, $2); $$->constant = $1;}
      ;

send
      : METHOD                      {$$ = new rd_tree_node(SEND_STMT, NULL); $$->constant = $1;}
      ;

class_def
      : CLASS OBJECT statement_list END_CS {$$ = new rd_tree_node(CLASS_DEF, $3); $$->constant = $2;}
      ;

method_def
      : DEF ID argument_def_list statement_list END_CS {$$ = new rd_tree_node(METHOD_DEF, $4, $3); $$->constant = $2;}
      ;

argument_def_list
      : argument_def                       {$$ = new rd_tree_node(ARGUMENT_LIST, $1);}
      | OPEN_PAR argument_def CLOSE_PAR    {$$ = new rd_tree_node(ARGUMENT_LIST, $2);}
      | OPEN_PAR CLOSE_PAR                 {$$ = new rd_tree_node(ARGUMENT_LIST, NULL);}
      ;
argument_def
      : getlocal                           {$$ = new rd_tree_node(ARGUMENT_DEF, $1);}
      | argument_def COMMA getlocal        {$$ = new rd_tree_node(ARGUMENT_DEF, $3, $1);}
      ;

getconstant
      : OBJECT                      {$$ = new rd_tree_node(GET_CONST); $$->constant = $1;}
      ;

putobject
      : NUMBER      {$$ = new rd_tree_node(PUT_OBJ); $$->obj = rd_new_number($1);}
      | STRING      {$$ = new rd_tree_node(PUT_STR); $$->obj = rd_new_string($1);}
      ;

getlocal
      : ID          {$$ = new rd_tree_node(GET_LOCAL); $$->constant = $1;}
      ;
%%


char *make_name() {
  char *name = new char[10], num[4];
  static int n = 0;
  sprintf(num, "%d", ++n);
  strcpy(name, "strconst"); strcat(name, num);
  return name;
}
