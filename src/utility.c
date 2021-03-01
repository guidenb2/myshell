/*
utilities.c
Author: Ben Guiden
*/

/* includes */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> /* a header file with a function we might need */
#include <string.h>
#include <dirent.h>  /* some functions need for do_dir */

#define MAX_BUFFER 1024 // max line buffer

void my_prompt()
{
   char dir[MAX_BUFFER];
   getcwd(dir, MAX_BUFFER);
   printf("%s%s ", dir, ">");
   return;
}

void do_quit()
{
   printf("Quiting...\n");
   sleep(1);
   exit(0);    // exit the program
   return;
}

void do_clear()
{
   printf("Clearing...\n");
   sleep(1);
   system("clear");  // use clear command
   return;
}

void do_help()
{
   system("more ../manual/readme.md");
   return;
}

void do_echo(char * tokens)
{
   while(tokens != NULL)
   {
      printf("%s ", tokens);
      tokens = strtok(NULL, " ");
   }
   printf("\n"); // Print new line
   return;
}

// https://c-for-dummies.com/blog/?p=3246
void do_dir()
{
   DIR *folder;
   struct dirent *file;
   folder = opendir(".");
   while((file=readdir(folder)) != NULL)
   {
      printf("%s\n", file->d_name);
   }
   closedir(folder);
   return;
}

char * curr_dir()
{
   char *dir = malloc(MAX_BUFFER);
   getcwd(dir, MAX_BUFFER);
   return dir;
}

void set_env_var(char *var)
{
   char * dir;
   dir = curr_dir();
   if(!strcmp(var, "SHELL"))
   {
      setenv(var, strcat(dir, "/myshell"), 1);
   }
   else
   {
      setenv(var, dir, 1);
   }
   free(dir);
   return;
}

// https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.2.0/com.ibm.zos.v2r2.bpxbd00/rputen.htm
void do_cd(char * dest)
{
   if(dest == NULL)
   {
      printf("%s\n", curr_dir());
      return;
   }
   else if(chdir(dest) == -1)
   {
      printf("Error: cannot find the directory %s\n", dest);
      return;
   }
   set_env_var("PWD");
   return;
}

// https://man7.org/linux/man-pages/man7/environ.7.html
void do_environ()
{
   extern char **environ;
   for(int i = 0; environ[i] != NULL; i++)
   {
      printf("%s\n", environ[i]);
   }
   return;
}