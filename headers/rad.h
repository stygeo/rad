#include "types.h"
#include "vm.h"

void rad_init();
void rad_script(const char *name);
void rad_exec();
void rd_eval_string(const char *script);
FILE *rd_load_file();
