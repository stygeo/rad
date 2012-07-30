#pragma once
#include <vector>
#include "value.h"

class VALUE;

// The syntax tree node types
enum rd_node_type {
  // Statements
  STMT_LIST,        // list of statements [left-part, right-part] (left-part may be another list)
  EMPTY_STMT,       // empty statement []
  ASSIGN_EXPR,
  SET_LOCAL,
  GET_LOCAL,
  PUT_STR,
  PUTS_STMT,
  PUT_OBJ,
  IFTHEN_STMT,
  IFTHENELSE_STMT,
  EQUAL_EXPR,
  BLOCK_STMT,
  WHILE_STMT,
  PUT_SPEC_OBJ,
  GET_CONST,
};

enum rd_data_type {
};

const int MXCHILD = 3;   // max. number of children a tree node can have

// The syntax tree node class
class rd_tree_node  {
  public:
    rd_tree_node(rd_node_type _type)   {type = _type;}
    rd_tree_node(rd_node_type _type, rd_tree_node *child1) {type = _type; child.push_back(child1);}
    rd_tree_node(rd_node_type _type, rd_tree_node *child1, rd_tree_node *child2) {type = _type; child.push_back(child1); child.push_back(child2);}
    rd_tree_node(rd_node_type _type, rd_tree_node *child1, rd_tree_node *child2, rd_tree_node *child3) {
      type = _type;
      child.push_back(child1); child.push_back(child2); child.push_back(child3);
    }
    void show ()  {show(0);}    // Show the tree contents
    void check();
    void append(rd_tree_node *_node) { child.push_back(_node); }
    int coerce_to_string(int childno);

    rd_node_type type;                // what type of node is it?
    rd_value *cont;

    VALUE *obj;
    char *constant;
    std::vector<rd_tree_node*>child;  // pointers to childen
    rd_data_type rettype;             // the 'return' type of this node
  private:
    void show(int level);
};

typedef rd_tree_node *SyntTree;

