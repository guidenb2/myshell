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
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_BUFFER 1024 // max line buffer

void my_prompt()
{
   char dir[MAX_BUFFER];
   getcwd(dir, MAX_BUFFER);
   printf("%s%s ", dir, ">");
   return;
}

int word_counter(char * line)
{
   int cnt = 0;
   for(int i = 0; i < strlen(line); i++)
   {
      if(line[i] == ' ')
      {
         if(line[i + 1] != '\0' && line[i + 1] != ' ')
         {
            cnt += 1;
         }
      }
   }
   return cnt + 1;
}

void do_quit()
{
   printf("Quiting...\n");
   sleep(1);
   exit(0);    // exit the program
   return;
}

void do_clear(int background)
{
   pid_t pid = getpid();
   printf("Clearing...\n");
   sleep(1);
   int status;
   switch (pid = fork ()) { 
      case -1:
         exit(EXIT_FAILURE); 
      case 0:                 // child
         execlp("clear", "clear", NULL);
         exit(EXIT_SUCCESS);
      default:                // parent
         if(background == 1)
         {
            waitpid(pid, &status, WNOHANG);
         }
         if(background == 0)
         {
            waitpid(pid, &status, WUNTRACED);
         }
   } 
   return;
}

void do_help()
{
   pid_t pid = getpid();
   int status;
   switch (pid = fork ()) { 
      case -1:
         exit(EXIT_FAILURE); 
      case 0:                 // child
         execlp("more", "more", "../manual/readme.md", NULL);
         exit(EXIT_SUCCESS);
      default:                // parent
         waitpid(pid, &status, WUNTRACED);
   } 
   return;
}

void do_echo(char ** tokens, int count, int background)
{
   if(background == 0)
   {
      for(int i = 1; i < count; i++)
      {
         printf("%s ", tokens[i]);
      }
   }
   else
   {
      for(int i = 1; i < count - 1; i++)
      {
         printf("%s ", tokens[i]);
      }
   }
   printf("\n"); // Print new line
   return;
}

void execute_echo(char ** tokens, int count, int background)
{
   int status;
   pid_t pid = getpid();
   switch (pid = fork ()) 
   { 
      case -1:
         exit(EXIT_FAILURE); 
      case 0:                 // child
         do_echo(tokens, count, background);
         exit(EXIT_SUCCESS);
      default:                // parent
         if(background == 0)
         {
            waitpid(pid, &status, WUNTRACED);
         }
         if(background == 1)
         {
            waitpid(pid, &status, WNOHANG);
         }
   } 
}

// https://c-for-dummies.com/blog/?p=3246
void do_dir(char * dest, int background)
{
   pid_t pid = getpid();
   int status;
   switch (pid = fork ()) { 
      case -1:
         exit(EXIT_FAILURE); 
      case 0:                 // child
         printf("Directory: %s\n", dest);
         execlp("ls", "ls", "-al", dest, NULL);
         exit(EXIT_SUCCESS);
      default:                // parent
         if(background == 0)
         {
            waitpid(pid, &status, WUNTRACED);
         }
         else
         {
            waitpid(pid, &status, WNOHANG);
         }
   } 
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

void do_pause()
{
   char input[MAX_BUFFER];
   printf("Press [enter] to continue...\n");
   while(strcmp(input, "\n"))
   {
      fgets(input, MAX_BUFFER, stdin);
   }
   return;
}

void execute_pause(int background)
{
   int status;
   pid_t pid = getpid();
   switch (pid = fork ()) 
   { 
      case -1:
         exit(EXIT_FAILURE); 
      case 0:                 // child
         do_pause();
         exit(EXIT_SUCCESS);
      default:                // parent
         if(background == 0)
         {
            waitpid(pid, &status, WUNTRACED);
         }
         if(background == 1)
         {
            waitpid(pid, &status, WNOHANG);
         }
   } 
}

void fork_execute(char ** tokens, int count, int background)
{
   int status;
   pid_t pid = getpid();
   switch (pid = fork ()) 
   { 
      case -1:
         exit(EXIT_FAILURE); 
      case 0:                 // child
         if(background == 1)
         {
            tokens[count - 1] = NULL;
         }
         execvp(tokens[0], tokens); 
         exit(EXIT_SUCCESS);
      default:                // parent
         if(background == 0)
         {
            waitpid(pid, &status, WUNTRACED);
         }
         if(background == 1)
         {
            waitpid(pid, &status, WNOHANG);
         }
   } 
}