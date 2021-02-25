/*
myshell.c
Author: Ben Guiden
*/

/* includes */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1024 // max line buffer

/* Function Prototypes */
#include "myshell.h"

/* Main Func */
int main()
{
   char buf[MAX_BUFFER];   // line buffer

   while(strcmp(buf, "\0"))
   {
      my_prompt();   // print the shell prompt
      fgets(buf, MAX_BUFFER, stdin);   // read input into buf
      buf[strlen(buf) - 1] = 0;  // remove newline character
      char * arg = strtok(buf, " ");   // break buf into tokens seperated by whitespace and take first token      
      if(arg == NULL)
      {
         do_quit();
      }
      if(strcmp(arg, "quit") == 0)  // run quit function if arg = quit
      {
         do_quit();
      }

      else if(strcmp(arg, "clr") == 0)  // run clear function if arg = clear
      {
         do_clear();
      }

      else if(strcmp(arg, "help") == 0)
      {
         do_help();
      }

      else if(strcmp(arg, "dir") == 0)
      {
         do_dir();
      }

      else if(strcmp(arg, "echo") == 0)
      {
         char * token = strtok(NULL, " "); // get next token for echo command
         do_echo(token);
      }

      else if(strcmp(arg, "cd") == 0)
      {
         char * dest = strtok(NULL, " ");
         do_cd(dest);
      }

      else if(strcmp(arg, "environ") == 0)
      {
         do_environ();
      }
   }
   do_quit();
}