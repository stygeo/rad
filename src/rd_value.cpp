#include "rd_value.h"
#include "globals.h"

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

VALUE::VALUE(const VALUE &val) {
  type = val.type;
  name = val.name;
  methods = val.methods;
}

void VALUE::send(char *method, int argc) {
  std::map<char *, rd_method*>::iterator it;

  it = methods.find(method);
  if(it == methods.end()) {
    printf("Undefined variable or method: `%s' for :%s (NameError)\n", method, name);
    // rd_raise_exception("");
    exit(1);
  } else {
    // Perform
    // Push something to the stack
  }
}

void VALUE::define_method(char *_method, VALUE *(*func)(), int argc) {
  rd_method *method = new rd_method();
  method->name = _method;
  method->type = T_C_FUNC;
  method->func = func;
}

void VALUE::define_method(char *_method, std::vector<rd_instr*> body, int argc) {
  rd_method *method = new rd_method();
  method->name = _method;
  method->type = T_R_FUNC;
  method->body = body;
}
