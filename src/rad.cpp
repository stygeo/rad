#include "rad.h"
#include "parse.h"
#include "lex.h"

#include "vm.h"

SyntTree tree;
rd_int_instr *intcode;

int errors = 0;
void error(char *format, ...) {
  va_list args;

  errors++;
  printf("Line: %d: ", lineno);
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
  printf("\n");
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

void rad_init() {
  yyin = NULL;
  rad_init_types();

  rd_vm::get();
}

void rad_script(const char *name) {
}

void rad_exec() {
  if(yyin == NULL) {
    yyin = stdin;
  }

  yyparse();

#ifdef SYN_TREE
  tree->show();
#endif

  if(!errors) {
    intcode = rd_mk_int_code(tree);
    intcode->number(1);
  } else {
    exit(1);
  }

  rd_vm *vm = rd_vm::get();

  vm->compile();
  vm->execute();
}

void rad_options(int argc, char *argv[]) {}

void rd_eval_string(const char *script) {
  char *nbuf;
  sprintf(nbuf, "%s\n", script);
  yy_scan_string(nbuf);
}

void rd_load_file(const char *file_name) {
  yyin = fopen(file_name, "rt");
}

