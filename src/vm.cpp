#include <iostream>
#include <string.h>

#include "vm.h"
#include "rd_value.h"

extern rd_int_instr *intcode;
extern std::vector<VALUE*> constants;

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


rd_instr *rd_mk_instr(rd_opcode opcode, rd_value *val) {
  return new rd_instr(opcode, val);
}

rd_instr *rd_mk_instr(rd_opcode opcode, int target) {
  return new rd_instr(opcode, target);
}

rd_instr *rd_mk_instr(rd_opcode opcode, char *constant) {
  return new rd_instr(opcode, constant);
}

rd_instr *rd_mk_instr(rd_opcode opcode, VALUE *obj) {
  return new rd_instr(opcode, obj);
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
        instr = rd_mk_instr(OP_PUT_STR, cinstr->obj);
        break;
      case OP_PUT_OBJ:
        instr = rd_mk_instr(OP_PUT_OBJ, cinstr->obj);
        break;
      case OP_SET_LOCAL:
        instr = rd_mk_instr(OP_SET_LOCAL, cinstr->constant);
        break;
      case OP_GET_LOCAL:
        instr = rd_mk_instr(OP_GET_LOCAL, cinstr->constant);
        break;
      case OP_GET_CONST:
        instr = rd_mk_instr(OP_GET_CONST, cinstr->constant);
        break;
      case OP_PUTS:
        instr = rd_mk_instr(OP_PUTS);
        break;
      case OP_EQUAL:
        instr = rd_mk_instr(OP_EQUAL);
        break;
      case OP_JMP:
        instr = rd_mk_instr(OP_JMP, cinstr->target->n);
        break;
      case OP_JMPF:
        instr = rd_mk_instr(OP_JMPF, cinstr->target->n);
        break;
      case OP_JMPT:
        instr = rd_mk_instr(OP_JMPT, cinstr->target->n);
        break;
      case JUMPTARGET:
        instr = rd_mk_instr(OP_NOP, cinstr->n);
        break;
      default:
        instr = rd_mk_instr(OP_NOP);
        break;
    }

    instrs.push_back(instr);
    cinstr = cinstr->next;
  }
}

void rd_vm::stat() {
  puts("+-----------------------------------------------------------------+");
  puts("|                    Rad VM Stats                                 |");
  //printf("| instr. %-56ld |\n", instrs.size());
  puts("+------+----------------------+-----------------------------------+");
  puts("| #    | Instruction          | Value (opt)                       |");
  puts("+------+----------------------+-----------------------------------+");
  intcode->show();
  puts("+------+----------------------+-----------------------------------+");
}

extern VALUE *rd_true;
extern VALUE *rd_false;

void rd_vm::execute() {
  VALUE *pval;
  int ip = 0;  // Instruction pointer reset = 0
  int cip = 0; // Current instruction pointer
  int ninstrs = instrs.size();

  rd_instr *instructions[ninstrs];
  int tmpi=0;
  for(auto instr : instrs) { instructions[tmpi] = instr; tmpi++;}
  rd_instr *instr;

  while(cip < ninstrs) {
    if(ip != 0) {
      cip = ip; ip = 0;
    }
    instr = instructions[cip];

    //printf("ip: %d. target: %d\n", ip, instr->target);
    switch(instr->opcode) {
      case OP_PUT_STR: case OP_PUT_OBJ:
        // Push value on stack
        stack.push(instr->obj);
        break;
      case OP_SET_LOCAL:
      {
        // Find variable
        rd_sym_desc *sd = sym_tab->find(instr->constant);
        // Pop stack
        pval = stack.pop();

        if(sd == NULL) {
          // Undefined variable, create new
          sd = new rd_sym_desc(instr->constant, pval, 0);
          // Add it to the symbol table
          sym_tab->add(sd);
        } else {
          // Defined variable, set new value
          sd->val = pval;
        }

        break;
      }
      case OP_GET_LOCAL:
      {
        // Find variable
        rd_sym_desc *t = sym_tab->find(instr->constant);
        VALUE *val;

        if(t == NULL) {
          printf("Undefined variable or method: `%s' (NameError)\n", instr->constant);
          exit(1);
        } else {
          val = t->val;
        }

        stack.push(val);

        break;
      }
      case OP_GET_CONST:
      {
        VALUE *constant = rd_find_constant(instr->constant);
        if(constant == NULL) {
          printf("NameError: Uninitialized constant %s\n", instr->constant);
          exit(1);
        }
        break;
      }
      case OP_PUTS:
      {
        pval = stack.pop();
        std::cout << pval->send("to_s", 1, new VALUE())->str_val << std::endl;

        break;
      }
      case OP_EQUAL:
      {
        VALUE *lval, *rval;

        lval = stack.pop(); rval = stack.pop();
//        std::cout << "lval: " << lval->to_s() << " rval " << rval->to_s() << std::endl;
        //stack.push(new rd_value(lval->eq(rval)));
        stack.push(rd_false);

        break;
      }
      case OP_JMP:
      {
        ip = instr->target;
        break;
      }
      case OP_JMPF:
      {
        pval = stack.pop();
        if(pval == rd_false) {
          //printf("I need to jump to: %d\n", instr->target);
          ip = instr->target;
        }
        break;
      }
      case OP_JMPT:
        pval = stack.pop();
        if(pval == rd_true)
          ip = instr->target;
        break;
    }
    pval = NULL;
    cip++;
  }
}
