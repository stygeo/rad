#include "rd_value.h"
#include "str.h"
#include "class.h"
#include "number.h"
#include "object_space.h"

extern VALUE *rd_class;
extern VALUE *rd_string;

VALUE *string_to_s(int, VALUE *argv, VALUE *self) {
  return self;
}

VALUE *string_to_i(int, VALUE *argv, VALUE *self) {
  return rd_new_number(atoi(self->str_val));
}

void init_rd_string() {
  rd_string = rd_define_class("String", rd_class);
  rd_string->define_method("to_s", string_to_s);
  rd_string->define_method("to_i", string_to_i);
}

VALUE *rd_new_string(char *str) {
  VALUE *string= new VALUE(rd_string);
  string->str_val = str;

  return string;
}
