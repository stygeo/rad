#include "rd_value.h"
#include "str.h"
#include "class.h"

extern VALUE *rd_class;
extern VALUE *rd_string;

void init_rd_string() {
  rd_string = rd_define_class("String", rd_class);
}

VALUE *rd_new_string(char *str) {
  VALUE *string= new VALUE(rd_string);
  string->str_val = str;

  return string;
}
