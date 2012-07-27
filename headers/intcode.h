#pragma once

#include "treenode.h"

enum rd_opcode  {
   OP_NOP,           // no operation
   OP_PUT_STR,       // put string
   OP_PUT_OBJ,       // put object
   OP_SET_LOCAL,     // set variable to top of stack
   OP_GET_LOCAL,     // get variable
   OP_PUTS,
   OP_JMPF,

   JUMPTARGET,
};

// Intermediate code instruction
class rd_int_instr {
public:
   rd_int_instr()   {opcode = OP_NOP; next=NULL; target=NULL; str=NULL;}
   rd_int_instr(rd_opcode _opcode)   {opcode = _opcode; next=NULL; target=NULL; str=NULL;}
   rd_int_instr(rd_opcode _opcode, rd_int_instr *_target)   {opcode = _opcode; target=_target; next=NULL; str=NULL;}
   rd_int_instr(rd_opcode _opcode, rd_value *_str) {opcode = _opcode; str = _str;}
   void show ();
   void number (int ln); // number the lines of this code block
   int len();

   int       n;         // line number
   rd_value *str;
   rd_opcode    opcode;    // the opcode
   rd_int_instr *target;    // jump target operand
   rd_int_instr *next;      // the next instruction
};

rd_int_instr *rd_mk_int_code(SyntTree tree);
