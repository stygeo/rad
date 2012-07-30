#include <stdio.h>

#include "config.h"
#include "args.h"

#include <iostream.h>
#include <stdarg.h>
#include "lex.h"
#include "parse.h"
#include "treenode.h"
#include "intcode.h"
#include "rd_value.h"
#include "number.h"

#include "vm.h"

VALUE *rd_number;
VALUE *rd_class;

int errors = 0;
void show_help() {
  printf("Usage: rad [switches] [--] [programfile] [arguments]\n");
  printf("  %-20s one line script\n", "-e 'command'");
  printf("  %-20s print version number and go in verbose mode\n", "-v");
  printf("  %-20s this\n", "-h");

  printf("\n  %-20s print copyright\n", "--copyright");
  printf("  %-20s print version number\n", "--version");

}

void error(char *format, ...) {
  va_list args;

  errors++;
  printf("Line: %d: ", lineno);
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
  printf("\n");
}

void error_summary() {
  printf("%d error(s) were found.\n", errors);
}

void yyerror(const char *msg) {
  error((char*)msg);
}

// This function is called by the lexer when the end-of-file
// is reached; you can reset yyin (the input FILE*) and return 0
// if you want to process another file; otherwise just return 1.
extern "C" int yywrap(void) {
  return 1;
}

SyntTree tree;
rd_int_instr *intcode;

int main(int argc, char *argv[]) {
  init_rd_class();
  init_rd_number();

  yyin = NULL;
  if(argc >= 2) {
    if(strcmp(argv[1], "-h") == 0) {
      show_help();
      exit(0);
    }
    else if(strcmp(argv[1], "--version") == 0) {
      printf("Rad(on) v. %d.%d.%d\n", RD_VER_MAJOR, RD_VER_MINOR, RD_VER_PATCH);
      exit(0);
    }
    else if(strcmp(argv[1], "--copyright") == 0) {
      printf("Rad - Copyright (C) 2012 Jeffrey Wilcke\n");
      exit(0);
    }
    else if(strcmp(argv[1], "-e") == 0) {
      if(argc >= 3) {
        yyin = yy_scan_string(argv[2]);
      } else {
        printf("rad: no code specified for -e");
        exit(1);
      }
    }
    else {
      yyin = fopen(argv[1], "rt");
    }
  }
  if(yyin == NULL) {
    printf("Rad(on) v. %d.%d.%d\n", RD_VER_MAJOR, RD_VER_MINOR, RD_VER_PATCH);
    yyin = stdin;
  }

  yyparse();
  error_summary();

#ifdef SYN_TREE
  tree->show();
#endif

  if(!errors) {
    intcode = rd_mk_int_code(tree);
    intcode->number(1);

    rd_vm vm;
    vm.compile();
#ifdef DEBUG
    vm.stat();
#endif
    vm.execute();

    return 0;
  }

  return 1;
}
