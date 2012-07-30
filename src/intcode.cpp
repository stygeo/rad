#include <stdio.h>
#include "treenode.h"
#include "intcode.h"
#include "rd_value.h"

// Number: assigns line numbers to this block of intermediate code, starting with ln
void rd_int_instr::number(int ln)   {
  rd_int_instr *num = this;
  while(num != NULL)   {
    num->n = ln++; num = num->next;
  }
}

int rd_int_instr::len() {
  rd_int_instr *i = next; int cnt = 1;
  while(i != NULL) { cnt++; i=i->next; }
  return cnt;;
}

// Names of the opcodes
char *op_name[] = {
  "nil", "putstring", "putobject", "setlocal",
  "getlocal", "puts", "opt_eq", "jumpunless",
  "jumpif", "jump", "getconstant",

  "jumptarget",
};

// show this block of intermediate code
void rd_int_instr::show()   {
  printf("| %04d | %-20s |", n, op_name[opcode]);
  if(constant) printf(" %-30s |", constant);
  if(obj)   printf(" %-30d |", obj->int_val);
  if(target)   printf(" %-30d |", target->n);
  if(!obj && !constant && !target) printf("%-31s |", "");
  printf("\n");

  if(next != NULL)   next->show();
}

// concatenates two blocks of instructions
rd_int_instr *concatenate(rd_int_instr *blk1, rd_int_instr *blk2)  {
  rd_int_instr *search = blk1;
  while(search->next != NULL)   search = search->next;
  search->next = blk2;
  return blk1;
}

// Prefix a jump target to a block; returns the new block
rd_int_instr *prefix_jt(rd_int_instr *blk, rd_int_instr *ref_instr)   {
   rd_int_instr *jt = new rd_int_instr;
   jt->opcode = JUMPTARGET;
   jt->target = ref_instr;   // the referring instruction
   jt->next = blk;

   return jt;
}


// Recursively generate intermediate code
rd_int_instr *rd_mk_int_code(SyntTree tree)  {
  rd_tree_node *root = tree;
  rd_int_instr *blk1, *blk2,
               *cond, *jump2else, *thenpart, *jump2end, *elsepart, *endif,
               *jump2callee, *endfunc, *jump_return, *return_target, *end,
               *begin, *jump2begin;

  switch(root->type)  {
    case STMT_LIST:
      blk1 = rd_mk_int_code(root->child[0]);
      blk2 = rd_mk_int_code(root->child[1]);
      concatenate(blk1, blk2);
      return blk1;
    case BLOCK_STMT:
      // Assemble a new chain. Not possible thru lex
      // child[0]->child[1] = if statement
      // child[1] = statement_list
      root->child[0]->child[1] = root->child[1];
      return rd_mk_int_code(root->child[0]);
    case EMPTY_STMT:
      return new rd_int_instr(OP_NOP);
    case SET_LOCAL:
      return new rd_int_instr(OP_SET_LOCAL, root->constant);
    case GET_LOCAL:
      return new rd_int_instr(OP_GET_LOCAL, root->constant);
    case GET_CONST:
      return new rd_int_instr(OP_GET_CONST, root->constant);
    case PUT_STR:
      return new rd_int_instr(OP_PUT_STR, root->obj);
    case PUT_OBJ:
      return new rd_int_instr(OP_PUT_OBJ, root->obj);
    case ASSIGN_EXPR:
      blk1 = rd_mk_int_code(root->child[0]);
      blk2 = rd_mk_int_code(root->child[1]);
      return concatenate(blk1, blk2);
    case PUTS_STMT:
      blk1 = rd_mk_int_code(root->child[0]);
      blk2 = new rd_int_instr(OP_PUTS);
      return concatenate(blk1, blk2);
    case EQUAL_EXPR:
      blk1 = rd_mk_int_code(root->child[0]);
      blk2 = rd_mk_int_code(root->child[1]);
      concatenate(blk1, blk2);

      return concatenate(blk1, new rd_int_instr(OP_EQUAL));
    case IFTHEN_STMT:
      cond     = rd_mk_int_code(root->child[0]);
      jump2end = new rd_int_instr(OP_JMPF);
      thenpart = rd_mk_int_code(root->child[1]);
      endif    = new rd_int_instr(JUMPTARGET, jump2end);
      jump2end->target = endif;

      concatenate(cond, jump2end);
      concatenate(jump2end, thenpart);
      concatenate(thenpart, endif);

      return cond;
    case IFTHENELSE_STMT:
      // First, create the necessary code parts
      cond      = rd_mk_int_code(root->child[0]);
      jump2else = new rd_int_instr(OP_JMPF);      // set target below
      thenpart  = rd_mk_int_code(root->child[1]);
      elsepart  = prefix_jt(rd_mk_int_code(root->child[2]), jump2else);
      jump2else->target = elsepart;
      jump2end  = new rd_int_instr(OP_JMP);       // set target below
      endif     = new rd_int_instr(JUMPTARGET, jump2end);
      jump2end->target = endif;

      // Now, concatenate them all
      concatenate(cond, jump2else);
      concatenate(jump2else, thenpart);
      concatenate(thenpart, jump2end);
      concatenate(jump2end, elsepart);
      concatenate(elsepart, endif);
      return cond;
    case WHILE_STMT:
      // Jump to if statement
      jump2end   = new rd_int_instr(OP_JMP);
      end        = new rd_int_instr(JUMPTARGET, jump2end);
      jump2end->target = end;

      // Jump back if true
      jump2begin = new rd_int_instr(OP_JMPT);
      begin      = new rd_int_instr(JUMPTARGET, jump2begin);
      jump2begin->target = begin;

      // Conditions and statement list
      cond       = rd_mk_int_code(root->child[0]);
      thenpart   = rd_mk_int_code(root->child[1]);

      concatenate(jump2end, begin); // JUMP, TARGET
      concatenate(begin, thenpart); // TARGET, STMT LIST
      concatenate(thenpart, end);   // STMT LIST, TARGET
      concatenate(end, cond);       // TARGET, CONDITIONS
      concatenate(cond, jump2begin);// CONDITIONS, IF STMT

      return jump2end;
  }
  return new rd_int_instr(OP_NOP); // shouldn't happen
}
