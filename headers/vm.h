#pragma once

#include <string>
#include <vector>
#include "stack.h"
#include "treenode.h"
#include "intcode.h"
#include "symtab.h"
#include "instr.h"
#include "class.h"
#include "object_space.h"

typedef rd_stack<VALUE*> vm_stack;

class rd_vm {
  public:
    rd_vm();
    ~rd_vm();
    void compile();
    void execute();
    void reset();
    void stat();
  private:
    vm_stack *stack;
    rd_sym_tab *sym_tab;
    std::vector<rd_instr*> instrs;
};
