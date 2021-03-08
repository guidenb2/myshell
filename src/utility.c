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

void fork_execute(char ** tokens, int count, int background)
{
   int status;
   int output;
   pid_t pid = getpid();
   switch (pid = fork ()) 
   { 
      case -1:
         exit(EXIT_FAILURE); 
      case 0:                 // child
         output = 0;
         char outFile[MAX_BUFFER];
         for(int i = 0; i < count; i++)
         {
            if(!strcmp(tokens[i], ">"))
            {
               tokens[i] = NULL;
               output = 1;
               strcpy(outFile, tokens[i + 1]);
            }
            else if(!strcmp(tokens[i], ">>"))
            {
               tokens[i] = NULL;
               output = 2;
               strcpy(outFile, tokens[i + 1]);
            }
         }
         if(output == 1)
         {
            freopen(outFile, "w", stdout);
         }
         else if(output == 2)
         {
            freopen(outFile, "a", stdout);
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

void do_quit()
{
   exit(0);    // exit the program
   return;
}

void do_clear(char ** tokens, int background, int count)
{
   for(int i = 0; i < count; i++)
   {
      if(!strcmp(tokens[i], "clr"))
      {
         tokens[i] = "clear";
      }
   }
   fork_execute(tokens, count, background);
   return;
}

void do_help(char ** tokens, char * path, int background, int count)
{
   int size = count + 2;
   char *args[size];
   args[0] = "more";
   args[1] = path;
   int i = 2;
   int j = 1;
   while(tokens[j] != NULL)
   {
      args[i] = tokens[j];
      i++;
      j++;
   }
   args[size - 1] = NULL;
   i = 0;
   fork_execute(args, count, background);
}

void do_echo(char ** tokens, int count, int background)
{
   int i = 1;
   if(background == 0)
   {
      while(tokens[i] != NULL)
      {
         printf("%s ", tokens[i]);
         i++;
      }
   }
   else
   {
      while(tokens[i] != NULL && i < count - 1)
      {
         printf("%s ", tokens[i]);
         i++;
      }
   }
   printf("\n"); // Print new line
   return;
}

void execute_echo(char ** tokens, int count, int background)
{
   int status;
   int output;
   pid_t pid = getpid();
   switch (pid = fork ()) 
   { 
      case -1:
         exit(EXIT_FAILURE); 
      case 0:                 // child
         output = 0;
         char outFile[MAX_BUFFER];
         for(int i = 0; i < count; i++)
         {
            if(!strcmp(tokens[i], ">"))
            {
               tokens[i] = NULL;
               output = 1;
               strcpy(outFile, tokens[i + 1]);
            }
            else if(!strcmp(tokens[i], ">>"))
            {
               tokens[i] = NULL;
               output = 2;
               strcpy(outFile, tokens[i + 1]);
            }
         }
         if(output == 1)
         {
            freopen(outFile, "w", stdout);
         }
         else if(output == 2)
         {
            freopen(outFile, "a", stdout);
         }
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
void do_dir(char ** tokens, int count, int background)
{
   int size = count + 2;
   char *args[size];
   args[0] = "ls";
   args[1] = "-al";
   int i = 2;
   int j = 1;
   while(tokens[j] != NULL)
   {
      args[i] = tokens[j];
      i++;
      j++;
   }
   args[size - 1] = NULL;
   i = 0;
   fork_execute(args, count, background);
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

void execute_environ(char ** tokens, int count, int background)
{
   int status;
   int output;
   pid_t pid = getpid();
   switch (pid = fork ()) 
   { 
      case -1:
         exit(EXIT_FAILURE); 
      case 0:                 // child
         output = 0;
         char outFile[MAX_BUFFER];
         for(int i = 0; i < count; i++)
         {
            if(!strcmp(tokens[i], ">"))
            {
               tokens[i] = NULL;
               output = 1;
               strcpy(outFile, tokens[i + 1]);
            }
            else if(!strcmp(tokens[i], ">>"))
            {
               tokens[i] = NULL;
               output = 2;
               strcpy(outFile, tokens[i + 1]);
            }
         }
         if(output == 1)
         {
            freopen(outFile, "w", stdout);
         }
         else if(output == 2)
         {
            freopen(outFile, "a", stdout);
         }
         do_environ();
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

void execute_pause(char ** tokens, int count, int background)
{
   int status;
   int output;
   pid_t pid = getpid();
   switch (pid = fork ()) 
   { 
      case -1:
         exit(EXIT_FAILURE); 
      case 0:                 // child
         output = 0;
         char outFile[MAX_BUFFER];
         for(int i = 0; i < count; i++)
         {
            if(!strcmp(tokens[i], ">"))
            {
               tokens[i] = NULL;
               output = 1;
               strcpy(outFile, tokens[i + 1]);
            }
            else if(!strcmp(tokens[i], ">>"))
            {
               tokens[i] = NULL;
               output = 2;
               strcpy(outFile, tokens[i + 1]);
            }
         }
         if(output == 1)
         {
            freopen(outFile, "w", stdout);
         }
         else if(output == 2)
         {
            freopen(outFile, "a", stdout);
         }
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
