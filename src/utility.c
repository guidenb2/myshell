/*
file: utility.c
author: Ben Guiden
student number: 19310046
email: ben.guiden2@mail.dcu.ie

I have read and understood the DCU Academic Integrity and Plagiarism Policy. All work 
is completely my own unless indicated otherwise.

**************************************************************************************

Sources used are commented beside where they are used

**************************************************************************************

This file contains the created functions used in myshell.c and listed in myshell.h 

*************************************************************************************/

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

/* Print the current working directory */
void my_prompt()
{
   char dir[MAX_BUFFER];   // variable to hold the current directory
   getcwd(dir, MAX_BUFFER);   // get current working directory and store it in our dir variable
   printf("%s%s ", dir, ">"); // print the cwd and a > for the command prompt without a newline
   return;
}

/* Count the number of words in a line */
int word_counter(char * line)
{
   int cnt = 0;   // Create a cnt var to store our count
   for(int i = 0; i < strlen(line); i++)  // Go through each character in the line until we've reached the length of the string
   {
      if(line[i] == ' ')   // If a character is whitespace
      {
         if(line[i + 1] != '\0' && line[i + 1] != ' ')   // Ensures we don't count a double space as two words
         {
            cnt += 1;
         }
      }
   }
   return cnt + 1;   // Add 1 for the final word which will not be seperated by whitespace
}

/* Fork and execute a command */
void fork_execute(char ** tokens, int count, int background)
{
   int status;
   int output;
   int input;
   pid_t pid = getpid();   // Get the pid
   switch (pid = fork ())  // fork process
   { 
      case -1: // failure
         exit(EXIT_FAILURE); 
      case 0:  // child process
         output = 0; // output = 0 for no redirection, 1 for replace file contents and 2 for append file contents
         input = 0;  // input = 0 for no redirection and 1 for input redirection
         char outFile[MAX_BUFFER];  // var to hold output file
         char inFile[MAX_BUFFER];   // var to hold input file
         for(int i = 0; i < count; i++)   // go through the arguments array provided
         {
            if(!strcmp(tokens[i], ">"))
            {
               tokens[i] = NULL; // set the position of ">" to NULL so our exec function terminates on it when reached
               output = 1;
               strcpy(outFile, tokens[i + 1]);  // the next arg will be the output file name
            }
            else if(!strcmp(tokens[i], ">>"))
            {
               tokens[i] = NULL;
               output = 2;
               strcpy(outFile, tokens[i + 1]);
            }
            else if(!strcmp(tokens[i], "<"))
            {
               tokens[i] = NULL;
               input = 1;
               strcpy(inFile, tokens[i + 1]);
            }
         }
         /* Check if i/o redirection has been found */
         if(output == 1)
         {
            freopen(outFile, "w", stdout);
         }
         else if(output == 2)
         {
            freopen(outFile, "a", stdout);
         }
         if(input == 1)
         {
            freopen(inFile, "r", stdin);
         }
         execvp(tokens[0], tokens); // execute the command
         exit(EXIT_SUCCESS);
      default:                // parent
         if(background == 0)  // if the program is NOT in the supposed to be in the background...
         {
            waitpid(pid, &status, WUNTRACED);   // wait until the program is done executing
         }
         if(background == 1)  // if the program is supposed to be run in the background...
         {
            waitpid(pid, &status, WNOHANG);  // do NOT wait
         }
   } 
}

/* Quit the shell program */
void do_quit()
{
   exit(0);    // exit the program
   return;
}

/* Clear the screen */
void do_clear(char ** tokens, int background, int count)
{
   /* Replace clr with clear */
   for(int i = 0; i < count; i++)
   {
      if(!strcmp(tokens[i], "clr"))
      {
         tokens[i] = "clear";
      }
   }
   /* fork and execute the command */
   fork_execute(tokens, count, background);
   return;
}

/* Display the user manual */
void do_help(char ** tokens, char * path, int background, int count)
{
   int size = count + 2;
   char *args[size];
   args[0] = "more";
   args[1] = path;   // path = full path to user manual (readme file)
   int i = 2;
   int j = 1;
   /* Create a new array containing [more, PATH, ...] */
   while(tokens[j] != NULL)
   {
      args[i] = tokens[j];
      i++;
      j++;
   }
   args[size - 1] = NULL;  // Add NULL terminator
   i = 0;
   fork_execute(args, count, background);
}

/* Print a word or phrase */
void do_echo(char ** tokens, int count, int background)
{
   int i = 1;  // Set i to 1 to avoid printing "echo"
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

/* display contents of a directory */
void do_dir(char ** tokens, int count, int background)
{
   int size = count + 2;
   char *args[size]; // Create a new array to contain our command :- [ls, -al, DIR, ..., NULL]
   args[0] = "ls";
   args[1] = "-al";
   int i = 2;
   int j = 1;
   while(tokens[j] != NULL)   // Copy contents from tokens to args
   {
      args[i] = tokens[j];
      i++;
      j++;
   }
   args[size - 1] = NULL;
   i = 0;
   fork_execute(args, count, background);
}

/* Returns the cwd as a string */
char * curr_dir()
{
   char *dir = malloc(MAX_BUFFER);
   getcwd(dir, MAX_BUFFER);
   return dir;
}

/* Change an environment variable */
// USED TO UNDERSTAND SETENV: https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.2.0/com.ibm.zos.v2r2.bpxbd00/rputen.htm
void set_env_var(char *var)
{
   /* Get current directory */
   char * dir;
   dir = curr_dir();
   
   if(!strcmp(var, "SHELL")) // if the parameter is SHELL, we want to set the env variable shell
   {
      setenv(var, strcat(dir, "/myshell"), 1);
   }
   else  // otherwise we want to change the env variable var to the cwd
   {
      setenv(var, dir, 1);
   }
   free(dir);
   return;
}

/* Change directory to dest */
void do_cd(char * dest)
{
   if(chdir(dest) == -1)   // Try to change directory, if it fails enter report failure and exit function
   {
      printf("Error: cannot find the directory %s\n", dest);
      return;
   }
   set_env_var("PWD");  // otherwise set PWD var to current directory
   return;
}

/* Print all environment string variables */
// USED TO UNDERSTAND AND DO ENVIRON: https://man7.org/linux/man-pages/man7/environ.7.html
void do_environ()
{
   extern char **environ;  // Get environment string variables
   for(int i = 0; environ[i] != NULL; i++)   // Go through them until we reach NULL
   {
      printf("%s\n", environ[i]);
   }
   return;
}

/* Pause the screen until enter is pressed */
void do_pause()
{
   char input[MAX_BUFFER]; // String to hold user input
   printf("Press [enter] to continue...\n");
   while(strcmp(input, "\n"))    // Keep getting input until a single newline is entered, i.e. enter by itself
   {
      fgets(input, MAX_BUFFER, stdin);
   }
   return;
}

/* The fork_execute function doesn't work for some internal commands, so a seperate function was created */
void execute_internal_command(char * command, char ** tokens, int count, int background)
{
   int status; // Used to hold status
   int output;
   pid_t pid = getpid();
   switch (pid = fork ()) 
   { 
      case -1: // failure
         exit(EXIT_FAILURE); 
      case 0:                 // child proccess
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
         if(!strcmp(command, "echo"))  // if command name is echo
         {
            do_echo(tokens, count, background);
         }
         else if(!strcmp(command, "environ"))   // if command name is environ
         {
            do_environ();
         }
         else if(!strcmp(command, "pause"))  // if command name is pause
         {
            do_pause();
         }
         exit(EXIT_SUCCESS);
      default:                // parent process
         if(background == 0)  // Foreground
         {
            waitpid(pid, &status, WUNTRACED);
         }
         if(background == 1)  // Background
         {
            waitpid(pid, &status, WNOHANG);
         }
   } 
}