#include "rd_value.h"
#include "globals.h"

VALUE *current_object;

VALUE *rd_find_constant(const char *name) {
  for(auto cnst : constants) {
    if(strcmp(cnst->name, name) == 0)
      return cnst;
  }

  return NULL;
}

VALUE *rd_define_class(const char *name, VALUE *super) {
  if(rd_find_constant(name)) {
    puts("Redefinition of class `%s' not allowed\n");
    exit(1);
  }

  VALUE *val = new VALUE();

  // Special base Class case. Super is self
  if(super == NULL) {
    val->super = val;
  }

  val->type = T_CLASS;
  val->name = name;

  constants.push_back(val);

  return val;
}

VALUE::VALUE(const VALUE *val) {
  type = val->type;
  name = val->name;
  methods = val->methods;
}

#define PUSH_CURRENT_OBJ(obj) \
  VALUE *_prev_obj = current_object; \
  current_object = obj;
#define POP_CURRENT_OBJ current_object = _prev_obj;

VALUE *VALUE::send(char *method, int argc, ...) {
  std::map<char *, rd_method*>::iterator it;

  it = methods.find(method);
  if(it == methods.end()) {
    printf("Undefined variable or method: `%s' for :%s (NameError)\n", method, name);
    // rd_raise_exception("");
    exit(1);
  } else {
    if(argc != 0) {
      va_list arg_list;
      va_start(arg_list, argc);
      VALUE *arguments[argc];
      for(int i = 0; i < argc; i++) {
        arguments[i] = va_arg(arg_list, VALUE*);
      }
      va_end(arg_list);
    }

    PUSH_CURRENT_OBJ(this);  // Push this object as the current object
      VALUE *ret = it->second->func();
    POP_CURRENT_OBJ;         // Restore previous current object

    return ret;
    // Push something to the stack
  }
}

void VALUE::define_method(char *_method, VALUE *(*func)(), int argc) {
  rd_method *method = new rd_method();
  method->name = _method;
  method->type = T_C_FUNC;
  method->func = func;

  methods[_method] = method;
}

void VALUE::define_method(char *_method, std::vector<rd_instr*> body, int argc) {
  rd_method *method = new rd_method();
  method->name = _method;
  method->type = T_R_FUNC;
  method->body = body;

  methods[_method] = method;
}
