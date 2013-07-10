#include <stdlib.h>
#include <stdio.h>

#include "rd_value.h"
#include "object.h"
#include "class.h"
#include "str.h"
#include "object_space.h"

extern VALUE *rd_class;
extern VALUE *rd_object;

void rd_scan_args(int argc, VALUE *argv, const char *format, ...) {
// TODO
}

VALUE *object_puts(int argc, VALUE *argv, VALUE *self) {
  VALUE *objects[argc+1];

  rd_scan_args(argc, argv, "0:0:*", &objects); // TODO
  for(auto val : self->arr_val) {
    char *str;
    if(val->type == T_STRING)
      str = val->str_val;
    else
      str = val->send("to_s", 0)->str_val;

    printf("%s\n", str);
  }

  return rd_new_string("nil");
}

void init_rd_object() {
  rd_object = rd_define_class("Object", rd_class);
  rd_object->define_method("puts", object_puts);
}

VALUE *rd_new_object() {
  VALUE *object = new VALUE(rd_object);

  return object;
}
