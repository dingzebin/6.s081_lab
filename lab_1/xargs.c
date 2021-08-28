#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


int read_from_left(char (*args)[MAXARG])
{
  char buf[512];
  int num_of_args = 0;
  if (read(0, buf, sizeof buf) != 0) {
    int len = strlen(buf);
    int start = 0;
    for (int i = 0; i < len; i++)
    {
      if (buf[i] == '\\' || buf[i] == ' ' || buf[i] == '\n')
      {
        if (buf[i] == '\\' && buf[i + 1] != 'n') {
          continue;
        }
        if (i - start > 0) 
        {
          char *p = *args;
          for (; start < i; start++)
          {
            if (buf[start] != '"') {
              *p++ = buf[start];
            }
          }
          *p = '\0';
          args++;
          num_of_args++;
        }
        if (buf[i] == '\\') {
          i++;          
        }
        start = i + 1;
      }
    }
  }
  return num_of_args;
}

void exec_cmd(char *arg[MAXARG], char params[MAXARG][MAXARG], int arg_end, int start, int num) {
  int tmp = arg_end;
  while (num--)
  {
    arg[arg_end++] = params[start++];
  }
  if (fork() == 0) {
    exec(arg[0], arg);
    exit(0);
  } else {
    wait(0);
    while (arg[tmp])
    {
      arg[tmp++] = 0;
    }
  } 
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    exit(1);
  }
  int n = 0;
  int i = 1;
  if (!strcmp(argv[1], "-n")) {
    n = atoi(argv[2]);
    i = 3;
  }
  char *arg[MAXARG];
  int j = 0;
  for (; i < argc; i++, j++) {
    arg[j] = argv[i];
  }
  char read_args[MAXARG][MAXARG];
  int num_of_args = read_from_left(read_args);
  if (n == 0) {
    exec_cmd(arg, read_args, j, 0, num_of_args);
  } else {
    int num = 0;
    int start = 0;
    for (int k = 0; k < num_of_args; k++)
    {
      if (num == n) {
        exec_cmd(arg, read_args, j, start, num);
        num = 0;
        start = k;
      }
      num++;
    }
    if (num > 0) {
      exec_cmd(arg, read_args, j, start, num);
    }
  }
  exit(0);
}
