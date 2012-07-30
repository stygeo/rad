#ifndef _RD_VALUE_H
#define _RD_VALUE_H

#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>

class rd_object;

enum _rd_VAL_TYPE {
  rd_NIL,
  rd_VAL_STR,
  rd_VAL_BOOL,
  rd_VAL_NUM,
  rd_VAL_DEC,
  rd_VAL_OBJ,
};
typedef _rd_VAL_TYPE rd_val_type;

class rd_value {
  public:
    rd_value(char *_s);
    rd_value(std::string _s);
    rd_value(int   _i);
    rd_value(int   _i, rd_val_type _type);
    rd_value(float _f);
    rd_value(bool  _b);

    char *sval() const;
    int   ival() const;
    bool  bval() const;
    float fval() const;
    void  assign(char *_s);
    void  assign(int   _i);
    void  assign(bool  _b);
    void  assign(float _f);
    int   len() const;

    rd_val_type kind_of() const;

    bool eq(const rd_value *rval) const;
    // Somehow overloaded operators don't work ...
    bool operator==(const rd_value &rval) const;
    bool operator!=(const rd_value &rval) const;
    friend std::ostream& operator<<(std::ostream& os, const rd_value& _val);

    const char *to_s() const;

    //void concat(rd_value *n)   {
    //  rd_value str_val = to_s();

    //  int nl = len() + n->len();
    //  char *ns = new char [nl + 1];
    //  strcpy(ns, s); strcat(ns, n->to_s());
    //  delete[] s; s = ns;
    //}

  private:
    char *s;    // String value
    int   i;    // Integer value
    float f;    // Float value
    bool  b;    // Bool value
    rd_object *obj;  // Object value

    rd_val_type type;
};

#endif
