#pragma once

class VALUE;

typedef VALUE *rd_exception;

void rd_raise(rd_exception exception, const char *format, ...);
