#include "rd_value.h"
#include "str.h"
#include "class.h"
#include "number.h"
#include "object_space.h"

extern VALUE *rd_class;
extern VALUE *rd_string;

VALUE *string_to_s(int) {
  return CURRENT_OBJECT;
}

VALUE *string_to_i(int) {
  return rd_new_number(atoi(CURRENT_OBJECT->str_val));
}

void init_rd_string() {
  rd_string = rd_define_class("String", rd_class);
  rd_string->define_method("to_s", string_to_s, 0);
  rd_string->define_method("to_i", string_to_i, 0);
}

VALUE *rd_new_string(char *str) {
  VALUE *string= new VALUE(rd_string);
  string->str_val = str;

  return string;
}
