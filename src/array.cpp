#include <stdlib.h>
#include <stdio.h>

#include "rd_value.h"
#include "array.h"
#include "class.h"
#include "str.h"
#include "object_space.h"

extern VALUE *rd_class;
extern VALUE *rd_array;

VALUE *array_to_s(int, VALUE *argv, VALUE *self) {
  return rd_new_string("nil");
}

void init_rd_array() {
  rd_array = rd_define_class("Array", rd_class);
  rd_array->type = T_ARRAY;
  rd_array->define_method("to_s", array_to_s);
}

VALUE *rd_new_array() {
  VALUE *array = new VALUE(rd_array);

  return array;
}
