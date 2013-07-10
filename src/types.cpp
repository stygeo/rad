#include "types.h"

VALUE *rd_number;
VALUE *rd_class;
VALUE *rd_true;
VALUE *rd_false;
VALUE *rd_string;
VALUE *rd_object;
VALUE *rd_array;

void rad_init_types() {
  init_rd_class();
  init_rd_object();
  init_rd_number();
  init_rd_boolean();
  init_rd_string();
  init_rd_array();
}
