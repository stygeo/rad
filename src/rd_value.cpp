#include "rd_value.h"
#include "str.h"
#include "globals.h"
#include "object_space.h"

// Global object counter
int _global_current_object_id = 0;
int _new_object_id() {
  return _global_current_object_id++;
}

VALUE *return_new_value(int argc, VALUE *argv, VALUE *self) {
  return new VALUE(self);
}

VALUE *default_to_s(int argc, VALUE *argv, VALUE *self) {
  return rd_new_string((char*)self->name);
}

VALUE::VALUE() : object_id(_new_object_id()) {
}

VALUE *rd_find_constant(const char *name) {
  //for(auto cnst : CONSTANTS) {
  int const_size = CONSTANTS->len();

  for(int i = 0; i < const_size; i++) {
    VALUE *cnst = CONSTANTS->get_no(i);
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

  val->define_method("new", return_new_value);
  val->define_method("to_s", default_to_s);

  ADD_CONST(val);

  return val;
}

VALUE::VALUE(const VALUE *val) : object_id(_new_object_id()) {
  type = val->type;
  name = val->name;
  methods = val->methods;
}

bool VALUE::respond_to(char *method) {
  rd_method *meth = get_method(method);

  if(meth) return true;
  return false;
}

rd_method *VALUE::get_method(char *method) {
  for(auto m : methods) {
    if(strcmp(m.first, method) == 0) {
      return m.second;
    }
  }

  return NULL;
}

VALUE *VALUE::send(char *method, int argc, VALUE *argv) {
  rd_method *meth = get_method(method);
  if(meth) {
    return meth->perform(argc, argv, argv);
  }

  printf("Undefined variable or method: `%s' for :%s (NameError)\n", method, name);
  exit(1);
}

VALUE *VALUE::send(char *method, int argc, ...) {
  for(auto m : methods) {
    if(strcmp(m.first, method) == 0) {
      //if( m.second->argc != -1 && argc != m.second->argc ) {
      //  printf("ArgumentError: wrong number of argumens (%d for %d)", argc, m.second->argc);
      //  exit(1);
      //}

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

      // Todo
      VALUE *argv;
      VALUE *ret = m.second->perform(argc, argv, this);

      return ret;
    }
  }

  printf("Undefined variable or method: `%s' for :%s (NameError)\n", method, name);
  exit(1);
}

void VALUE::define_method(char *_method, VALUE *(*func)(int argc, VALUE *argv, VALUE *self)) {
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

VALUE *rd_method::perform(int argc, VALUE *argv, VALUE *self) {
  if(func != NULL) {
    return func(argc, argv, self);
  } else if(body.size()) {
    return rd_new_string("nil");
  }

  return rd_new_string("nil");
}

