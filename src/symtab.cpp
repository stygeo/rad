#include <stdio.h>
#include <string.h>
#include "symtab.h"
#include "rd_value.h"

rd_sym_desc::rd_sym_desc(const char *_name, VALUE *_val, int _line)    {
  name = _name;
  val = _val;
  line = _line;
  next = NULL;
}

rd_sym_desc::~rd_sym_desc() {
  if(this == NULL)   return;
  if(name != NULL)   delete [] name;
}

void rd_sym_desc::show() {
  printf("| %-20s | %4d |(= \"%s\")\n", name, line, val->str_val);
}

rd_sym_tab::rd_sym_tab() {
  start = NULL;
}

rd_sym_tab::~rd_sym_tab() {
  delete start;
}

bool rd_sym_tab::add(rd_sym_desc *symb) {
  if(find(symb->name) != NULL)
    return false;
  if(start == NULL)
    start = symb;
  else  {
    rd_sym_desc *search = start;
    while(search->next != NULL)
      search = search->next;
    search->next = symb;
  }
  return true;
}

rd_sym_desc *rd_sym_tab::find(const char *name)   {
  rd_sym_desc *search = start;
  while(search != NULL && strcmp(search->name, name) != 0) {
    search = search->next;
  }
  return search;
}

void rd_sym_tab::show()  {
  rd_sym_desc *search = start;
  puts("+----------------------+------+");
  puts("| Name                 | Line |");
  puts("+----------------------+------+");
  while(search != NULL)  {
    search->show();
    search = search->next;
  }
  puts("+----------------------+------+");
}
