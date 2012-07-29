/* Lexical analyzer */
%{

/* Includes */
#include <string.h>
#include <iostream>

class TreeNode;

#define _LEX_CPP_
#include "lex.h"
#include "parser.hpp"

extern "C" int yywrap(); // Declare yywrap

/* Forwards */
void identifier();
void number();
void string_constant();
void eat_comment();
void print_errorized_token();

%}

/* ------------------------------------
   Macros (Standard regexp)
   ------------------------------------*/

LETTER   [a-zA-Z_]
NUMBER   [0-9]
IDENT    {LETTER}({LETTER}|{NUMBER})*
STR      \"[^\"]*\"
WSPACE   [ \t]+

/* ------------------------------------
   Lexer rules
   ------------------------------------*/

%%

"if"      {return IF;}
"else"    {return ELSE;}
"puts"    {return PUTS;}
"input"   {return INPUT;}
"def"     {return DEF;}
"return"  {return RETURN;}
"end"     {return END_CS;}
"while"   {return WHILE;}
"="       {return ASSIGN;}
"=="      {return EQUAL;}
"+"       {return CONCAT;}
"("       {return OPEN_PAR;}
")"       {return CLOSE_PAR;}
"{"       {return BEGIN_CS;}
"}"       {return END_CS;}
","       {return COMMA;}
{IDENT}   {identifier();        /* identifier: copy name */
          return ID;}
{STR}     {string_constant();   /* string constant: copy contents */
          return STRING;}
{NUMBER}  {number();
          return NUMBER;}
"#"       {eat_comment();}      /* Comment:     skip */
\n        {lineno++;            /* newline:     count lines */
          return END_STMT;}     /* endstmt:     End of statement */
";"       {return END_STMT;}
{WSPACE}  {}                    /* whitespace: (do nothing) */
.         {return ERROR_TOKEN;} /* other char: error, illigal token */

%%

/* ------------------------------------
   Functions
   ------------------------------------*/

void eat_comment() {
  char c;

  while((c == yyinput()) != '\n' && c != 0);
  lineno++;
}

/* Pass ID */
void identifier() {
  yylval.str = new char[strlen(yytext+1)];
  strcpy(yylval.str, yytext);
}

/* Pass string const */
void string_constant() {
  int l = strlen(yytext)-2;
  yylval.str = new char[l+1];
  strncpy(yylval.str, &yytext[1], l); yylval.str[l] = 0;
}

/* Pass number */
void number() {
  yylval.num = atoi(yytext);
}
