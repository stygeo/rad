#pragma once

#include <map>

#include "instr.h"

class VALUE;
class rd_method;

enum T_TYPE {
  T_NIL,
  T_STRING,
  T_NUMBER,
  T_CLASS,

  T_C_FUNC,
  T_R_FUNC,
};

struct rd_method {
  T_TYPE type;

  const char *name;
  int argc;
  VALUE *(*func)(int argc);
  std::vector<rd_instr*> body;
  VALUE *perform(int argc) { return func(argc); }
};

class VALUE {
  public:
    VALUE();
    VALUE(const VALUE *val);

    T_TYPE type;
    const char *name;
    VALUE *super;

    VALUE *send(char *method, int argc, ...);
    void define_method(char *method, VALUE *(*func)(int argc), int argc);
    void define_method(char *method, std::vector<rd_instr*> body, int argc);

    std::map<char *, rd_method*> methods;
    char *str_val;
    int   int_val;

    const int object_id;
};

VALUE *rd_define_class(const char *name, VALUE *super = NULL);
VALUE *rd_find_constant(const char *name);
