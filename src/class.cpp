#include "rd_value.h"
#include "class.h"

extern VALUE *rd_class;

void init_rd_class() {
  rd_class = rd_define_class("Class", NULL);
}

VALUE *default_init() {
  return rd_class;
}

