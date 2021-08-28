#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void seive() {
  int prime;
  read(0, &prime, sizeof(int));
  printf("prime %d \n", prime);
  
  int p[2];
  int cpid = 0;
  pipe(p);
  while (1)
  {
    int n;
    if (read(0, &n, sizeof(int)) != 0)
    {
      if (n % prime != 0) 
      {
        if (cpid == 0) {
          cpid = fork();
          if (cpid == 0) 
          {
            close(0);
            dup(p[0]);
            close(p[1]);
            seive();          
          } else {
            close(p[0]);
          }
        }
        write(p[1], &n, sizeof(int));
      }
    } else 
    {
      close(p[1]);
      break;
    }
  }
}

int main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  if (fork() == 0) 
  {
    close(0);
    dup(p[0]);
    close(p[1]);
    seive();
  } else
  {
    int i;
    for (i = 2; i < 35; i++) 
    {
      write(p[1], &i, sizeof(int));
    }
    close(p[1]);
  }
  wait((int *)0);
  exit(0);
}
