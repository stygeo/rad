#include "rd_value.h"
#include "str.h"
#include "class.h"

extern VALUE *rd_class;
extern VALUE *rd_string;
extern VALUE *current_object;

VALUE *string_to_s() {
  return current_object;
}

void init_rd_string() {
  rd_string = rd_define_class("String", rd_class);
  rd_string->define_method("to_s", string_to_s, 0);
}

VALUE *rd_new_string(char *str) {
  VALUE *string= new VALUE(rd_string);
  string->str_val = str;

  return string;
}
