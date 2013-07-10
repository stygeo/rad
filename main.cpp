#include <stdio.h>

#include "config.h"
#include "args.h"

#include <iostream>
#include <stdarg.h>
#include "rad.h"

void show_help() {
  printf("Usage: rad [switches] [--] [programfile] [arguments]\n");
  printf("  %-20s one line script\n", "-e 'command'");
  printf("  %-20s print version number and go in verbose mode\n", "-v");
  printf("  %-20s this\n", "-h");

  printf("\n  %-20s print copyright\n", "--copyright");
  printf("  %-20s print version number\n", "--version");
}

int main(int argc, char *argv[]) {
  rad_init();

  if(argc >= 2) {
    if(strcmp(argv[1], "-h") == 0) {
      show_help();
      exit(0);
    }
    else if(strcmp(argv[1], "--version") == 0) {
      printf("Rad(on) v. %d.%d.%d\n", RD_VER_MAJOR, RD_VER_MINOR, RD_VER_PATCH);
      exit(0);
    }
    else if(strcmp(argv[1], "--copyright") == 0) {
      printf("Rad - Copyright (C) 2012 Jeffrey Wilcke\n");
      exit(0);
    }
    else if(strcmp(argv[1], "-e") == 0) {
      if(argc >= 3) {
        rd_eval_string(argv[2]);
      } else {
        printf("rad: no code specified for -e");
        exit(1);
      }
    }
    else {
      rd_load_file(argv[1]);
    }

    rad_exec();

    return 0;
  }

  return 1;
}
