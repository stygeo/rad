#include "value.h"

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
