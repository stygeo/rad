#include <iostream>
#include <string.h>
#include <vm.h>

rd_value *nil = new rd_value(0, rd_NIL);

rd_instr *rd_mk_instr(rd_opcode opcode, rd_value *val = nil);

rd_vm::rd_vm() {
  sym_tab = new rd_sym_tab();
}

rd_vm::~rd_vm() {
  reset();
}

void rd_vm::reset() {
  stack.empty();
}

extern rd_int_instr *intcode;

rd_instr *rd_mk_instr(rd_opcode opcode, rd_value *val) {
  return new rd_instr(opcode, val);
}

void rd_vm::compile() {
  int ninstr = intcode->len();
  rd_int_instr *cinstr = intcode;

  for(int i = 0; i < ninstr; i++) {
    rd_instr *instr;

    switch(cinstr->opcode) {
      case OP_NOP:
        instr = rd_mk_instr(OP_NOP);
        break;
      case OP_PUT_STR:
        instr = rd_mk_instr(OP_PUT_STR, cinstr->str);
        break;
      case OP_PUT_OBJ:
        instr = rd_mk_instr(OP_PUT_OBJ, cinstr->str);
        break;
      case OP_SET_LOCAL:
        instr = rd_mk_instr(OP_SET_LOCAL, cinstr->str);
        break;
      case OP_GET_LOCAL:
        instr = rd_mk_instr(OP_GET_LOCAL, cinstr->str);
        break;
      case OP_PUTS:
        instr = rd_mk_instr(OP_PUTS);
        break;
      default:
        instr = rd_mk_instr(OP_NOP);
        break;
    }

    instrs.push_back(instr);
    cinstr = cinstr->next;
  }
}

void rd_vm::execute() {
  rd_value *pval;

  for(auto instr : instrs) {
    switch(instr->opcode) {
      case OP_PUT_STR: case OP_PUT_OBJ:
        // Push value on stack
        stack.push(instr->val);
        break;
      case OP_SET_LOCAL:
      {
        // Find variable
        rd_sym_desc *t = sym_tab->find(instr->val->to_s());
        // Pop stack
        pval = stack.pop();

        if(t == NULL) {
          // Undefined variable, create new
          t = new rd_sym_desc(instr->val->to_s(), STR_VAR, pval, 0);
          // Add it to the symbol table
          sym_tab->add(t);
        } else {
          // Defined variable, set new value
          t->val = pval;
        }

        break;
      }
      case OP_GET_LOCAL:
      {
        // Find variable
        rd_sym_desc *t = sym_tab->find(instr->val->to_s());
        rd_value *val;

        if(t == NULL) {
          printf("Undefined variable or method: `%s'", instr->val->to_s());
          val = nil;
        } else {
          val = t->val;
        }

        stack.push(val);

        break;
      }
      case OP_PUTS:
      {
        pval = stack.pop();
        std::cout << pval->to_s() << std::endl;
        break;
      }
      default:
        break;
    }
    pval = NULL;
  }
  sym_tab->show();
}
