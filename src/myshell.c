/*
file: myshell.c
author: Ben Guiden
student number: 19310046
email: ben.guiden2@mail.dcu.ie

I have read and understood the DCU Academic Integrity and Plagiarism Policy. All work 
is my own unless indicated otherwise.

***************************************************************************************************

As permission has been provide, the base structure for this file was taken from Lab 4b.

strcmp() is used to check if the in the input matches an internal command name.
As strcmp() returns a 0 to indicate a match in two strings, we use !strcmp().

https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c/8465083 was used
to understand the process of concatenating two strings in C.

***************************************************************************************************/

/* includes */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define MAX_BUFFER 1024 // max line buffer

/* Function Prototypes */
#include "myshell.h"

/* Main Func */
int main(int argc, char * argv[])
{
   /* Create variables that will be need later */
   int count;
   int background = 0; // background will be = 1 if the command should be run in the background
   FILE * fp;
   char *manualPath = malloc(strlen(curr_dir()) + strlen("/../manual/readme") + 1); // +1 for the null-terminator
   
   /* Gets the full (absolute) path to the manual which allows us to find the manual no matter what dir we are in */
   strcpy(manualPath, curr_dir());
   strcat(manualPath, "/../manual/readme"); 
   
   /* Call the create set_env_var function with the paramater "SHELL" to set the shell env variable */
   set_env_var("SHELL");

   /* If command line contains more than 1 word, it contains a batchfile which should be read */
   if(argc > 1)
   {
      char * batchfile = argv[1];   // batchfile = the second commandline argument
      fp = fopen(batchfile, "r");
      if(fp == NULL)    // If file variable fp is Null, the file doesn't exist
      {
         printf("Error opening file\n");
         free(manualPath);
         return(-1); // -1 indicates failure
      }
   }

   while(1) // As our shell program knows when to quit, the while loop can keep on going
   {
      char buf[MAX_BUFFER];   // line buffer
      if(argc > 1)   // For batchfiles
      {
         if(fgets(buf, MAX_BUFFER, fp))   // if a newline exists
         {
            buf[strcspn(buf, "\r")] = '\0';  // remove the return key so our strcmp() works
         }
         else  // if we have reached the end of file
         {
            fclose(fp); // Close the file
            free(manualPath); // Free the manualPath var
            do_quit();
         }
      }
      else  // For command line inputs
      {
         my_prompt();   // print the shell prompt
         fgets(buf, MAX_BUFFER, stdin);   // read input into buf
         buf[strcspn(buf, "\n")] = '\0';  // remove newline character so our strcmp() works
      }
      count = word_counter(buf); // Get the number of words in our line. This count is used for loops later
      char * args[count];  // Create an array to store each individual word seperately
      
      /* Use strtok to split a line on whitespace characters */
      args[0] = strtok(buf, " ");
      for(int i = 1; i < count; i++)
      {
         args[i] = strtok(NULL, " ");
      }
      args[sizeof args / sizeof args[0]] = NULL;   // Set the final element of the array to NULL, so exec functions works. Exec functions terminate on the first NULL characters in an array.

      char * arg = args[0];   // Get the first element of the array. The first element is the command name. We use this in strcmp() to check if the command matches one of our internal commands

      if(arg == NULL)   // If no command is entered, we exit to avoid a segementation error
      {
         printf("No value entered\n");
         free(manualPath);
         do_quit();
      }

      /* Check if the command is to be entered in a background process */
      for(int i = 0; i < count; i++)
      {
         if(strcmp(args[i], "&") == 0) // if an argument is == &, we know that the command should be executed in the background
         {
            background = 1;   // background = 1 indicates that the command should be run in the background
         }
      }
      if(!strcmp(arg, "quit"))  // run quit function if arg = quit
      {
         free(manualPath);
         do_quit(args, background, count);   // Call the do_quit command from utility.c
      }

      else if(!strcmp(arg, "clr"))  // run clear function if arg = clear
      {
         do_clear(args, background, count);
      }

      else if(!strcmp(arg, "pause"))  // run clear function if arg = clear
      {
         execute_internal_command("pause", args, count, background); // Call the command with pause as a parameter to indicate to run the do_pause command
      }

      else if(!strcmp(arg, "help"))
      {
         do_help(args, manualPath, background, count);   // Call do_help, we also provide the path to the manual. NB: as with all commands we also provide background to indicate if the command should be run in the background and also the count
      }

      else if(!strcmp(arg, "dir"))
      {
         do_dir(args, count, background);
      }

      else if(!strcmp(arg, "echo"))
      {
         execute_internal_command("echo", args, count, background);
      }

      else if(!strcmp(arg, "cd"))
      {
         if(count == 1) // If only one word is provided, we know that no directory has been provided. Therefore, we just print the current directory
         {
            do_dir(args, count, background);
         }
         else if(count > 1)
         {
            if(!strcmp(args[1], ">") || !strcmp(args[1], ">>"))   // If the second word is just used for output redirection, print the current directory
            {
               do_dir(args, count, background);
            }
            else
            {
               do_cd(args[1]);   // If a dir is provided, change to it
            }
         }
      }

      else if(!strcmp(arg, "environ"))
      {
         execute_internal_command("environ", args, count, background);
      }

      else if(!strcmp(arg, "calender"))
      {
         args[0] = "cal";
         fork_execute(args, count, background);
      }

      else
      {
         fork_execute(args, count, background);
      }
   }
   exit(0);
}