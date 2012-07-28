#pragma once

#include <string>
#include <vector>
#include "stack.h"
#include "treenode.h"
#include "intcode.h"
#include "symtab.h"

extern rd_value *nil;

struct rd_instr {
  rd_instr() { opcode = OP_NOP; val = nil; }
  rd_instr(rd_opcode _opcode) { opcode = _opcode; val = nil; }
  rd_instr(rd_opcode _opcode, rd_value *_val) { opcode = _opcode; val = _val; }

  rd_instr(rd_opcode _opcode, int _target) { opcode = _opcode; target = _target; }

  rd_opcode   opcode;
  int         target;
  rd_value    *val;
};

typedef rd_stack<rd_value*> vm_stack;

class rd_vm {
  public:
    rd_vm();
    ~rd_vm();
    void compile();
    void execute();
    void reset();
    void stat();
  private:
    std::vector<rd_instr*> instrs;
    vm_stack stack;
    rd_sym_tab *sym_tab;
};
