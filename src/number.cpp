#include "rd_value.h"
#include "number.h"
#include "class.h"

extern VALUE *rd_class;
extern VALUE *rd_number;

void init_rd_number() {
  rd_number = rd_define_class("Number", rd_class);
}

VALUE *rd_new_number(int num) {
  VALUE *number = new VALUE(*rd_number);

  return number;
}
