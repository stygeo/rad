#pragma once

#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>

enum _rd_VAL_TYPE {
  rd_NIL,
  rd_VAL_STR,
  rd_VAL_BOOL,
  rd_VAL_NUM,
  rd_VAL_DEC
};
typedef _rd_VAL_TYPE rd_val_type;

class rd_value {
  public:
    rd_value() {}
    rd_value(char *_s)                    {s = new char[1];s[0]=0; assign(_s); type = rd_VAL_STR;}
    rd_value(std::string _s)              {s = new char[1];s[0]=0; assign(_s.c_str()); type = rd_VAL_STR;}
    rd_value(int   _i)                    {i = _i; type = rd_VAL_NUM;}
    rd_value(int   _i, rd_val_type _type) {i = _i; type = _type;}
    rd_value(float _f)                    {f = _f; type = rd_VAL_DEC;}
    rd_value(bool  _b)                    {b = _b; type = rd_VAL_BOOL;}

    char *sval()  {return s;}   // Return string value
    int   ival()  {return i;}   // Return number value
    bool  bval()  {return b;}   // Return boolean value
    float fval()  {return f;}   // Return float value
    void  assign(char *_s)   {int l; delete[] s; l = strlen(_s); s = new char[l+1]; strcpy(s, _s);type = rd_VAL_STR;}
    void  assign(int   _i)   {i = _i;type = rd_VAL_NUM;}
    void  assign(bool  _b)   {b = _b;type = rd_VAL_BOOL;}
    void  assign(float _f)   {f = _f;type = rd_VAL_DEC;}

    //friend std::ostream& operator<<(std::ostream& os, rd_value& _val) {
    //  os << _val.sval(); return os;
    //}

    int   len()              {return strlen(s);}

    rd_val_type kind_of() {return type;}

    char *to_s() {
      std::stringstream buff;

      switch(type) {
        case rd_NIL:
          buff << "nil";
          break;
        case rd_VAL_BOOL:
          buff << (b ? "true" : "false");
          break;
        case rd_VAL_NUM:
          buff << i;
          break;
        case rd_VAL_DEC:
          buff << f;
          break;
        default:
          buff << sval();
          break;
      }
      return (char*)buff.str().c_str();
    }

    void concat(rd_value *n)   {
      rd_value str_val = to_s();

      int nl = len() + n->len();
      char *ns = new char [nl + 1];
      strcpy(ns, s); strcat(ns, n->to_s());
      delete[] s; s = ns;
    }

  private:
    char *s;    // String value
    int   i;    // Integer value
    float f;    // Float value
    bool  b;    // Bool value

    rd_val_type type;
};


