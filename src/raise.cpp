#include "rd_value.h"
#include "raise.h"

rd_exception MethodError;

void rd_raise(rd_exception exception, const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf(format, args);
  va_end(args);
  exit(0);
}
