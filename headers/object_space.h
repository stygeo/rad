#pragma once

#include <vector>
#include "stack.h"
#include "rd_value.h"

#define PUSH_CURRENT_OBJ(obj) \
  rd_object_space::get()->push_current_obj(obj)
#define POP_CURRENT_OBJ() rd_object_space::get()->pop_current_obj()
#define CURRENT_OBJECT    rd_object_space::get()->current_object()
#define ST_PUSH(obj)      rd_object_space::get()->get_stack()->push(obj)
#define ST_POP()          rd_object_space::get()->get_stack()->pop()
#define ADD_CONST(obj)    rd_object_space::get()->get_constants()->push(obj)
#define CONSTANTS         rd_object_space::get()->get_constants()

typedef rd_stack<VALUE*> v_stack;
typedef rd_stack<VALUE*> o_stack;

class rd_object_space {
  public:
    rd_object_space();
    ~rd_object_space();
    static rd_object_space *get();

    v_stack *get_stack() { return stack; }
    o_stack *get_constants() { return constants; }

    void   push_current_obj(VALUE *obj);
    void   pop_current_obj();
    VALUE *current_object();
  private:
    static rd_object_space *_singleton;
    v_stack *stack;     // Variable stack
    o_stack *object_stack; // current object stack;
    o_stack *constants;
};
