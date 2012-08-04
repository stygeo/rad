#include "rd_value.h"
#include "globals.h"
#include "object_space.h"

// Global object counter
int _global_current_object_id = 0;
int _new_object_id() {
  return _global_current_object_id++;
}

VALUE *return_new_value(int) {
  return new VALUE(CURRENT_OBJECT);
}

VALUE::VALUE() : object_id(_new_object_id()) {
}

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

  val->define_method("new", return_new_value, 0);

  constants.push_back(val);

  return val;
}

VALUE::VALUE(const VALUE *val) : object_id(_new_object_id()) {
  type = val->type;
  name = val->name;
  methods = val->methods;
}

VALUE *VALUE::send(char *method, int argc, ...) {
  for(auto m : methods) {
    if(strcmp(m.first, method) == 0) {
      if( m.second->argc != -1 && argc != m.second->argc ) {
        printf("ArgumentError: wrong number of argumens (%d for %d)", argc, m.second->argc);
        exit(1);
      }

      /*
      if(argc != 0) {
        va_list arg_list;
        va_start(arg_list, argc);
        VALUE *arguments[argc];
        for(int i = 0; i < argc; i++) {
          arguments[i] = va_arg(arg_list, VALUE*);
        }
        va_end(arg_list);
      }
      */

      PUSH_CURRENT_OBJ(this);  // Push this object as the current object
        VALUE *ret = m.second->perform(argc);
      POP_CURRENT_OBJ();         // Restore previous current object

      return ret;
      // Push something to the stack
    }
  }

  printf("Undefined variable or method: `%s' for :%s (NameError)\n", method, name);
  exit(1);
}

void VALUE::define_method(char *_method, VALUE *(*func)(int argc), int argc) {
  rd_method *method = new rd_method();
  method->name = _method;
  method->type = T_C_FUNC;
  method->func = func;
  method->argc = argc;

  methods[_method] = method;
}

void VALUE::define_method(char *_method, std::vector<rd_instr*> body, int argc) {
  rd_method *method = new rd_method();
  method->name = _method;
  method->type = T_R_FUNC;
  method->body = body;
  method->argc = argc;

  methods[_method] = method;
}
