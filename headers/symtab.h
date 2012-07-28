#pragma once

#include <iostream.h>
#include <string.h>
#include "value.h"

enum rd_sym_type   {
  STR_VAR,    // String variable
  STR_CONST,  // String constant
};

class rd_sym_desc {
  public:
    rd_sym_desc (const char *_name, rd_value *_val, int _line);
    ~rd_sym_desc ();
    void show ();
    void set_no(int n) {no=n;}
    int  get_no() {return no;}
    int no;

    const char   *name;  // name of the symbol
    rd_value     *val;
    rd_sym_type  type;   // type of the symbol
    int          line;   // line it was first encountered
    rd_sym_desc  *next;  // next symbol in list
};

class rd_sym_tab  {
  public:
    rd_sym_tab();
    ~rd_sym_tab();
    bool add (rd_sym_desc *symb);
    rd_sym_desc *find(const char *name);
    void show ();
    rd_sym_desc *get_first()   { current = start; return current; }
    rd_sym_desc *get_next() {
      if(current != NULL)
        current = current->next;
      return current;
    }

  private:
    rd_sym_desc *start;
    rd_sym_desc *current;
};
