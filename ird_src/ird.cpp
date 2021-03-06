#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rad.h"
#include "ird.h"

static char **ird_completion(const char *, int, int);
char *ird_generator(const char *, int);
char *dupstr(char*);
bool check_quit(const char *buf);

void *xmalloc(int);

char *cmd[] = {"exit"};

void init_ird(int argc, char *argv[]) {
  printf("Interactive Rad %d.%d.%d\n", RD_VER_MAJOR, RD_VER_MINOR, RD_VER_PATCH);

  char *buf;
  rl_attempted_completion_function = ird_completion;

  int count = 1;
  char *prompt;
  sprintf(prompt, "\nrad :%03d > ", count);
  while((buf = readline(prompt)) != NULL) {
    rl_bind_key('\t', rl_complete);
    if(check_quit(buf))
      break;
    if(buf[0] != 0) {
      add_history(buf);

      rd_eval_string(buf);
      rad_exec();
    }

    count++;
    sprintf(prompt, "\nrad :%03d > ", count);
  }

  free(buf);

  exit(0);
}

bool check_quit(const char *buf) {
  if(
      strcmp(buf, "quit") == 0 ||
      strcmp(buf, "q") == 0 ||
      strcmp(buf, "exit") == 0)
    return true;

  return false;
}

static char** ird_completion( const char * text , int start,  int end)
{
  char **matches;

  matches = (char **)NULL;

  if (start == 0)
    matches = rl_completion_matches ((char*)text, &ird_generator);
  else
    rl_bind_key('\t',rl_insert);

  return (matches);
}

char* ird_generator(const char* text, int state)
{
  static int list_index, len;
  char *name;

  if (!state) {
    list_index = 0;
    len = strlen (text);
  }

  while((name = cmd[list_index])) {
    list_index++;

    if (strncmp (name, text, len) == 0)
      return (dupstr(name));
  }

  /* If no names matched, then return NULL. */
  return ((char *)NULL);

}

char * dupstr (char* s) {
  char *r;

  r = (char*) xmalloc ((strlen (s) + 1));
  strcpy (r, s);
  return (r);
}

void * xmalloc (int size)
{
  void *buf;

  buf = malloc (size);
  if (!buf) {
    fprintf (stderr, "Error: Out of memory. Exiting.'n");
    exit (1);
  }

  return buf;
}
