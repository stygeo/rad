#pragma once

#include "intcode.h"

class VALUE;

struct rd_instr {
  rd_instr() { opcode = OP_NOP; obj = NULL; }
  rd_instr(rd_opcode _opcode) { opcode = _opcode; obj = NULL; }
  rd_instr(rd_opcode _opcode, int _target) { opcode = _opcode; target = _target; }
  rd_instr(rd_opcode _opcode, char * _const) { opcode = _opcode; constant = _const; }
  rd_instr(rd_opcode _opcode, VALUE * _obj) { opcode = _opcode; obj = _obj; }

  rd_opcode   opcode;
  int         target;

  char *constant;
  VALUE *obj;
};
