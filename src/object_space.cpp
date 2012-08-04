#include "object_space.h"

rd_object_space *rd_object_space::_singleton;
rd_object_space *rd_object_space::get() {
  if(_singleton == NULL) {
    _singleton = new rd_object_space();
  }

  return _singleton;
}

rd_object_space::rd_object_space() {
  stack = new v_stack();
  object_stack = new o_stack();
}

rd_object_space::~rd_object_space() {
  stack->empty();
  object_stack->empty();

  delete stack;
  stack = NULL;
}

void rd_object_space::push_current_obj(VALUE *val) {
  object_stack->push(val);
}

void rd_object_space::pop_current_obj() {
  object_stack->pop();
}

VALUE *rd_object_space::current_object() {
  return object_stack->get_top();
}
