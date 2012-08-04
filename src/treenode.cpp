#include <stdio.h>
#include "treenode.h"

void error(char *format, ...);

// Node names
char *name[] = {
  "statement list",
  "empty statement",
  "assign",
  "setlocal",
  "getlocal",
  "putstring",
  "putobject",
  "ifthen",
  "ifthenelse",
  "equal",
  "block",
  "while",
  "putspecialobj",
  "getconstant",
  "send",
  "compstmt",
  "argumentlist",
  "argument",
};

// Numbers of children per node type
int children[] = {
   2, 0, 1, 1, 0, 2, 3, 0, 2, 1, 2, 0, 0
};

// Recursively show the contents of the syntax tree
void rd_tree_node::show(int level)   {
  int i,nl;
  if(type != STMT_LIST)   {
    for(i = 0; i < level; i++)   printf("   ");
    printf("%s", name[type]);
    switch(type)  {
      case SET_LOCAL: case GET_LOCAL:
        //printf("\t%s", cont->to_s());
        break;
      case PUT_STR: case PUT_OBJ:
        //printf("\t\"%s\"", cont->to_s());
        break;
    }
    nl = level + 1;
    printf("\n");
  } else nl = level;

  for(auto c : child) {
    c->show(nl);
  }
}

// Check the semantics of this node
int rd_tree_node::coerce_to_string(int childno) {
  return 1;
}

void rd_tree_node::check() {
  // Now, check the semantics
  switch(type)  {
    case ASSIGN_EXPR: // coerce expression to string
      if(!coerce_to_string(0))
        error("=: Can't coerce to string");
      break;
  }
}
