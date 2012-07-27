#include <stdio.h>
#include <string.h>
#include "symtab.h"

rd_sym_desc::rd_sym_desc(char *_name, rd_sym_type _type, rd_value *_val, int _line)    {
  name = _name; type = _type;
  val = _val;
  line = _line;
  next = NULL;
}

rd_sym_desc::~rd_sym_desc()  {
   if(this == NULL)   return;
   if(name != NULL)   delete [] name;
}

void rd_sym_desc::show()   {
   if(type == STR_CONST)
      printf("| %-20s | %4d |(= \"%s\")\n", name, line, val->sval());
   else
      printf("| %-20s | %4d |\n", name, line);
}

rd_sym_tab::rd_sym_tab()   {
   start = NULL;
}

rd_sym_tab::~rd_sym_tab()   {
   delete start;
}

bool rd_sym_tab::add(rd_sym_desc *symb)  {
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

rd_sym_desc *rd_sym_tab::find(char *name)   {
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
