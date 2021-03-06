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
   char buf[MAX_BUFFER];   // line buffer
   int count;
   int background;
   FILE * file;
   set_env_var("SHELL");

   if(argc > 1)
   {
      char * batchfile = argv[1];
      file = fopen(argv[1], "r");
   }

   while(strcmp(buf, "\0"))
   {
      if(argc > 1)
      {
         fgets(buf, MAX_BUFFER, file);
         if(buf[strlen(buf) - 1] == '\n')
         {
            buf[strcspn(buf, "\n")] = '\0';
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
      char * s = strtok(buf, " ");
      int i = 0;
      while(s != NULL)
      {
         args[i] = s;
         s = strtok(NULL, " ");
         i++;
      }
      args[sizeof args / sizeof args[0]] = NULL;
      char * arg = args[0];
      if(strcmp(args[count - 1], "&") == 0)
      {
         background = 1;
      }
      else
      {
         background = 0;
      }
      if(arg == NULL)
      {
         printf("No value entered\n");
         do_quit();
      }
      else if(strcmp(arg, "quit") == 0)  // run quit function if arg = quit
      {
         do_quit();
      }

      else if(strcmp(arg, "clr") == 0)  // run clear function if arg = clear
      {
         do_clear();
      }

      else if(strcmp(arg, "pause") == 0)  // run clear function if arg = clear
      {
         do_pause();
      }

      else if(strcmp(arg, "help") == 0)
      {
         do_help();
      }

      else if(strcmp(arg, "dir") == 0)
      {
         char * dest = args[1];
         if(count > 1)
         {
            do_dir(dest);
         }
         else
         {
            do_dir(".");
         }
      }

      else if(strcmp(arg, "echo") == 0)
      {
         do_echo(args, count);
      }

      else if(strcmp(arg, "cd") == 0)
      {
         if(count > 1)
         {
            do_cd(args[1]);
         }
         else
         {
            do_dir(".");
         }
      }

      else if(strcmp(arg, "environ") == 0)
      {
         do_environ();
      }

      else
      {
         execute(args, count, background);
      }
   }
   do_quit();
}