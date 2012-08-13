#include "config.h"

#include "types.h"
#include "vm.h"

void rad_init();
void rad_script(const char *name);
void rad_exec();
void rad_options(int argc, char *argv[]);

void rd_eval_string(const char *script);
void rd_load_file(const char *file_name);
