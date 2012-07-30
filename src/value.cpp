#include "value.h"

rd_value::rd_value(char *_s)                    {s = new char[1];s[0]=0; assign(_s); type = rd_VAL_STR;}
rd_value::rd_value(std::string _s)              {s = new char[1];s[0]=0; assign(_s.c_str()); type = rd_VAL_STR;}
rd_value::rd_value(int   _i)                    {i = _i; type = rd_VAL_NUM;}
rd_value::rd_value(int   _i, rd_val_type _type) {i = _i; type = _type;}
rd_value::rd_value(float _f)                    {f = _f; type = rd_VAL_DEC;}
rd_value::rd_value(bool  _b)                    {b = _b; type = rd_VAL_BOOL;}

char *rd_value::sval() const  {return s;}   // Return string value
int   rd_value::ival() const  {return i;}   // Return number value
bool  rd_value::bval() const  {return b;}   // Return boolean value
float rd_value::fval() const  {return f;}   // Return float value
void  rd_value::assign(char *_s)   {int l; delete[] s; l = strlen(_s); s = new char[l+1]; strcpy(s, _s);type = rd_VAL_STR;}
void  rd_value::assign(int   _i)   {i = _i;type = rd_VAL_NUM;}
void  rd_value::assign(bool  _b)   {b = _b;type = rd_VAL_BOOL;}
void  rd_value::assign(float _f)   {f = _f;type = rd_VAL_DEC;}
int   rd_value::len() const        {return strlen(s);}

rd_val_type rd_value::kind_of() const {return type;}
const char *rd_value::to_s() const {
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
  return buff.str().c_str();
}

std::ostream &operator<<(std::ostream &os, const rd_value &_val){
  os << _val.sval(); return os;
}

bool rd_value::eq(const rd_value *rval) const {
  if(type == rval->type) {
    switch(type) {
      case rd_NIL:
        puts("Warning: Comparing nil to nil.");
        return true;
      case rd_VAL_BOOL:
        return bval() == rval->bval();
      case rd_VAL_NUM:
        return ival() == rval->ival();
      case rd_VAL_DEC:
        return fval() == rval->fval();
      case rd_VAL_STR:
        return (strcmp(sval(), rval->sval()) == 0 ? true : false);
    }
  }

  return false;
}

bool rd_value::operator==(const rd_value &rval) const {
}

bool rd_value::operator!=(const rd_value &rval) const {
  return !(*this == rval);
}
