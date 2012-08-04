#include <stdlib.h>
#include <stdio.h>

#include "rd_value.h"
#include "number.h"
#include "class.h"
#include "str.h"
#include "object_space.h"

extern VALUE *rd_class;
extern VALUE *rd_number;

VALUE *number_to_s(int) {
  char *buff;
  buff = new char[50];
  snprintf(buff, sizeof(buff), "%d", CURRENT_OBJECT->int_val);
  return rd_new_string(buff);
}

void init_rd_number() {
  rd_number = rd_define_class("Number", rd_class);
  rd_number->type = T_NUMBER;
  rd_number->define_method("to_s", number_to_s, 0);
}

VALUE *rd_new_number(int num) {
  VALUE *number = new VALUE(rd_number);
  number->int_val = num;

  return number;
}
