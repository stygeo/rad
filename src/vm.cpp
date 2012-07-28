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

rd_instr *rd_mk_instr(rd_opcode opcode, int target) {
  return new rd_instr(opcode, target);
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
      case OP_EQUAL:
        instr = rd_mk_instr(OP_EQUAL);
        break;
      case OP_JMP:
        instr = rd_mk_instr(OP_JMP, cinstr->target->n);
        break;
      case OP_JMPF:
        instr = rd_mk_instr(OP_JMPF, cinstr->target->n);
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
  printf("| instr. %-56d |\n", instrs.size());
  puts("+------+----------------------+-----------------------------------+");
  puts("| #    | Instruction          | Value (opt)                       |");
  puts("+------+----------------------+-----------------------------------+");
  intcode->show();
  puts("+------+----------------------+-----------------------------------+");
}

void rd_vm::execute() {
  rd_value *pval;
  int ip = 0; // Instruction pointer reset = 0
  int cip = 0;

  for(auto instr : instrs) {
    //printf("ip: %d. target: %d\n", ip, instr->target);
    if((ip != 0 && ip == instr->target) || ip == 0) {
      ip = 0; // Reset instruction pointer = 0

      switch(instr->opcode) {
        case OP_PUT_STR: case OP_PUT_OBJ:
          // Push value on stack
          stack.push(instr->val);
          break;
        case OP_SET_LOCAL:
        {
          // Find variable
          rd_sym_desc *sd = sym_tab->find(instr->val->sval());
          // Pop stack
          pval = stack.pop();

          if(sd == NULL) {
            // Undefined variable, create new
            sd = new rd_sym_desc(instr->val->sval(), pval, 0);
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
          rd_sym_desc *t = sym_tab->find(instr->val->sval());
          rd_value *val;

          if(t == NULL) {
            printf("Undefined variable or method: `%s'\n", instr->val->sval());
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
        case OP_EQUAL:
        {
          rd_value *lval, *rval;

          lval = stack.pop(); rval = stack.pop();
          //std::cout << "lval: " << lval->to_s() << " rval " << rval->to_s() << std::endl;
          stack.push(new rd_value(lval->eq(rval)));

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
          if(!pval->bval()) {
            //printf("I need to jump to: %d\n", instr->target);
            ip = instr->target;
          }
          break;
        }
      }
      pval = NULL;
      cip++;
    }
  }
}
