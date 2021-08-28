#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find_file(char *path, char *file_name)
{
  // printf("path => %s \n", path);
  int fd;  
  struct stat st;
  struct dirent de;
  char buf[512], *p;
  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  if (st.type == T_DIR) {
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      if (strcmp(file_name, de.name) == 0) {
        write(1, buf, strlen(buf));
      }
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      if (strcmp(".", de.name) != 0 && strcmp("..", de.name) != 0 && st.type == T_DIR) {
        find_file(buf, file_name);
      }
    }
  }
}


int main(int argc, char **argv)
{
  if (argc < 3) 
  {
    fprintf(2, "Usage: find [path] [name]...\n");
    exit(1);
  }
  find_file(argv[1], argv[2]);
  write(1, "\n", 1);
  exit(0);
}

