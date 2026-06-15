// File: tree.c
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int errmsg(const char * msg)
{
  fprintf(stderr, "Error: %s\n", msg);
  return 1;
}

int recursive_list(DIR * cwd, int depth)
{
  while (1)
  {
    struct dirent * entry = readdir(cwd);
    if (entry == NULL) break;
    char * fname = entry->d_name;
    if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0) continue;

    for (int i = 1; i < depth; i++) printf("|  ");
    printf("|- %s\n", fname);

    DIR * next = opendir(fname);
    if (next != NULL)
    {
      if (chdir(fname) != 0)
      {
        closedir(next);
        return errmsg("chdir() failed");
      }
      if (recursive_list(next, depth + 1) != 0)
      {
        closedir(next);
        return errmsg("recursive_list() failed");
      }
      closedir(next);
      if (chdir("..") != 0) return errmsg("chdir() failed");
    }
  }
  return 0;
}

int main(int argc, char ** argv)
{
  char * dirname = ".";
  if (argc > 1)
  {
    dirname = argv[1];
    if (chdir(dirname) != 0) return errmsg("chdir() failed");
  }
  DIR * cwd = opendir(".");
  if (cwd == NULL) return errmsg("opendir() failed");

  puts(dirname);
  if (recursive_list(cwd, 1) != 0)
  {
    closedir(cwd);
    return errmsg("recursive_list() failed");
  }

  closedir(cwd);
  return 0;
}
