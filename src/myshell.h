/*
file: myshell.h
author: Ben Guiden
student number: 19310046
email: ben.guiden2@mail.dcu.ie

I have read and understood the DCU Academic Integrity and Plagiarism Policy. All work 
is completely my own unless indicated otherwise.

*************************************************************************************

This file contains function prototypes

************************************************************************************/

/* Function prototypes */
void my_prompt();
void do_clear(char ** tokens, int background, int count);
int word_counter(char*  line);
void do_quit();
void do_help(char ** tokens, char * path, int background, int count);
void do_echo(char ** tokens, int count, int background);
void do_dir(char ** tokens, int count, int background);
char * curr_dir();
void do_cd(char ** tokens, int background);
void do_environ();
void set_env_var(char *var);
void do_pause();
void execute_internal_command(char * command, char ** tokens, int count, int background);
void fork_execute(char ** tokens, int count, int background);