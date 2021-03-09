/*
myshell.c
Author: Ben Guiden
*/

/* includes */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>


#define MAX_BUFFER 1024 // max line buffer

/* Function Prototypes */
#include "myshell.h"

/* Main Func */
int main(int argc, char * argv[])
{
   int count;
   char *manualPath = malloc(strlen(curr_dir()) + strlen("/../manual/readme") + 1); // +1 for the null-terminator
   strcpy(manualPath, curr_dir());
   strcat(manualPath, "/../manual/readme");
   int background = 0;
   FILE * fp;
   set_env_var("SHELL");

   if(argc > 1)
   {
      char * batchfile = argv[1];
      fp = fopen(batchfile, "r");
      if(fp == NULL) 
      {
         perror("Error opening file");
         return(-1);
      }
   }

   while(1)
   {
      char buf[MAX_BUFFER];   // line buffer
      if(argc > 1)
      {
         if(fgets(buf, MAX_BUFFER, fp))
         {
            buf[strcspn(buf, "\r")] = '\0';
         }
         else
         {
            fclose(fp);
            do_quit();
         }
      }
      else
      {
         my_prompt();   // print the shell prompt
         fgets(buf, MAX_BUFFER, stdin);   // read input into buf
         buf[strcspn(buf, "\n")] = '\0';
      }
      count = word_counter(buf);
      char * args[count];
      args[0] = strtok(buf, " ");
      for(int i = 1; i < count; i++)
      {
         args[i] = strtok(NULL, " ");
      }
      args[sizeof args / sizeof args[0]] = NULL;
      char * arg = args[0];
      for(int i = 0; i < count; i++)
      {
         if(strcmp(args[i], "&") == 0)
         {
            background = 1;
         }
      }
      if(strlen(arg) == 0)
      {
         printf("No value entered\n");
         do_quit();
      }
      else if(strcmp(arg, "quit") == 0)  // run quit function if arg = quit
      {
         do_quit(args, background, count);
      }

      else if(strcmp(arg, "clr") == 0)  // run clear function if arg = clear
      {
         do_clear(args, background, count);
      }

      else if(strcmp(arg, "pause") == 0)  // run clear function if arg = clear
      {
         execute_internal_command("pause", args, count, background);
      }

      else if(strcmp(arg, "help") == 0)
      {
         do_help(args, manualPath, background, count);
      }

      else if(strcmp(arg, "dir") == 0)
      {
         do_dir(args, count, background);
      }

      else if(strcmp(arg, "echo") == 0)
      {
         execute_internal_command("echo", args, count, background);
      }

      else if(strcmp(arg, "cd") == 0)
      {
         if(count == 1)
         {
            do_dir(args, count, background);
         }
         else if(count > 1)
         {
            if(!strcmp(args[1], ">") || !strcmp(args[1], ">>"))
            {
               do_dir(args, count, background);
            }
            else
            {
               do_cd(args[1]);
            }
         }
      }

      else if(strcmp(arg, "environ") == 0)
      {
         execute_internal_command("environ", args, count, background);
      }

      else
      {
         fork_execute(args, count, background);
      }
   }
   exit(0);
}