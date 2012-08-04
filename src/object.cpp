#include <stdlib.h>
#include <stdio.h>

#include "rd_value.h"
#include "object.h"
#include "class.h"
#include "str.h"
#include "object_space.h"

extern VALUE *rd_class;
extern VALUE *rd_object;

VALUE *object_to_s(int) {
  return rd_new_string("Object");
}

VALUE *object_puts(int argc) {
  for(int i = 0; i < argc; i++) {
    VALUE *pval = ST_POP();
    char *str;
    if(pval->type == T_STRING)
      str = pval->str_val;
    else
      str = pval->send("to_s", 0)->str_val;

    printf("%s\n", str);
  }

  return rd_new_string("nil");
}

void init_rd_object() {
  rd_object = rd_define_class("Object", rd_class);
  rd_object->define_method("to_s", object_to_s, 0);
  rd_object->define_method("puts", object_puts, -1);
}

VALUE *rd_new_object() {
  VALUE *object = new VALUE(rd_object);

  return object;
}
