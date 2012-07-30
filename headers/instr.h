#pragma once

#include "intcode.h"

class VALUE;

extern rd_value *nil;

struct rd_instr {
  rd_instr() { opcode = OP_NOP; val = nil; }
  rd_instr(rd_opcode _opcode) { opcode = _opcode; val = nil; }
  rd_instr(rd_opcode _opcode, rd_value *_val) { opcode = _opcode; val = _val; }
  rd_instr(rd_opcode _opcode, int _target) { opcode = _opcode; target = _target; }
  rd_instr(rd_opcode _opcode, char * _const) { opcode = _opcode; constant = _const; }
  rd_instr(rd_opcode _opcode, VALUE * _obj) { opcode = _opcode; obj = _obj; }

  rd_opcode   opcode;
  int         target;
  rd_value    *val;

  char *constant;
  VALUE *obj;
};
