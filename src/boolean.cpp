#include "rd_value.h"
#include "boolean.h"
#include "class.h"

extern VALUE *rd_class;
extern VALUE *rd_true;
extern VALUE *rd_false;

void init_rd_boolean() {
  rd_true = rd_define_class("TrueClass", rd_class);
  rd_false = rd_define_class("FalseClass", rd_class);
}

